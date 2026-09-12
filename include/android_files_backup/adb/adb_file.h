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
 public:
  AdbFile(QString name, FileType type, QString permissions, qint64 size,
          QString modification_time);

  FileType type() const;
  const QString& permissions() const;
  qint64 size() const;
  const QDateTime& modificationTime() const;

 private:
  QString name_;
  FileType type_;
  QString permissions_;
  qint64 size_;
  QDateTime modification_time_;
};

}  // namespace android_files_backup
