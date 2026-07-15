#pragma once

#include <QString>
#include <QMap>

namespace android_files_backup {

enum class AdbDeviceState {
    Device, 
    Unauthorized,
    Offline,
    NoPermissions,
    Bootloader,
    Recovery,
    Sideload,
    Unknown
};

struct AdbDevice {
    QString serial;
    AdbDeviceState state = AdbDeviceState::Unknown;

    QString product;
    QString model;
    QString deviceName;
    QString transportId;

    QMap<QString, QString> properties;

    [[nodiscard]] bool isUsable() const {
        return state == AdbDeviceState::Device;
    }

    QString printableDevice() const;
};

[[nodiscard]] AdbDeviceState parseDeviceState(const QString& state);
[[nodiscard]] QString deviceStateToString(AdbDeviceState state);
} // namespace android_files_backup
