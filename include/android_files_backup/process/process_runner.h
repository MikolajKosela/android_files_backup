#pragma once

#include "android_files_backup/result/result.h"

#include <QString>
#include <QStringList>

namespace android_files_backup {

[[nodiscard]] ProcessResult runProcess(const QString &program,
                                       const QStringList &arguments,
                                       int timeoutMs = 30'000);

} // namespace android_files_backup