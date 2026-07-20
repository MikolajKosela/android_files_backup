#pragma once

#include <QList>
#include <QString>
#include <qglobal.h>

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

struct BackupResult {
    qsizetype copiedFiles = 0;
    qsizetype skippedFiles = 0;
    qsizetype scannedFiles = 0;
    QStringList errors;

    [[nodiscard]] bool success() const { return errors.isEmpty(); }
};

} // namespace android_files_backup