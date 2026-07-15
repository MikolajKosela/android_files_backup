#pragma once

#include <QString>
#include <QStringList>

namespace android_files_backup {

struct ProcessResult {
    bool started = false;
    bool finished = false;
    int exitCode = -1;

    QString standardOutput;
    QString standardError;

    [[nodiscard]] bool success() const;
};

[[nodiscard]] ProcessResult runProcess(
    const QString& program,
    const QStringList& arguments,
    int timeoutMs = 30'000
);

} // namespace android_files_backup