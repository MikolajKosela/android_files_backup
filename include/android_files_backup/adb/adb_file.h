#pragma once

#include <QDateTime>
#include <QString>

namespace android_files_backup {

enum class FileType {
  kRegularFile,
  kDirectory,
  kSymbolicLink,
  kBlockDevice,
  kCharacterDevice,
  kSocket,
  kNamedPipe,
  kUnknown
};

class AdbFile {
 private:
  QString name_;
  QString path_;
  FileType type_;
  QString permissions_;
  qint64 size_;
  QDateTime modification_time_;

 public:
  AdbFile(QString name, QString path, FileType type, QString permissions,
          qint64 size, QString modification_time);

  const QString& name() const;
  const QString& path() const;
  FileType type() const;
  const QString& permissions() const;
  qint64 size() const;
  const QDateTime& modificationTime() const;
};

QString fileTypeToString(FileType type);

}  // namespace android_files_backup
