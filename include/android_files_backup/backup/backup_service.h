#pragma once

#include "android_files_backup/adb/adb_client.h"
#include "android_files_backup/adb/adb_device.h"
#include <QString>

namespace android_files_backup {

class BackupService {
  public:
    void performFilesPull_functionForTesting(const AdbClient &adbClient,
                                             const AdbDevice &device,
                                             const QString remote,
                                             const QString target,
                                             const QString condition);
};
} // namespace android_files_backup