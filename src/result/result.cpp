#include "android_files_backup/result/result.h"

#include <QString>

namespace android_files_backup {

bool ProcessResult::success() const {
    return started && finished && exitCode == 0;
}

} // namespace android_files_backup