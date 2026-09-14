#pragma once

#include <QString>

#include "android_files_backup/adb/adb_client.h"
#include "android_files_backup/adb/adb_device.h"
#include "android_files_backup/backup/backup_progress.h"
#include "android_files_backup/backup/backup_result.h"

namespace android_files_backup {

class BackupService {
 public:
  std::expected<BackupResult, QString> performFilesPull_functionForTesting(
      const AdbClient &adbClient, const AdbDevice &device, const QString remote,
      const QString target, const QString condition,
      const ProgressCallback &progressCallback = {});
};
}  // namespace android_files_backup