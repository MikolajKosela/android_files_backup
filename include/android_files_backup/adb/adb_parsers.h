#pragma once

#include <expected>

#include "android_files_backup/adb/adb_device.h"
#include "android_files_backup/adb/adb_file.h"

namespace android_files_backup {

const std::expected<AdbDevice, QString> parseDeviceLine(const QString& line);
const std::expected<AdbFile, QString> parseFileLine(const QString& line,
                                                    const QString& parentPath);

}  // namespace android_files_backup