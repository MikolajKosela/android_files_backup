#pragma once

#include <qlist.h>

#include <QList>
#include <QString>
#include <expected>
#include <optional>

#include "android_files_backup/adb/adb_client.h"
#include "android_files_backup/adb/adb_device.h"
#include "android_files_backup/backup/backup_service.h"
#include "android_files_backup/result/result.h"

namespace android_files_backup {

class ApplicationController {
 public:
  ApplicationController();

  void refreshDevices();

  [[nodiscard]] const QList<AdbDevice> &devices() const;

  void selectDevice(const QString &serial);

  std::expected<QStringList, QString> listRemoteDirectories(
      const QString &root) const;

  std::expected<QString, QString> getRemoteParentDirectory(
      const QString &child) const;

  [[nodiscard]] bool hasSelectedDevice() const;

  [[nodiscard]] std::expected<BackupResult, QString>
  createFilesPull_functionForTesting(const QString remote, const QString target,
                                     const QString condition,
                                     const ProgressCallback &progressCallback);

  [[nodiscard]] std::expected<QStringList, QString> listMemoryCards() const;

  AdbClient adbClient_;
  QList<AdbDevice> devices_;
  BackupService backupService_;

  std::optional<AdbDevice> usedDevice_;
};

}  // namespace android_files_backup