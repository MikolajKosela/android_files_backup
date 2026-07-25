#include "android_files_backup/ui/cli_application.h"
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
        choiceDevice();
        createCustomFilesPull_functionForTesting();
        return 0;
    }
}

} // namespace android_files_backup