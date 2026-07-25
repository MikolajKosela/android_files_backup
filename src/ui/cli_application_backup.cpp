#include "android_files_backup/errors/exceptions.h"
#include "android_files_backup/ui/cli_application.h"
#include "android_files_backup/utils/utils.h"

namespace android_files_backup {

void CliApplication::createFilesPull_functionForTesting(QString remote,
                                                        QString target,
                                                        QString condition) {

    clearScreen();
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
        output_ << "\n\nPomyślnie wykonano przesył plików :) \n";
        output_ << "Nienapotkano żadnych błędów \n";
        output_ << QStringLiteral("Przeskanowano tyle plików: %1 \nSkopiowano "
                                  "tyle plików: %2 \n")
                       .arg(result.scannedFiles)
                       .arg(result.copiedFiles);
    } else {
        output_
            << "\n\nWykonano przesył plików, jednak napotkano na problemy \n";
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
    displayDespiteCleaning = "";

    displayDespiteCleaning += "**Przesyłanie plików na komputer**\n";
    output_ << displayDespiteCleaning;
    output_.flush();

    const QString remote = chooseRemoteDirectory();
    displayDespiteCleaning += "Prześlij z: " + remote + "\n";
    output_ << displayDespiteCleaning;
    output_.flush();

    const QString destination = chooseLocalDirectory();
    displayDespiteCleaning += "Zapisz w: " + destination + "\n";
    output_ << displayDespiteCleaning;
    output_.flush();

    const QString pattern = choosePattern();
    displayDespiteCleaning += "Wzorzec: " + pattern;

    displayDespiteCleaning +=
        "\n**Rozpoczynam procedurę przesyłania plików na komputer**\n";
    output_ << displayDespiteCleaning;
    output_.flush();

    createFilesPull_functionForTesting(remote, destination, pattern);
    displayDespiteCleaning = "";
}

[[nodiscard]] QString CliApplication::choosePattern() {
    clearScreen();
    QString result =
        readLine("Podaj wzorzec (wpisz * aby przesłać wszystkie pliki): ");

    return result;
}
} // namespace android_files_backup