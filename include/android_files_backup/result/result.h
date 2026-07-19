#pragma once

#include <QString>

namespace android_files_backup {

struct ProcessResult {
    bool started = false;
    bool finished = false;
    int exitCode = -1;

    QString standardOutput;
    QString standardError;

    [[nodiscard]] bool success() const;
};

} // namespace android_files_backup