#include "android_files_backup/adb/adb_client.h"
#include "android_files_backup/adb/adb_device.h"
#include "android_files_backup/errors/exceptions.h"
#include "android_files_backup/process/process_runner.h"
#include "android_files_backup/result/result.h"

#include <QFileInfo>
#include <QRegularExpression>
#include <QString>
#include <QStringList>
#include <qfileinfo.h>
#include <qglobal.h>
#include <qnamespace.h>

namespace android_files_backup {

AdbDevice parseDeviceLine(const QString &line) {
    const QStringList parts =
        line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);

    if (parts.size() < 2) {
        throw AdbException(
            QString("Nieprawidłowy wiersz adb: %1 \n").arg(line));
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

QStringList AdbClient::runForDevice(const AdbDevice &device,
                                    const QStringList &arguments) const {
    QStringList argumentsWithDevice = {"-s", device.serial};
    argumentsWithDevice.append(arguments);

    const ProcessResult processResult = runProcess("adb", argumentsWithDevice);

    if (!processResult.success()) {
        QString commend = "adb";
        for (const auto &arg : arguments) {
            commend += " " + arg;
        }

        throw AdbException(
            QStringLiteral("Wystąpił błąd podczas wykonywania komendy\n "
                           "%1 \n dla urządzenia %2 \nLogi: %3 %4 \n")
                .arg(commend)
                .arg(QString(device.serial))
                .arg(processResult.standardOutput)
                .arg(processResult.standardError));
    }
    return processResult.standardOutput.split('\n', Qt::SkipEmptyParts);
}

QList<AdbDevice> AdbClient::listDevices() const {
    const ProcessResult processResult = runProcess("adb", {"devices", "-l"});

    if (!processResult.success()) {
        throw AdbException(
            QStringLiteral(
                "Wystąpił błąd podczas listowania urządzeń \nLogi: %1 %2 \n")
                .arg(processResult.standardOutput)
                .arg(processResult.standardError));
    }

    QList<AdbDevice> devices;

    const QStringList lines =
        processResult.standardOutput.split('\n', Qt::SkipEmptyParts);

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