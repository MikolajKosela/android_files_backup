#include <qdebug.h>
#include <qfileinfo.h>
#include <qglobal.h>
#include <qnamespace.h>

#include <QFileInfo>
#include <QRegularExpression>
#include <QString>
#include <QStringList>
#include <expected>

#include "android_files_backup/adb/adb_file.h"
#include "android_files_backup/adb/adb_parsers.h"

namespace android_files_backup {
const std::expected<AdbFile, QString> parseFileLine(const QString& line,
                                                    const QString& parentPath) {
  const QStringList parts = line.simplified().split(' ');

  if (parts.size() < 8) {
    return std::unexpected("Invalid ls output: too few fields");
  }

  const QString& permissions = parts[0];
  const QString& size_string = parts[4];
  const QString& date = parts[5];
  const QString& time = parts[6];
  const QString name = parts.mid(7).join(' ').replace("\\ ", " ");
  const QString& path = parentPath + "/" + name;

  if (permissions.isEmpty()) {
    return std::unexpected("Invalid ls output: missing permissions");
  }

  bool size_ok = false;
  const qint64 size = size_string.toLongLong(&size_ok);

  if (!size_ok) {
    return std::unexpected("Invalid ls output: invalid file size");
  }

  const QString modification_time = date + " " + time;

  FileType type;

  switch (permissions[0].unicode()) {
    case '-':
      type = FileType::kRegularFile;
      break;
    case 'd':
      type = FileType::kDirectory;
      break;
    case 'l':
      type = FileType::kSymbolicLink;
      break;
    case 'b':
      type = FileType::kBlockDevice;
      break;
    case 'c':
      type = FileType::kCharacterDevice;
      break;
    case 's':
      type = FileType::kSocket;
      break;
    case 'p':
      type = FileType::kNamedPipe;
      break;
    default:
      type = FileType::kUnknown;
      break;
  }

  return AdbFile(name, path, type, permissions, size, modification_time);
}
}  // namespace android_files_backup