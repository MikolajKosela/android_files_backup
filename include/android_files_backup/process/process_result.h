#pragma once

#include <qglobal.h>

#include <QList>
#include <QString>

namespace android_files_backup {

struct ProcessResult {
  bool started = false;
  bool finished = false;
  int exitCode = -1;

  QString standardOutput;
  QString standardError;

  [[nodiscard]] bool success() const {
    return started && finished && exitCode == 0;
  }
};
}  // namespace android_files_backup