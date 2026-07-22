#include "android_files_backup/ui/cli_application.h"
#include "android_files_backup/adb/adb_device.h"
#include "android_files_backup/application/application_controller.h"
#include "android_files_backup/backup/backup_progress.h"
#include "android_files_backup/errors/exceptions.h"
#include "android_files_backup/result/result.h"
#include "android_files_backup/utils/utils.h"
#include <QDir>
#include <qfileinfo.h>
#include <qglobal.h>
#include <qstringliteral.h>

namespace android_files_backup {

CliApplication::CliApplication(ApplicationController &controller)
    : controller_(controller), input_(stdin), output_(stdout), error_(stderr) {}

int CliApplication::run() {
    while (true) {

        choiceDevice();
        /*
        createFilesPull_functionForTesting("/sdcard/DCIM/Screenshots",
                                           "build/test/ang",
                                           "*Diki sownik angielskiego*");
        createFilesPull_functionForTesting("/sdcard/DCIM/Screenshots",
                                           "build/test/niem",
                                           "*Diki sownik niemieckiego*");
        */
        createCustomFilesPull_functionForTesting();
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

        return;
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

    if (devicesNum == 0) {
        return;
    }

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

    output_ << "Przesyłam na komputer pliki z " << remote << " do " << target
            << " spełniające warunek: " << condition << "\n";

    newDirectory(target);

    output_.flush();

    BackupResult result;
    try {
        result = controller_.createFilesPull_functionForTesting(
            remote, target, condition, [this](const BackupProgress &progress) {
                output_ << "\r\x1B[2KPostęp: " << progress.processedFiles
                        << " / " << progress.totalFiles << " "
                        << progress.currentFile;
                output_.flush();
            });

    } catch (const AdbException &error) {
        error_ << "\nBłąd komunikacji adb: " << error.what();
        error_.flush();
    } catch (const BackupException &error) {
        error_ << "Doszło do błędu w trakcie wykonywania przesyłu plików\n"
               << error.what();
        error_.flush();

        output_ << "Nie udało się wykonać przesyłu plików\n---------- \n";
        output_.flush();
        return;
    }

    if (result.success()) {
        output_ << "\nPomyślnie wykonano przesył plików :) \n";
        output_ << "Nienapotkano żadnych błędów \n";
        output_ << QStringLiteral("Przeskanowano tyle plików: %1 \nSkopiowano "
                                  "tyle plików: %2 \n")
                       .arg(result.scannedFiles)
                       .arg(result.copiedFiles);
    } else {
        output_ << "\nWykonano przesył plików, jednak napotkano na problemy \n";
        output_ << QStringLiteral(
                       "Przeskanowano tyle plików: %1 \nSkopiowano "
                       "tyle plików: %2 \nPominięto tyle plików: %3 \n")
                       .arg(result.scannedFiles)
                       .arg(result.copiedFiles)
                       .arg(result.skippedFiles);
        output_ << "Błędy podczas przesyłania: " << result.errors.size()
                << "\n";
        const auto errorsCnt = result.errors.size();

        if (errorsCnt > 10) {
            output_ << "Lista pierwszych 10 błędów: \n";
            for (auto i = 0; i < 10; i++) {
                const QString &err = result.errors[i];

                output_ << "[" << i + 1 << "]" << " -> " << err << "\n";
            }
            output_ << "... Pozostało tyle błędów do oczytania: "
                    << errorsCnt - 10 << "\n";
            output_.flush();
        } else {
            output_ << "Lista błędów: \n";
            for (auto i = 0; i < result.errors.size(); i++) {
                const auto &err = result.errors[i];
                output_ << "[" << i + 1 << "]" << " -> " << err << "\n";
            }
        }
        output_.flush();
    }
    output_ << "----------\n\n";

    output_.flush();
}

void CliApplication::createCustomFilesPull_functionForTesting() {
    output_ << "**Przesyłanie plików na komputer**\n";
    output_.flush();

    const QString destination = chooseLocalDirectory();
    output_ << "Wybrano katalog: " << destination << "\n";
    output_.flush();

    const QString remote = chooseRemoteDirectory();
    output_ << "Wybrano katalog: " << remote << "\n";
    output_.flush();

    QString pattern = readLine("Podaj wzorzec: ");
    output_ << "\n**Rozpoczynam procedurę przesyłania plików na komputer**\n";
    output_.flush();

    createFilesPull_functionForTesting(remote, destination, pattern);
}

QString CliApplication::chooseLocalDirectory() {
    QString currentPath = QDir::currentPath();

    while (true) {
        QDir directory(currentPath);

        const QFileInfoList directiories = directory.entryInfoList(
            QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name | QDir::IgnoreCase);

        output_ << "\n**Zapisz pliki w katalogu:**\n"
                << "*Przydatne skróty: \n"
                << " 0 Katalog domowy\n"
                << "\n*Bieżący katalog: \n"
                << currentPath << ":\n"
                << " 1. . (Wybierz ten katalog)\n"
                << " 2. .. (Przejdź wyżej)\n";

        for (qsizetype i = 0; i < directiories.size(); ++i) {
            output_ << " " << i + 3 << ". " << directiories[i].fileName()
                    << "\n";
        }

        output_.flush();

        const int choice = readInteger(
            "Wybierz opcję: ", 0, static_cast<int>(directiories.size()) + 2);

        if (choice == 0) {
            currentPath = QDir::homePath();
            continue;
        }

        if (choice == 1) {
            return currentPath;
        }

        if (choice == 2) {
            QDir parent(currentPath);

            if (parent.cdUp()) {
                currentPath = parent.absolutePath();
            }

            continue;
        }

        currentPath = directiories[choice - 3].absoluteFilePath();
    }

    return currentPath;
}

QString toDisplayPath(QString path) {
    static const QStringList internalStoragePrefixes = {"/storage/emulated/0",
                                                        "/sdcard"};

    for (const QString &prefix : internalStoragePrefixes) {
        if (path == prefix) {
            return "Pamięć wewnętrzna";
        }

        if (path.startsWith(prefix + '/')) {
            path.replace(0, prefix.size(), "Pamięć wewnętrzna");
            return path;
        }
    }

    if (path.size() == 0) {
        path = "/";
    }

    return path;
}

QString CliApplication::chooseRemoteDirectory() {
    QString currentPath = "/sdcard";

    while (true) {
        const QStringList list = controller_.listRemoteDirectories(currentPath);

        output_ << "\n**Wybierz katalog w pamięci telefonu:**\n"
                << "*Przydatne skróty: \n"
                << " 0 Pamięć wewnętrzna \n"
                << "\n*Bieżący katalog: \n"
                << toDisplayPath(currentPath) << ":\n"
                << " 1. . (Wybierz ten katalog)\n"
                << " 2. .. (Przejdź wyżej)\n";

        for (qsizetype i = 0; i < list.size(); ++i) {
            output_ << " " << i + 3 << ". " << QFileInfo(list[i]).fileName()
                    << "\n";
        }
        output_.flush();

        const int choice = readInteger("Wybierz opcję: ", 0,
                                       static_cast<int>(list.size()) + 2);

        if (choice == 0) {
            currentPath = "/sdcard";
            continue;
        }

        if (choice == 1) {
            return currentPath;
        }

        if (choice == 2) {
            currentPath = controller_.getRemoteParentDirectory(currentPath);
            continue;
        }

        currentPath = list[choice - 3];
    }
    return currentPath;
}

} // namespace android_files_backup