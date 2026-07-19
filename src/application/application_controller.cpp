#include "android_files_backup/application/application_controller.h"
#include "android_files_backup/adb/adb_client.h"
#include "android_files_backup/adb/adb_device.h"
#include "android_files_backup/backup/backup_progress.h"

#include <QDebug>
#include <optional>
#include <qnamespace.h>

namespace android_files_backup {
ApplicationController::ApplicationController() {}

void ApplicationController::refreshDevices() {
    devices_ = adbClient_.listDevices();
}

const QList<AdbDevice> &ApplicationController::devices() const {
    return devices_;
}

void ApplicationController::selectDevice(const QString &serial) {
    for (const auto &device : devices_) {
        if (device.serial == serial && device.isUsable()) {
            usedDevice_ = device;
        }
    }
}

bool ApplicationController::hasSelectedDevice() const {
    if (usedDevice_.has_value() && usedDevice_->isUsable()) {
        return true;
    }
    return false;
}

void ApplicationController::createFilesPull_functionForTesting(
    const QString remote, const QString target, const QString condition,
    const ProgressCallback &progressCallback) {

    if (hasSelectedDevice()) {
        return backupService_.performFilesPull_functionForTesting(
            adbClient_, usedDevice_.value(), remote, target, condition,
            progressCallback);
    }
}

} // namespace android_files_backup