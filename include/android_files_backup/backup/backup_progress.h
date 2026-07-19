#pragma once

#include <QString>
#include <qglobal.h>

#include <functional>

namespace android_files_backup {
struct BackupProgress {
    qsizetype processedFiles = 0;
    qsizetype totalFiles = 0;

    QString currentFile;
};

using ProgressCallback = std::function<void(const BackupProgress &)>;
} // namespace android_files_backup