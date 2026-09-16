#include <qfileinfo.h>
#include <qglobal.h>
#include <qhashfunctions.h>
#include <qstringliteral.h>

#include "android_files_backup/ui/cli_application.h"

namespace android_files_backup {

int CliApplication::showMainMenu() {
  clearScreen();

  const QStringList options = {"Wyjdź", "Wybierz telefon",
                               "Przesył plików z telefonu na komputer"};

  const int choice = showMenu("** Menu główne **\nWybierz opcję: ", options, 0);

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
      showCaption("\nWybrano opcję, której nie można obsłużyć\n");
      return 1;
      break;
  }

  return 0;
}

}  // namespace android_files_backup