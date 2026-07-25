#include "android_files_backup/ui/cli_application.h"
#include <QFileInfo>

namespace android_files_backup {

QString toDisplayPath(QString path, QStringList memoryCards) {
    static const QStringList internalStoragePrefixes = {"/storage/emulated/0",
                                                        "/sdcard"};

    for (const QString &prefix : internalStoragePrefixes) {
        if (path == prefix) {
            return "Pamięć wewnętrzna aka /sdcard lub /storage/emulated/0";
        }

        if (path.startsWith(prefix + '/')) {
            path.replace(0, prefix.size(), "Pamięć wewnętrzna");
            return path;
        }
    }

    for (const QString &memoryCard : memoryCards) {
        if (path == memoryCard) {
            return "Karta pamięci " + QFileInfo(path).fileName() + " aka " +
                   path;
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
        clearScreen();
        output_ << "Poczekaj, aż program przeskanuje ten katalog";
        output_.flush();
        const QStringList list = controller_.listRemoteDirectories(currentPath);

        clearScreen();
        output_ << "\n**Wybierz katalog w pamięci telefonu:**\n"
                << "*Przydatne skróty: \n"
                << " 0 Pamięć wewnętrzna \n";

        const QStringList memoryCards = controller_.listMemoryCards();
        for (auto i = 0; i < memoryCards.size(); i++) {
            output_ << " " << i * (-1) - 1 << ". Karta Pamięci "
                    << QFileInfo(memoryCards[i]).fileName() << "\n";
        }

        output_ << "\n*Bieżący katalog: \n"
                << toDisplayPath(currentPath, memoryCards) << ":\n"
                << " 1. . (Wybierz ten katalog)\n"
                << " 2. .. (Przejdź wyżej)\n";

        for (qsizetype i = 0; i < list.size(); ++i) {
            output_ << " " << i + 3 << ". " << QFileInfo(list[i]).fileName()
                    << "\n";
        }

        output_.flush();

        const int choice = readInteger(
            "Wybierz opcję: ", static_cast<int>(memoryCards.size()) * (-1),
            static_cast<int>(list.size()) + 2);

        if (choice < 0) {
            currentPath = memoryCards[(-1) * choice - 1];
            continue;
        }

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