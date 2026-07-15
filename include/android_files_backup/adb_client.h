#pragma once 

#include "android_files_backup/adb_device.h"

#include <QList>
#include <QString>

namespace android_files_backup {

class AdbClient {
public:
    [[nodiscard]] QList<AdbDevice> listDevices() const;
private:
    [[nodiscard]] AdbDevice parseDeviceLine(const QString& line) const;

};

} // namespace android_files_backup