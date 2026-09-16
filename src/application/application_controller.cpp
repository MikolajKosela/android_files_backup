#include "android_files_backup/application/application_controller.h"

#include <qnamespace.h>

#include <QDebug>
#include <expected>
#include <optional>

#include "android_files_backup/adb/adb_device.h"
#include "android_files_backup/adb/adb_file_system.h"
#include "android_files_backup/backup/backup_progress.h"

namespace android_files_backup {
ApplicationController::ApplicationController() {}

void ApplicationController::refreshDevices() {
  if (const auto result = adbClient_.listDevices(); result.has_value()) {
    devices_ = result.value();
  }
}

const QList<AdbDevice> &ApplicationController::devices() const {
  return devices_;
}

void ApplicationController::selectDevice(const QString &serial) {
  for (const auto &device : devices_) {
    if (device.serial == serial && device.isUsable()) {
      usedDevice_ = device;
    }
  }
}

bool ApplicationController::hasSelectedDevice() const {
  if (usedDevice_.has_value() && usedDevice_->isUsable()) {
    return true;
  }
  return false;
}

std::expected<BackupResult, QString>
ApplicationController::createFilesPull_functionForTesting(
    const QString remote, const QString target, const QString condition,
    const ProgressCallback &progressCallback) {
  if (!hasSelectedDevice()) {
    return std::unexpected("Niewybrano urządzenia\n");
  }

  const auto result = backupService_.performFilesPull_functionForTesting(
      adbClient_, usedDevice_.value(), remote, target, condition,
      progressCallback);

  if (!result.has_value()) {
    return std::unexpected("Błąd przy wykonywaniu kopii:\n" + result.error());
  }

  return result.value();
}

std::expected<QStringList, QString>
ApplicationController::listRemoteDirectories(const QString &root) const {
  if (!hasSelectedDevice()) {
    return std::unexpected("Niewybrano urządzenia\n");
  }

  return adbClient_.listDirectories(usedDevice_.value(), root);
}

std::expected<QString, QString> ApplicationController::getRemoteParentDirectory(
    const QString &child) const {
  if (!hasSelectedDevice()) {
    return std::unexpected("Niewybrano urządzenia\n");
  }

  return adbClient_.getParentDirectory(usedDevice_.value(), child);
}

[[nodiscard]] std::expected<QStringList, QString>
ApplicationController::listMemoryCards() const {
  if (!hasSelectedDevice()) {
    return std::unexpected("Niewybrano urządzenia\n");
  }

  return adbClient_.listMemoryCards(usedDevice_.value());
}

}  // namespace android_files_backup