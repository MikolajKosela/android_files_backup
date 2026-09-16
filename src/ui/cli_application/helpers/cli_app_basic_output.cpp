#include "android_files_backup/ui/cli_application.h"

namespace android_files_backup {

void CliApplication::clearScreen() {
  output_ << "\x1B[2J\x1B[3J\x1B[H";
  //"\x1B[2J\x1B[H";
  output_ << displayDespiteCleaning;
  output_.flush();
}

void CliApplication::showCaption(const QString caption) {
  output_ << caption;
  output_.flush();
}

void CliApplication::showError(const QString error) {
  error_ << error;
  error_.flush();
}

}  // namespace android_files_backup