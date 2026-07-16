#include "android_files_backup/application/application_controller.h"

#include <QCoreApplication>
#include <QDebug>
#include <qglobal.h>
#include <qlist.h>

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    android_files_backup::ApplicationController controller;

    controller.refreshDevices();

    for (const auto &device : controller.devices()) {
        qInfo().noquote() << device.serial << device.model
                          << deviceStateToString(device.state);
    }

    controller.selectDevice();

    controller.createFilesPull_debugFunction("/sdcard/DCIM/Screenshots",
                                             "build/test/ang",
                                             "*Diki sownik angielskiego*");
    controller.createFilesPull_debugFunction("/sdcard/DCIM/Screenshots",
                                             "build/test/niem",
                                             "*Diki sownik niemieckiego*");

    return 0;
}