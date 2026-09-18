#include <QString>
#include <QStringList>
#include <expected>

#include "android_files_backup/adb/adb_device.h"
#include "android_files_backup/adb/adb_file_system.h"

namespace android_files_backup {

[[nodiscard]] std::expected<QStringList, QString> AdbFileSystem::findFiles(
    const AdbDevice &device, const QString &remote) const {
  const auto result = adbClient_.shell(device.serial, {"find", remote});

  if (!result.has_value()) {
    return std::unexpected("Błąd przy znalezieniu plików z podanej ścieżki\n" +
                           result.error());
  }

  return *result;
}

[[nodiscard]] std::expected<void, QString> AdbFileSystem::pullFile(
    const AdbDevice &device, const QString &file, const QString &target) const {
  const auto result = adbClient_.pull(device.serial, {"-a", file, target});

  if (!result.has_value()) {
    return std::unexpected("Błąd przy kopiowaniu pliku\n" + result.error());
  }

  return {};
}

}  // namespace android_files_backup