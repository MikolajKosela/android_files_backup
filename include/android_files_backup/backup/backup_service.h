#pragma once

#include <QString>

#include "android_files_backup/adb/adb_client.h"
#include "android_files_backup/adb/adb_device.h"
#include "android_files_backup/adb/adb_file_system.h"
#include "android_files_backup/backup/backup_progress.h"
#include "android_files_backup/backup/backup_result.h"

namespace android_files_backup {

class BackupService {
 public:
  explicit BackupService(AdbClient &adbClient);

  std::expected<BackupResult, QString> performFilesPull_functionForTesting(
      const AdbFileSystem &adbClient, const AdbDevice &device,
      const QString remote, const QString target, const QString condition,
      const ProgressCallback &progressCallback = {});

 private:
  AdbFileSystem adbFileSystem_;
};
}  // namespace android_files_backup