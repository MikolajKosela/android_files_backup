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

namespace android_files_backup {

std::expected<AdbDevice, QString> AdbClient::parseDeviceLine(
    const QString &line) const {
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

}  // namespace android_files_backup