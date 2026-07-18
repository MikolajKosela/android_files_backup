#include "android_files_backup/application/application_controller.h"
#include "android_files_backup/ui/cli_application.h"

#include <QCoreApplication>

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    android_files_backup::ApplicationController controller;
    android_files_backup::CliApplication cli(controller);

    cli.run();
    return 0;
}