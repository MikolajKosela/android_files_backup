#include "android_files_backup/ui/cli_application.h"
#include <QDir>

namespace android_files_backup {

QString CliApplication::chooseLocalDirectory() {
    QString currentPath = QDir::currentPath();

    while (true) {
        clearScreen();
        output_ << "Poczekaj, aż program przeskanuje ten katalog";
        output_.flush();

        QDir directory(currentPath);

        const QFileInfoList directiories = directory.entryInfoList(
            QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name | QDir::IgnoreCase);

        clearScreen();
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

} // namespace android_files_backup