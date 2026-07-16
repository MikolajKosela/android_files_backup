#include "android_files_backup/adb/adb_client.h"
#include "android_files_backup/adb/adb_device.h"
#include "android_files_backup/process/process_runner.h"
#include "android_files_backup/utils/utils.h"

#include <QFileInfo>
#include <QRegularExpression>
#include <QStringList>
#include <qfileinfo.h>
#include <qglobal.h>
#include <qnamespace.h>
#include <stdexcept>

namespace android_files_backup {

AdbDevice parseDeviceLine(const QString &line) {
    const QStringList parts =
        line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);

    if (parts.size() < 2) {
        throw std::runtime_error(
            QString("Nieprawidłowy wierdz adb: %1").arg(line).toStdString());
    }

    AdbDevice device;

    device.serial = parts[0];

    QString stateText = parts[1];

    if (stateText == "no" && parts.size() >= 3 && parts[2] == "permissions") {
        stateText = "no permissions";
    }

    device.state = parseDeviceState(stateText);

    for (qsizetype i = 2; i < parts.size(); ++i) {
        const QString &token = parts[i];

        const qsizetype colonPosition = token.indexOf(';');

        if (colonPosition <= 0) {
            continue;
        }

        const QString key = token.left(colonPosition);
        const QString value = token.mid(colonPosition + 1);

        device.properties.insert(key, value);
    }

    device.product = device.properties.value("product");

    device.model = device.properties.value("model");

    device.deviceName = device.properties.value("device");

    device.transportId = device.properties.value("transport_id");

    return device;
}

void AdbClient::pullFile(const AdbDevice &device, const QString file,
                         const QString target) {
    const ProcessResult pullResult =
        runProcess("adb", {"-s", device.serial, "pull", "-a", file, target});

    if (pullResult.exitCode != 0) {
        qInfo() << "Wystąpił błąd przy wykonywaniu komendy:";
        qInfo() << "adb pull -a " + file + " " + target;
        qInfo().noquote() << pullResult.standardError;
        qInfo().noquote() << pullResult.standardOutput;
    }
}

void AdbClient::pullFiles(const AdbDevice &device, const QString remote,
                          const QString target, const QString condition) {
    qInfo().noquote() << "\nKopiuję pliki z" << remote << "do" << target
                      << "spełniające warunek:" << condition;

    android_files_backup::newDirectory(target);

    const ProcessResult result =
        runProcess("adb", {"-s", device.serial, "shell", "find", remote});

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
            pullFile(device, file, target);
        }
    }
    qInfo() << "Postęp: " << cnt << "/" << files.size();
}

QList<AdbDevice> AdbClient::listDevices() const {
    const ProcessResult result = runProcess("adb", {"devices", "-l"});

    if (!result.success()) {
        throw std::runtime_error(result.standardError.toStdString());
    }

    QList<AdbDevice> devices;

    const QStringList lines =
        result.standardOutput.split('\n', Qt::SkipEmptyParts);

    for (const QString &rawLine : lines) {
        const QString line = rawLine.trimmed();

        if (line.isEmpty()) {
            continue;
        }

        if (line.startsWith("List of devices attached")) {
            continue;
        }

        if (line.startsWith("*")) {
            continue;
        }
        devices.append(parseDeviceLine(line));
    }
    return devices;
}

} // namespace android_files_backup