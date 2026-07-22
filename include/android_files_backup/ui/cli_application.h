#pragma once

#include "android_files_backup/application/application_controller.h"

#include <QTextStream>

namespace android_files_backup {

class CliApplication {
  public:
    explicit CliApplication(ApplicationController &controller);
    int run();

  private:
    void clearScreen();

    void showDevices();
    void choiceDevice();
    void selectDevice(int choice);

    void createFilesPull_functionForTesting(QString remote, QString target,
                                            QString condition);
    void createCustomFilesPull_functionForTesting();

    [[nodiscard]] QString chooseLocalDirectory();
    [[nodiscard]] QString chooseRemoteDirectory();
    [[nodiscard]] QString choosePattern();

    [[nodiscard]] int readInteger(const QString &prompt, int minimum,
                                  int maximum);

    [[nodiscard]] QString readLine(const QString &prompt);

    void printError(const QString &message);

    ApplicationController &controller_;

    QString displayDespiteCleaning;

    QTextStream input_;
    QTextStream output_;
    QTextStream error_;
};

} // namespace android_files_backup