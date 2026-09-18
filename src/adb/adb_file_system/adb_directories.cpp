#include <QFileInfo>
#include <QRegularExpression>
#include <QString>
#include <QStringList>
#include <expected>

#include "android_files_backup/adb/adb_device.h"
#include "android_files_backup/adb/adb_file_system.h"

namespace android_files_backup {

std::expected<QStringList, QString> AdbFileSystem::listDirectories(
    const AdbDevice &device, const QString &root) const {
  const auto paths =
      adbClient_.shell(device.serial, {"ls", "-d", root + "/*/"});
  if (!paths.has_value()) {
    return std::unexpected("Błąd przy listowaniu katalogów na telefonie\n" +
                           paths.error());
  }

  QStringList list;
  list.reserve(paths->size());

  for (QString path : *paths) {
    while (path.endsWith('/')) path.chop(1);

    list.append(path);
  }
  return list;
}

[[nodiscard]] std::expected<QString, QString> AdbFileSystem::getParentDirectory(
    const AdbDevice &device, const QString &child) const {
  const auto result = adbClient_.shell(
      device.serial, {"sh", "-c", "'cd " + child + " && cd .. && pwd'"});

  if (!result.has_value()) {
    return std::unexpected("Błąd przy przechodzeniu do katalogu wyżej\n" +
                           result.error());
  }

  QString path = result->first();

  if (path.endsWith('/')) {
    path.chop(1);
  }

  return path;
}

[[nodiscard]] std::expected<QStringList, QString>
AdbFileSystem::listMemoryCards(const AdbDevice &device) const {
  const auto paths =
      adbClient_.shell(device.serial, {"ls", "-d", "/storage/*"});

  if (!paths.has_value()) {
    return std::unexpected("Błąd przy listowaniu kart pamięci\n" +
                           paths.error());
  }

  QStringList results;

  for (const auto &path : *paths) {
    if (path != "/storage/emulated" && path != "/storage/self") {
      results.append(path);
    }
  }
  return results;
}

}  // namespace android_files_backup