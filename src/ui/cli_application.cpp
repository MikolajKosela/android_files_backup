#include "android_files_backup/ui/cli_application.h"
#include "android_files_backup/errors/exceptions.h"
#include <QDir>
#include <qfileinfo.h>
#include <qglobal.h>
#include <qhashfunctions.h>
#include <qstringliteral.h>

namespace android_files_backup {

CliApplication::CliApplication(ApplicationController &controller)
    : controller_(controller), input_(stdin), output_(stdout), error_(stderr) {}

int CliApplication::run() {
    while (true) {
        if (showMainMenu() == 1) {
            return 0;
        }
    }
}

int CliApplication::showMainMenu() {
    clearScreen();

    const QString menu = "** Menu główne **\n"
                         "Wybierz opcję: \n"
                         "0. Wyjdź \n"
                         "1. Wybierz telefon \n"
                         "2. Przesył plików z telefonu na komputer \n";

    showCaption(menu);

    const int choice = readInteger("Wybierz opcję: ", 0, 2);

    switch (choice) {
    case 0:
        showCaption("\nDo następnego\n");
        return 1;
        break;
    case 1:
        choiceDevice();
        break;
    case 2:
        createCustomFilesPull_functionForTesting();
        break;
    default:
        throw ApplicationException(
            "W menu została wybrana opcja, której nie można wykonać \n");
        return 1;
        break;
    }

    return 0;
}

} // namespace android_files_backup