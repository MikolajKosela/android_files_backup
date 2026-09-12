#include "android_files_backup/adb/adb_file.h"

namespace android_files_backup {

AdbFile::AdbFile(QString name, FileType type, QString permissions, qint64 size,
                 QString modification_time)
    : name_(name),
      type_(type),
      permissions_(permissions),
      size_(size),
      modification_time_(
          QDateTime::fromString(modification_time, "yyyy-MM-dd HH:mm")) {}

FileType AdbFile::type() const { return type_; }

const QString& AdbFile::permissions() const { return permissions_; }

qint64 AdbFile::size() const { return size_; }

const QDateTime& AdbFile::modificationTime() const {
  return modification_time_;
}

}  // namespace android_files_backup