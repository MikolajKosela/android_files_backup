#include <qfileinfo.h>

#include <QDir>

#include "android_files_backup/ui/cli_application.h"

namespace android_files_backup {

QString CliApplication::chooseLocalDirectory() {
  QString currentPath = QDir::currentPath();

  while (true) {
    clearScreen();
    showCaption("Poczekaj, aż program przeskanuje ten katalog");

    QDir directory(currentPath);

    const QFileInfoList directories = directory.entryInfoList(
        QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name | QDir::IgnoreCase);

    clearScreen();
    QStringList options = {"Katalog domowy", ". (Wybierz ten katalog)",
                           ".. (Przejdź wyżej)"};

    for (const QFileInfo &directory : directories) {
      options << directory.fileName();
    }

    const int choice = showMenu(
        "**Zapisz pliki w katalogu:**\n"
        "*Przydatne skróty:\n"
        " 0 Katalog domowy\n"
        "*Bieżący katalog:\n" +
            currentPath + ":",
        options, 0);

    switch (choice) {
      case 0:
        currentPath = QDir::homePath();
        continue;
        break;

      case 1:
        return currentPath;
        break;

      case 2:
        QDir parent(currentPath);

        if (parent.cdUp()) {
          currentPath = parent.absolutePath();
        }

        continue;
    }

    currentPath = directories[choice - 3].absoluteFilePath();
  }

  return currentPath;
}

}  // namespace android_files_backup