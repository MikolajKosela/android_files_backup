#include "android_files_backup/adb_client.h"
#include "android_files_backup/adb_device.h"

#include <QCoreApplication>
#include <QDebug>
#include <exception>
#include <qglobal.h>
#include <qlist.h>

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    android_files_backup::AdbClient adb;

    adb.chooseDevice();
    adb.pullFiles("/sdcard/DCIM/Screenshots", "build/test/ang", "*Diki sownik angielskiego*");
    adb.pullFiles("/sdcard/DCIM/Screenshots", "build/test/niem", "*Diki sownik niemieckiego*");

    return 0;
}