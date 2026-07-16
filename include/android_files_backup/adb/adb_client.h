#pragma once

#include "android_files_backup/adb/adb_device.h"

#include <QList>
#include <QString>

namespace android_files_backup {

class AdbClient {
  public:
    void pullFiles(const AdbDevice &device, const QString remote,
                   const QString target, const QString condition = "");
    [[nodiscard]] QList<AdbDevice> listDevices() const;

  private:
    void pullFile(const AdbDevice &device, const QString file,
                  const QString target);
};

} // namespace android_files_backup