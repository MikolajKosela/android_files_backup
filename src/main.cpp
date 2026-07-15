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

    const android_files_backup::AdbClient adb;

    try {
        const QList<android_files_backup::AdbDevice> devices =
            adb.listDevices();

        if (devices.isEmpty()) {
            qInfo() << "Nie znaleziono żadnych urządzeń.";
            return 1;
        }

        for (qsizetype i = 0; i < devices.size(); ++i) {
            const auto& device = devices[i];

            qInfo().noquote()
                << QString("[%1] %2 | %3  %4")
                    .arg(i)
                    .arg(device.serial)
                    .arg(device.model)
                    .arg(
                        android_files_backup:: 
                            deviceStateToString(device.state)
                    );
        }
    } 
    catch (const std::exception& error) {
        qCritical() << error.what();
        return 1;
    }

    return 0;
}