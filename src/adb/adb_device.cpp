#include "android_files_backup/adb/adb_device.h"

#include <QDebug>

namespace android_files_backup {

AdbDeviceState parseDeviceState(const QString &state) {
    if (state == "device") {
        return AdbDeviceState::Device;
    }

    if (state == "unauthorized") {
        return AdbDeviceState::Unauthorized;
    }

    if (state == "offline") {
        return AdbDeviceState::Offline;
    }

    if (state == "no permissions") {
        return AdbDeviceState::NoPermissions;
    }

    if (state == "bootloader") {
        return AdbDeviceState::Bootloader;
    }

    if (state == "recovery") {
        return AdbDeviceState::Recovery;
    }

    if (state == "sideload") {
        return AdbDeviceState::Sideload;
    }

    return AdbDeviceState::Unknown;
}

QString deviceStateToString(AdbDeviceState state) {
    switch (state) {
    case AdbDeviceState::Device:
        return "device";
    case AdbDeviceState::Unauthorized:
        return "unauthorized";
    case AdbDeviceState::Offline:
        return "offline";
    case AdbDeviceState::NoPermissions:
        return "no permissions";
    case AdbDeviceState::Bootloader:
        return "bootloader";
    case AdbDeviceState::Recovery:
        return "recovery";
    case AdbDeviceState::Sideload:
        return "sideload";
    case AdbDeviceState::Unknown:
        return "unknown";
    }

    return "unknown";
}

QString AdbDevice::printableDevice() const {
    return QString("%1 | %2  %3")
        .arg(serial)
        .arg(model)
        .arg(android_files_backup::deviceStateToString(state));
}

} // namespace android_files_backup