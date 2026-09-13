#include "android_files_backup/adb/adb_file.h"

namespace android_files_backup {

QString fileTypeToString(FileType type) {
  switch (type) {
    case FileType::kRegularFile:
      return "regular file";

    case FileType::kDirectory:
      return "directory";

    case FileType::kSymbolicLink:
      return "symbolic link";

    case FileType::kBlockDevice:
      return "block device";

    case FileType::kCharacterDevice:
      return "character device";

    case FileType::kSocket:
      return "socket";

    case FileType::kNamedPipe:
      return "named pipe";

    case FileType::kUnknown:
      return "unknown";
  }

  return "unknown";
}

AdbFile::AdbFile(QString name, QString path, FileType type, QString permissions,
                 qint64 size, QString modification_time)
    : name_(name),
      path_(path),
      type_(type),
      permissions_(permissions),
      size_(size),
      modification_time_(
          QDateTime::fromString(modification_time, "yyyy-MM-dd HH:mm")) {}

const QString& AdbFile::name() const { return name_; }

const QString& AdbFile::path() const { return path_; }

FileType AdbFile::type() const { return type_; }

const QString& AdbFile::permissions() const { return permissions_; }

qint64 AdbFile::size() const { return size_; }

const QDateTime& AdbFile::modificationTime() const {
  return modification_time_;
}

}  // namespace android_files_backup