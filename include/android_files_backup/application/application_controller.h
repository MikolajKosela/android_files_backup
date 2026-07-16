#pragma once

#include "android_files_backup/adb/adb_client.h"
#include "android_files_backup/adb/adb_device.h"

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

    void selectDevice();

    [[nodiscard]] bool hasSelectedDevice() const;

    void createFilesPull_debugFunction(const QString remote,
                                       const QString target,
                                       const QString condition);

    AdbClient adbClient_;
    QList<AdbDevice> devices_;

    std::optional<AdbDevice> usedDevice_;
};

} // namespace android_files_backup