#include "android_files_backup/adb/adb_client.h"

#include <qdebug.h>
#include <qfileinfo.h>
#include <qglobal.h>
#include <qnamespace.h>

#include <QFileInfo>
#include <QRegularExpression>
#include <QString>
#include <QStringList>
#include <expected>

#include "android_files_backup/adb/adb_device.h"
#include "android_files_backup/process/process_runner.h"
#include "android_files_backup/result/result.h"

namespace android_files_backup {

std::expected<AdbDevice, QString> parseDeviceLine(const QString &line) {
  const QStringList parts =
      line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);

  if (parts.size() < 2) {
    return std::unexpected(
        QString("Nieprawidłowy wiersz adb: %1 \n").arg(line));
  }

  AdbDevice device;

  device.serial = parts[0];

  QString stateText = parts[1];

  if (stateText == "no" && parts.size() >= 3 && parts[2] == "permissions") {
    stateText = "no permissions";
  }

  device.state = parseDeviceState(stateText);

  for (qsizetype i = 2; i < parts.size(); ++i) {
    const QString &token = parts[i];

    const qsizetype colonPosition = token.indexOf(';');

    if (colonPosition <= 0) {
      continue;
    }

    const QString key = token.left(colonPosition);
    const QString value = token.mid(colonPosition + 1);

    device.properties.insert(key, value);
  }

  device.product = device.properties.value("product");

  device.model = device.properties.value("model");

  device.deviceName = device.properties.value("device");

  device.transportId = device.properties.value("transport_id");

  return device;
}

std::expected<QStringList, QString> AdbClient::runForDevice(
    const AdbDevice &device, const QStringList &arguments) const {
  QStringList argumentsWithDevice = {"-s", device.serial};
  argumentsWithDevice.append(arguments);

  QString commend = "adb";
  for (const auto &arg : arguments) {
    commend += " " + arg;
  }
  /*
  qInfo() << commend;
  */

  const ProcessResult processResult = runProcess("adb", argumentsWithDevice);

  if (!processResult.success()) {
    return std::unexpected(
        QString("Wystąpił błąd podczas wykonywania komendy\n "
                "%1 \n dla urządzenia %2 \nLogi: %3 %4 \n")
            .arg(commend)
            .arg(QString(device.serial))
            .arg(processResult.standardOutput)
            .arg(processResult.standardError));
  }
  return processResult.standardOutput.split('\n', Qt::SkipEmptyParts);
}

std::expected<QList<AdbDevice>, QString> AdbClient::listDevices() const {
  const ProcessResult processResult = runProcess("adb", {"devices", "-l"});

  if (!processResult.success()) {
    return std::unexpected(
        QString("Wystąpił błąd podczas listowania urządzeń \nLogi: %1 %2 \n")
            .arg(processResult.standardOutput)
            .arg(processResult.standardError));
  }

  QList<AdbDevice> devices;

  const QStringList lines =
      processResult.standardOutput.split('\n', Qt::SkipEmptyParts);

  for (const QString &rawLine : lines) {
    const QString line = rawLine.trimmed();

    if (line.isEmpty()) {
      continue;
    }

    if (line.startsWith("List of devices attached")) {
      continue;
    }

    if (line.startsWith("*")) {
      continue;
    }
    if (auto device = parseDeviceLine(line); device.has_value()) {
      devices.append(device.value());
    }
  }
  return devices;
}

std::expected<AdbDeviceState, QString> AdbClient::getDeviceState(
    const QString &serial) const {
  const ProcessResult result =
      runProcess("adb", {"-s", serial, "get-state"}, 5'000);

  if (!result.success()) {
    return AdbDeviceState::Disconnected;
  }

  const QString state = result.standardOutput.trimmed();

  return parseDeviceState(state);
}

std::expected<QStringList, QString> AdbClient::listDirectories(
    const AdbDevice &device, const QString &root) const {
  // const QStringList result = runForDevice(
  // device, {"shell", "sh", "-c", "'cd " + root + " && realpath */'"});

  const auto paths = runForDevice(device, {"shell", "ls", "-d", root + "/*/"});

  if (!paths.has_value()) {
    return std::unexpected(
        QString("Błąd w trakcie listowania folderu\n" + paths.error()));
  }

  QStringList list;
  list.reserve(paths.value().size());

  for (QString path : paths.value()) {
    while (path.endsWith('/')) path.chop(1);

    list.append(path);
  }
  return list;
}

[[nodiscard]] std::expected<QString, QString> AdbClient::getParentDirectory(
    const AdbDevice &device, const QString &child) const {
  const auto results = runForDevice(
      device, {"shell", "sh", "-c", "'cd " + child + " && cd .. && pwd'"});

  if (!results.has_value()) {
    return std::unexpected("Błąd przy przechodzeniu do katalogu wyżej\n" +
                           results.error());
  }

  QString path = results.value().first();
  // results[0]?

  if (path.endsWith('/')) {
    path.chop(1);
  }

  return path;
}

[[nodiscard]] std::expected<QStringList, QString> AdbClient::listMemoryCards(
    const AdbDevice &device) const {
  const auto paths = runForDevice(device, {"shell", "ls", "-d", "/storage/*"});

  if (!paths.has_value()) {
    return std::unexpected("Błąd przy listowaniu kart pamięci\n" +
                           paths.error());
  }

  QStringList results;

  for (const auto &path : paths.value()) {
    if (path != "/storage/emulated" && path != "/storage/self") {
      results.append(path);
    }
  }
  return results;
}

[[nodiscard]] std::expected<QStringList, QString> AdbClient::findFiles(
    const AdbDevice &device, const QString &remote) const {
  const auto result = runForDevice(device, {"shell", "find", remote});

  if (!result.has_value()) {
    return std::unexpected("Błąd przy znalezieniu plików z podanej ścieżki\n" +
                           result.error());
  }

  return *result;
}

[[nodiscard]] std::expected<QStringList, QString> AdbClient::pullFile(
    const AdbDevice &device, const QString &file, const QString &target) const {
  const auto result = runForDevice(device, {"pull", "-a", file, target});

  if (!result.has_value()) {
    return std::unexpected("Błąd przy kopiowaniu pliku\n" + result.error());
  }

  return *result;
}

}  // namespace android_files_backup