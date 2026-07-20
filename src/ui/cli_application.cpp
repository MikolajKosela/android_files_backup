#include "android_files_backup/ui/cli_application.h"
#include "android_files_backup/adb/adb_device.h"
#include "android_files_backup/application/application_controller.h"
#include "android_files_backup/backup/backup_progress.h"
#include "android_files_backup/errors/exceptions.h"
#include <qglobal.h>

namespace android_files_backup {

CliApplication::CliApplication(ApplicationController &controller)
    : controller_(controller), input_(stdin), output_(stdout), error_(stderr) {}

int CliApplication::run() {
    while (true) {
        choiceDevice();
        createFilesPull_functionForTesting("/sdcard/DCIM/Screenshots",
                                           "build/test/ang",
                                           "*Diki sownik angielskiego*");
        createFilesPull_functionForTesting("/sdcard/DCIM/Screenshots",
                                           "build/test/niem",
                                           "*Diki sownik niemieckiego*");
        return 0;
    }
}

int CliApplication::readInteger(const QString &prompt, int minimun,
                                int maximum) {
    while (true) {
        output_ << prompt;
        output_.flush();

        const QString line = input_.readLine().trimmed();

        bool ok = false;
        const int value = line.toInt(&ok);

        if (!ok) {
            error_ << "Podaj poprawną liczbę. \n";
            error_.flush();
            continue;
        }

        if (value < minimun || value > maximum) {
            error_ << "Wartość musi być z zakresu: " << minimun << " - "
                   << maximum << "\n";
            error_.flush();
        }

        return value;
    }
}

QString CliApplication::readLine(const QString &prompt) {
    output_ << prompt;
    output_.flush();

    return input_.readLine().trimmed();
}

void CliApplication::showDevices() {
    controller_.refreshDevices();

    const auto &devices = controller_.devices();

    if (devices.isEmpty()) {
        output_ << "Nie znaleziono urządzeń \n";
        output_.flush();
    }

    output_ << "Znalezione urządzenia: \n";

    for (auto i = 0; i < devices.size(); i++) {
        auto &device = devices[i];
        output_ << "[" << i << "] " << device.serial << device.model
                << " state: " << deviceStateToString(device.state) << "\n";
    }

    output_.flush();
}

void CliApplication::choiceDevice() {
    showDevices();

    int devicesNum = controller_.devices_.size();

    const int choice = readInteger("Wybierz urządzenie: ", 0, devicesNum - 1);

    const AdbDevice &device = controller_.devices_[choice];

    controller_.selectDevice(device.serial);

    output_ << "Wybrano " << device.serial << " " << device.model
            << "\n---------- \n";
    output_.flush();
}

void CliApplication::createFilesPull_functionForTesting(QString remote,
                                                        QString target,
                                                        QString condition) {

    output_ << "Kopiuję pliki z " << remote << " do " << target
            << " spełniające warunek: " << condition << "\n";

    output_.flush();

    try {
        controller_.createFilesPull_functionForTesting(
            remote, target, condition, [this](const BackupProgress &progress) {
                output_ << "\r\x1B[2KPostęp: " << progress.processedFiles
                        << " / " << progress.totalFiles << " "
                        << progress.currentFile;
                output_.flush();
            });

    } catch (const AdbException &error) {
        error_ << "\nBłąd komunikacji adb: " << error.what();
        error_.flush();

        output_ << "Nie udało się wykonać kopii zapasowej\n---------- \n";
        output_.flush();
        return;
    }

    output_ << "Pomyślnie wykonano kopię :) \n---------- \n";
    output_.flush();
}

} // namespace android_files_backup