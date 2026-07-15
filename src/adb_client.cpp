#include "android_files_backup/adb_client.h"
#include "android_files_backup/adb_device.h"
#include "android_files_backup/process_runner.h"

#include <QRegularExpression>
#include <QStringList>
#include <qglobal.h>
#include <qnamespace.h>
#include <qregularexpression.h>
#include <stdexcept>

namespace android_files_backup {

QList<AdbDevice> AdbClient::listDevices() const {
    const ProcessResult result = 
        runProcess("adb", {"devices", "-l"});

    if (!result.success()) {
        throw std::runtime_error (
            result.standardError.toStdString()
        );
    }

    QList<AdbDevice> devices;

    const QStringList lines = 
        result.standardOutput.split(
            '\n',
            Qt::SkipEmptyParts
        );

    for (const QString& rawLine : lines) {
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

AdbDevice AdbClient::parseDeviceLine(const QString& line) const {
    const QStringList parts = 
        line.split(
            QRegularExpression("\\s+"),
            Qt::SkipEmptyParts
        );

    if (parts.size() < 2) {
        throw std::runtime_error(
            QString("Nieprawidłowy wierdz adb: %1")
                .arg(line)
                .toStdString()
        );
    }

    AdbDevice device;

    device.serial = parts[0];

    QString stateText = parts[1];

    if (stateText == "no" &&
        parts.size() >= 3 &&
        parts[2] == "permissions") {
            stateText = "no permissions";
        }

    device.state = parseDeviceState(stateText);

    for (qsizetype i = 2; i < parts.size(); ++i) {
        const QString& token = parts[i];

        const qsizetype colonPosition = token.indexOf(';');

        if (colonPosition <= 0) {
            continue;
        }

        const QString key = token.left(colonPosition);
        const QString value = token.mid(colonPosition + 1);

        device.properties.insert(key, value);
    }

    device.product =
        device.properties.value("product");

    device.model =
        device.properties.value("model");

    device.deviceName =
        device.properties.value("device");

    device.transportId =
        device.properties.value("transport_id");

    return device;
}


} // namespace android_files_backup