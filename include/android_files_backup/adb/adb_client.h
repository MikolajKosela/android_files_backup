#pragma once

#include "android_files_backup/adb/adb_device.h"
#include "android_files_backup/process/process_runner.h"

#include <QList>
#include <QString>

namespace android_files_backup {

class AdbClient {
  public:
    [[nodiscard]] ProcessResult
    runForDevice(const AdbDevice &device, const QStringList &arguments) const;

    [[nodiscard]] QList<AdbDevice> listDevices() const;
};

} // namespace android_files_backup