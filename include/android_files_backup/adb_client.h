#pragma once 

#include "android_files_backup/adb_device.h"

#include <QList>
#include <QString>
#include <optional>

namespace android_files_backup {

class AdbClient {
public:
    void chooseDevice();
    void pullFiles(QString remote, QString target, QString condition = "");

private:
    void refreshDevicesList();
    QList<AdbDevice> devicesList;
    std::optional<AdbDevice> device;
    [[nodiscard]] QList<AdbDevice> listDevices() const;
    [[nodiscard]] AdbDevice parseDeviceLine(const QString& line) const;
};

} // namespace android_files_backup