#pragma once

#include <qglobal.h>

#include <QList>
#include <QString>

namespace android_files_backup {

struct BackupResult {
  qsizetype copiedFiles = 0;
  qsizetype skippedFiles = 0;
  qsizetype scannedFiles = 0;
  QStringList errors;

  [[nodiscard]] bool success() const { return errors.isEmpty(); }
};

}  // namespace android_files_backup