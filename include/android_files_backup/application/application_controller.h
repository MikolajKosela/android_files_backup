#pragma once

#include "android_files_backup/adb/adb_client.h"
#include "android_files_backup/adb/adb_device.h"
#include "android_files_backup/backup/backup_service.h"
#include "android_files_backup/result/result.h"

#include <QList>
#include <QString>

#include <optional>
#include <qlist.h>

namespace android_files_backup {

class ApplicationController {
  public:
    ApplicationController();

    void refreshDevices();

    [[nodiscard]] const QList<AdbDevice> &devices() const;

    void selectDevice(const QString &serial);

    [[nodiscard]] bool hasSelectedDevice() const;

    [[nodiscard]] BackupResult createFilesPull_functionForTesting(
        const QString remote, const QString target, const QString condition,
        const ProgressCallback &progressCallback);

    AdbClient adbClient_;
    QList<AdbDevice> devices_;
    BackupService backupService_;

    std::optional<AdbDevice> usedDevice_;
};

} // namespace android_files_backup