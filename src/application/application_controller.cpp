#include "android_files_backup/application/application_controller.h"
#include "android_files_backup/adb/adb_client.h"
#include "android_files_backup/adb/adb_device.h"
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

void ApplicationController::selectDevice() {
    qInfo() << "Wybieram urządzenie...";
    for (const auto &device : devices_) {
        if (!device.isUsable()) {
            qInfo() << "Urządzenie nie jest gotowe";
            return;
        }

        usedDevice_ = device;
    }
}

bool ApplicationController::hasSelectedDevice() const {
    if (usedDevice_ != std::nullopt && usedDevice_->isUsable()) {
        return true;
    }
    return false;
}

void ApplicationController::createFilesPull_debugFunction(
    const QString remote, const QString target, const QString condition) {

    if (usedDevice_.has_value()) {
        adbClient_.pullFiles(usedDevice_.value(), remote, target, condition);
    }
}

} // namespace android_files_backup