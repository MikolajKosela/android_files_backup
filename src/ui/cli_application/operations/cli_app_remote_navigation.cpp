#include <QFileInfo>

#include "android_files_backup/ui/cli_application.h"

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
      return "Karta pamięci " + QFileInfo(path).fileName() + " aka " + path;
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
    showCaption("Poczekaj, aż program przeskanuje ten katalog");

    QStringList list;
    if (const auto result = controller_.listRemoteDirectories(currentPath);
        result.has_value()) {
      list.append(result.value());
    }

    clearScreen();
    QStringList memoryCards;

    if (const auto result = controller_.listMemoryCards(); result.has_value()) {
      memoryCards.append(result.value());
    }

    QString header =
        "**Wybierz katalog w pamięci telefonu:**\n*Bieżący katalog: \n" +
        toDisplayPath(currentPath, memoryCards) + ":";
    QStringList options;

    for (auto i = 0; i < memoryCards.size(); i++) {
      options << "Karta Pamięci " + QFileInfo(memoryCards[i]).fileName();
    }
    options << "Pamięć wewnętrzna";

    options << ". (Wybierz ten katalog)" << ".. (Przejdź wyżej)";

    for (qsizetype i = 0; i < list.size(); ++i) {
      options << QFileInfo(list[i]).fileName();
    }

    const int choice = showMenu(header, options, memoryCards.size() * (-1));

    if (choice < 0) {
      currentPath = memoryCards[(-1) * choice - 1];
      continue;

    } else if (choice == 0) {
      currentPath = "/sdcard";
      continue;

    } else if (choice == 1) {
      return currentPath;

    } else if (choice == 2) {
      currentPath = controller_.getRemoteParentDirectory(currentPath).value();
      continue;

    } else {
      currentPath = list[choice - 3];
    }
  }
  return currentPath;
}

}  // namespace android_files_backup