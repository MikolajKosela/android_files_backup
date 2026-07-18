#include "android_files_backup/backup/backup_service.h"
#include "android_files_backup/adb/adb_client.h"
#include "android_files_backup/adb/adb_device.h"
#include "android_files_backup/utils/utils.h"

#include <QDebug>
#include <QFileInfo>

namespace android_files_backup {

void BackupService::performFilesPull_functionForTesting(
    const AdbClient &adbClient, const AdbDevice &device, const QString remote,
    const QString target, const QString condition) {
    newDirectory(target);

    const ProcessResult result =
        adbClient.runForDevice(device, {"shell", "find", remote});

    auto files = result.standardOutput.split('\n', Qt::SkipEmptyParts);

    auto pattern =
        android_files_backup::fromWildCardToRegularExpression(condition);

    long long int cnt = 0;

    for (auto file : files) {
        cnt++;
        if (cnt % 100 == 0) {
            qInfo() << "Postęp: " << cnt << "/" << files.size();
        }
        // qInfo().noquote() << file;
        file = file.trimmed();
        // qInfo().noquote() << file;

        const QString fileName = QFileInfo(file).fileName();

        if (pattern.match(fileName).hasMatch()) {
            const ProcessResult pullResult =
                adbClient.runForDevice(device, {"pull", "-a", file, target});
        }
    }
    qInfo() << "Postęp: " << cnt << "/" << files.size();
}
} // namespace android_files_backup