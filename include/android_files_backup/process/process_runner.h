#pragma once

#include <QString>
#include <QStringList>

#include "android_files_backup/process/process_result.h"

namespace android_files_backup {

[[nodiscard]] ProcessResult runProcess(const QString &program,
                                       const QStringList &arguments,
                                       int timeoutMs = 30'000);

}  // namespace android_files_backup