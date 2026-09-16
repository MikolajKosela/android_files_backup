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

namespace android_files_backup {

std::expected<QStringList, QString> AdbFileSystem::listDirectories(
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

[[nodiscard]] std::expected<QString, QString> AdbFileSystem::getParentDirectory(
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

[[nodiscard]] std::expected<QStringList, QString>
AdbFileSystem::listMemoryCards(const AdbDevice &device) const {
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

}  // namespace android_files_backup