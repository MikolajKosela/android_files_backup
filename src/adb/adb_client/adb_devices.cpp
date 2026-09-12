#include <qdebug.h>
#include <qfileinfo.h>
#include <qglobal.h>
#include <qnamespace.h>

#include <QFileInfo>
#include <QRegularExpression>
#include <QString>
#include <QStringList>
#include <expected>

#include "android_files_backup/adb/adb_client.h"
#include "android_files_backup/adb/adb_device.h"
#include "android_files_backup/process/process_runner.h"
#include "android_files_backup/result/result.h"

namespace android_files_backup {

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

}  // namespace android_files_backup