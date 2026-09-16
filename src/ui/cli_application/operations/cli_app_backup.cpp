#include "android_files_backup/ui/cli_application.h"
#include "android_files_backup/utils/utils.h"

namespace android_files_backup {

void CliApplication::createFilesPull_functionForTesting(QString remote,
                                                        QString target,
                                                        QString condition) {
  clearScreen();
  newDirectory(target);

  const auto result = controller_.createFilesPull_functionForTesting(
      remote, target, condition, [this](const BackupProgress &progress) {
        showCaption(
            "\r\x1B[2KPostęp: " + QString::number(progress.processedFiles) +
            " / " + QString::number(progress.totalFiles) + " " +
            progress.currentFile);
      });

  if (!result.has_value()) {
    showError("\nDoszło do błędu w trakcie wykonywania przesyłu plików\n" +
              result.error());
    return;
  }

  if (result->success()) {
    showCaption(
        "\n\nPomyślnie wykonano przesył plików :) \n"
        "Nienapotkano żadnych błędów \n" +
        QStringLiteral("Przeskanowano tyle plików: %1 \nSkopiowano "
                       "tyle plików: %2 \n")
            .arg(result->scannedFiles)
            .arg(result->copiedFiles));
  } else {
    showCaption("\n\nWykonano przesył plików, jednak napotkano na problemy \n" +
                QStringLiteral("Przeskanowano tyle plików: %1 \nSkopiowano "
                               "tyle plików: %2 \nPominięto tyle plików: %3 \n")
                    .arg(result->scannedFiles)
                    .arg(result->copiedFiles)
                    .arg(result->skippedFiles) +
                "Błędy podczas przesyłania: " +
                QString::number(result->errors.size()) + "\n");

    const auto errorsCnt = result->errors.size();

    if (errorsCnt > 10) {
      showCaption("Lista pierwszych 10 błędów: \n");
      for (auto i = 0; i < 10; i++) {
        const QString &err = result->errors[i];

        showCaption("[" + QString::number(i + 1) + "] -> " + err + "\n");
      }
      showCaption("... Pozostało tyle błędów do oczytania: " +
                  QString::number(errorsCnt - 10) + "\n");
    } else {
      showCaption("Lista błędów: \n");
      for (auto i = 0; i < result->errors.size(); i++) {
        const auto &err = result->errors[i];
        showCaption("[" + QString::number(i + 1) + "] -> " + err + "\n");
      }
    }
  }
  showCaption("----------\n\n");
}

void CliApplication::createCustomFilesPull_functionForTesting() {
  displayDespiteCleaning = "";

  displayDespiteCleaning += "**Przesyłanie plików na komputer**\n";
  showCaption(displayDespiteCleaning);

  const QString remote = chooseRemoteDirectory();
  displayDespiteCleaning += "Prześlij z: " + remote + "\n";
  showCaption(displayDespiteCleaning);

  const QString destination = chooseLocalDirectory();
  displayDespiteCleaning += "Zapisz w: " + destination + "\n";
  showCaption(displayDespiteCleaning);

  const QString pattern = choosePattern();
  displayDespiteCleaning += "Wzorzec: " + pattern;

  displayDespiteCleaning +=
      "\n**Rozpoczynam procedurę przesyłania plików na komputer**\n";
  showCaption(displayDespiteCleaning);

  createFilesPull_functionForTesting(remote, destination, pattern);
  displayDespiteCleaning = "";

  waitForEnter();
}

[[nodiscard]] QString CliApplication::choosePattern() {
  clearScreen();
  QString result =
      readLine("Podaj wzorzec (wpisz * aby przesłać wszystkie pliki): ");

  return result;
}
}  // namespace android_files_backup