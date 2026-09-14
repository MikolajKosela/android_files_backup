#include "android_files_backup/ui/cli_application.h"

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

}  // namespace android_files_backup