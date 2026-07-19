#pragma once

#include "android_files_backup/application/application_controller.h"

#include <QTextStream>

namespace android_files_backup {

class CliApplication {
  public:
    explicit CliApplication(ApplicationController &controller);
    int run();

  private:
    void showDevices();
    void choiceDevice();
    void selectDevice(int choice);

    void createFilesPull_functionForTesting(QString remote, QString target,
                                            QString condition);

    [[nodiscard]] int readInteger(const QString &prompt, int minimum,
                                  int maximum);

    [[nodiscard]] QString readLine(const QString &prompt);

    void printError(const QString &message);

    ApplicationController &controller_;

    QTextStream input_;
    QTextStream output_;
    QTextStream error_;
};

} // namespace android_files_backup