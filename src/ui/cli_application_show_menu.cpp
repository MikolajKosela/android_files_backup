#include "android_files_backup/ui/cli_application.h"

namespace android_files_backup {

int CliApplication::showMenu(const QString &header, const QStringList &options,
                             int firstOption) {
  output_ << "\n" << header << "\n";

  for (qsizetype i = 0; i < options.size(); ++i) {
    output_ << " " << firstOption + i << ". " << options[i] << "\n";
  }

  output_.flush();

  return readInteger("Wybierz opcję: ", firstOption,
                     firstOption + options.size() - 1);
}

}  // namespace android_files_backup