#pragma once

#include "android_files_backup/adb/adb_device.h"

#include <QList>
#include <QString>

namespace android_files_backup {

class AdbClient {
  public:
    QStringList runForDevice(const AdbDevice &device,
                             const QStringList &arguments) const;

    [[nodiscard]] QList<AdbDevice> listDevices() const;
};

} // namespace android_files_backup