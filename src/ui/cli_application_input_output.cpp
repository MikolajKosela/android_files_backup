#include "android_files_backup/ui/cli_application.h"

namespace android_files_backup {

void CliApplication::clearScreen() {
    output_ << "\x1B[2J\x1B[3J\x1B[H";
    //"\x1B[2J\x1B[H";
    output_ << displayDespiteCleaning;
    output_.flush();
}

int CliApplication::readInteger(const QString &prompt, int minimun,
                                int maximum) {
    while (true) {
        output_ << prompt;
        output_.flush();

        const QString line = input_.readLine().trimmed();

        bool ok = false;
        const int value = line.toInt(&ok);

        if (!ok) {
            error_ << "Podaj poprawną liczbę. \n";
            error_.flush();
            continue;
        }

        if (value < minimun || value > maximum) {
            error_ << "Wartość musi być z zakresu: " << minimun << " - "
                   << maximum << "\n";
            error_.flush();
            continue;
        }

        return value;
    }
}

QString CliApplication::readLine(const QString &prompt) {
    output_ << prompt;
    output_.flush();

    return input_.readLine().trimmed();
}

} // namespace android_files_backup