#include "android_files_backup/adb_client.h"
#include "android_files_backup/adb_device.h"
#include "android_files_backup/process_runner.h"
#include "android_files_backup/utils.h"

#include <QFileInfo>
#include <QRegularExpression>
#include <QStringList>
#include <optional>
#include <qfileinfo.h>
#include <qglobal.h>
#include <qnamespace.h>
#include <stdexcept>

namespace android_files_backup {

// Public

void AdbClient::chooseDevice() {
    refreshDevicesList();

    if (devicesList.isEmpty()) {
        qInfo() << "Nie znaleziono żadnych urządzeń.";
        return;
    }

    qInfo() << "Podłączone urządzenia";
    for (qsizetype i = 0; i < devicesList.size(); ++i) {
        const auto &device = devicesList[i];

        qInfo().noquote() << QString("[%1] %2").arg(i).arg(
            device.printableDevice());
    }

    if (devicesList.size() == 1) {
        qInfo() << "Zostało wybrane jedyne podłączone urządzenie";
        device = devicesList[0];
        // qInfo().noquote() << device->printableDevice();
    } else {
        // Do dokończenia
    }
}

void AdbClient::pullFiles(QString remote, QString target, QString condition) {
    qInfo().noquote() << "\nKopiuję pliki z" << remote << "do" << target
                      << "spełniające warunek:" << condition;

    android_files_backup::newDirectory(target);

    const ProcessResult result =
        runProcess("adb", {"-s", device->serial, "shell", "find", remote});

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

        const QString fileName = QFileInfo(file).fileName();

        if (pattern.match(fileName).hasMatch()) {

            const ProcessResult pullResult =
                runProcess("adb", {"pull", file, target});
            if (pullResult.exitCode != 0) {
                qInfo() << "Wystąpił błąd przy wykonywaniu komendy:";
                qInfo() << "adb pull " + file + " " + target;
                qInfo().noquote() << pullResult.standardError;
                qInfo().noquote() << pullResult.standardOutput;
                return;
            }
        }
    }
    qInfo() << "Postęp: " << cnt << "/" << files.size();
}

// Private
void AdbClient::refreshDevicesList() { devicesList = listDevices(); }

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

AdbDevice AdbClient::parseDeviceLine(const QString &line) const {
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

} // namespace android_files_backup