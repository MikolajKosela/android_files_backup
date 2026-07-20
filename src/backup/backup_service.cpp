#include "android_files_backup/backup/backup_service.h"
#include "android_files_backup/adb/adb_client.h"
#include "android_files_backup/adb/adb_device.h"
#include "android_files_backup/backup/backup_progress.h"
#include "android_files_backup/utils/utils.h"

#include <QDebug>
#include <QFileInfo>
#include <qglobal.h>

namespace android_files_backup {

void BackupService::performFilesPull_functionForTesting(
    const AdbClient &adbClient, const AdbDevice &device, const QString remote,
    const QString target, const QString condition,
    const ProgressCallback &progressCallback) {
    newDirectory(target);

    const QStringList files =
        adbClient.runForDevice(device, {"shell", "find", remote});

    const qsizetype total = files.size();

    auto pattern =
        android_files_backup::fromWildCardToRegularExpression(condition);

    for (auto i = 0; i < total; ++i) {
        const auto file = files[i].trimmed();

        if (progressCallback) {
            progressCallback({.processedFiles = i,
                              .totalFiles = total,
                              .currentFile = file});
        }

        // qInfo().noquote() << file;
        // qInfo().noquote() << file;

        const QString fileName = QFileInfo(file).fileName();

        if (pattern.match(fileName).hasMatch()) {
            adbClient.runForDevice(device, {"pull", "-a", file, target});
        }
    }
}
} // namespace android_files_backup