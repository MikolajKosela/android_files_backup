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