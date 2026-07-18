#include "android_files_backup/adb/adb_client.h"
#include "android_files_backup/adb/adb_device.h"
#include "android_files_backup/process/process_runner.h"

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

ProcessResult AdbClient::runForDevice(const AdbDevice &device,
                                      const QStringList &arguments) const {
    QStringList argumentsWithDevice = {"-s", device.serial};
    argumentsWithDevice.append(arguments);

    const ProcessResult result = runProcess("adb", argumentsWithDevice);

    if (!result.success()) {
        qInfo() << "Wystąpił błąd przy wykonywaniu komendy:";
        QString commend = "adb";
        for (const auto &arg : argumentsWithDevice) {
            commend += " " + arg;
        }

        qInfo().noquote() << commend;
        if (QString info = result.standardOutput; !info.isEmpty()) {
            qInfo().noquote() << info;
        }
        if (QString info = result.standardError; !info.isEmpty()) {
            qInfo().noquote() << info;
        }
    }

    return result;
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