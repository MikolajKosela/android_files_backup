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
#include "android_files_backup/adb/adb_file_system.h"
#include "android_files_backup/adb/adb_parsers.h"

namespace android_files_backup {

std::expected<AdbDeviceState, QString> AdbFileSystem::getDeviceState(
    const QString &serial) const {
  const auto result = adbClient_.getState(serial, {});

  if (!result.has_value()) {
    return AdbDeviceState::Disconnected;
  }

  return parseDeviceState(*result);
}

std::expected<QList<AdbDevice>, QString> AdbFileSystem::listDevices() const {
  const auto list = adbClient_.devices({"-l"});

  if (!list.has_value()) {
    return std::unexpected("Wystąpił błąd podczas listowanie urządzeń\n" +
                           list.error());
  }

  QList<AdbDevice> devices;

  for (const QString &rawLine : *list) {
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