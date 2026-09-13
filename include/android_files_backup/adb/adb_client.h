#pragma once

#include <QList>
#include <QString>
#include <expected>

#include "android_files_backup/adb/adb_device.h"

namespace android_files_backup {

class AdbClient {
 private:
  std::expected<QStringList, QString> runForDevice(
      const AdbDevice &device, const QStringList &arguments) const;

 public:
  [[nodiscard]] std::expected<QList<AdbDevice>, QString> listDevices() const;

  [[nodiscard]] std::expected<AdbDeviceState, QString> getDeviceState(
      const QString &serial) const;

  [[nodiscard]] std::expected<QStringList, QString> listDirectories(
      const AdbDevice &device, const QString &root) const;

  [[nodiscard]] std::expected<QString, QString> getParentDirectory(
      const AdbDevice &device, const QString &child) const;

  [[nodiscard]] std::expected<QStringList, QString> listMemoryCards(
      const AdbDevice &device) const;

  [[nodiscard]] std::expected<QStringList, QString> findFiles(
      const AdbDevice &device, const QString &remote) const;

  [[nodiscard]] std::expected<QStringList, QString> pullFile(
      const AdbDevice &device, const QString &file,
      const QString &target) const;
};

}  // namespace android_files_backup