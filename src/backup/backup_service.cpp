#include "android_files_backup/backup/backup_service.h"

#include <qglobal.h>

#include <QDebug>
#include <QFileInfo>
#include <expected>

#include "android_files_backup/adb/adb_client.h"
#include "android_files_backup/adb/adb_device.h"
#include "android_files_backup/backup/backup_progress.h"
#include "android_files_backup/result/result.h"
#include "android_files_backup/utils/utils.h"

namespace android_files_backup {

std::expected<BackupResult, QString>
BackupService::performFilesPull_functionForTesting(
    const AdbClient &adbClient, const AdbDevice &device, const QString remote,
    const QString target, const QString condition,
    const ProgressCallback &progressCallback) {
  BackupResult result;

  const auto files = adbClient.findFiles(device, remote);
  // adbClient.runForDevice(device, {"shell", "find", remote});

  if (!files.has_value()) {
    return std::unexpected("Błąd:\n" + files.error());
  }

  const qsizetype total = files.value().size();

  auto pattern =
      android_files_backup::fromWildCardToRegularExpression(condition);

  for (auto i = 0; i < total; ++i) {
    const auto file = files.value()[i].trimmed();

    result.scannedFiles++;

    // qInfo().noquote() << QFileInfo(file).fileName();
    // qInfo().noquote() << file;

    const QString fileName = QFileInfo(file).fileName();

    if (pattern.match(fileName).hasMatch()) {
      const auto pullResult = adbClient.pullFile(device, file, target);

      if (!pullResult.has_value()) {
        result.skippedFiles++;
        result.errors.append(pullResult.error());
        continue;
      }

      result.copiedFiles++;
    }

    if (progressCallback) {
      progressCallback(
          {.processedFiles = i + 1, .totalFiles = total, .currentFile = file});
    }
  }
  /*
  qInfo() << result.copiedFiles << " " << result.scannedFiles << " "
          << result.skippedFiles;
  */
  return result;
}
}  // namespace android_files_backup