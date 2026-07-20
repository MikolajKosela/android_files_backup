#include "android_files_backup/backup/backup_service.h"
#include "android_files_backup/adb/adb_client.h"
#include "android_files_backup/adb/adb_device.h"
#include "android_files_backup/backup/backup_progress.h"
#include "android_files_backup/errors/exceptions.h"
#include "android_files_backup/result/result.h"
#include "android_files_backup/utils/utils.h"

#include <QDebug>
#include <QFileInfo>
#include <qglobal.h>

namespace android_files_backup {

BackupResult BackupService::performFilesPull_functionForTesting(
    const AdbClient &adbClient, const AdbDevice &device, const QString remote,
    const QString target, const QString condition,
    const ProgressCallback &progressCallback) {

    BackupResult result;

    newDirectory(target);

    QStringList files;
    try {
        files = adbClient.runForDevice(device, {"shell", "find", remote});

    } catch (const AdbException &error) {
        throw BackupException(
            QStringLiteral(
                "Nie znaleziono podanej ścieżki na wybranym telefonie\n%1")
                .arg(QString(error.what())));

        result.errors.append(QString(error.what()));
        return result;
    }

    const qsizetype total = files.size();

    auto pattern =
        android_files_backup::fromWildCardToRegularExpression(condition);

    for (auto i = 0; i < total; ++i) {
        const auto file = files[i].trimmed();

        result.scannedFiles++;

        // qInfo().noquote() << file;
        // qInfo().noquote() << file;

        const QString fileName = QFileInfo(file).fileName();

        if (pattern.match(fileName).hasMatch()) {
            try {
                adbClient.runForDevice(device, {"pull", "-a", file, target});
            } catch (const AdbException &error) {
                result.skippedFiles++;
                result.errors.append(error.what());
                continue;
            }

            result.copiedFiles++;
        }

        if (progressCallback) {
            progressCallback({.processedFiles = i + 1,
                              .totalFiles = total,
                              .currentFile = file});
        }
    }
    /*
    qInfo() << result.copiedFiles << " " << result.scannedFiles << " "
            << result.skippedFiles;
    */
    return result;
}
} // namespace android_files_backup