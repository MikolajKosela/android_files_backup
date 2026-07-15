#include "android_files_backup/process_runner.h"

#include <QCoreApplication>
#include <QDebug>

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    const android_files_backup::ProcessResult result =
        android_files_backup::runProcess(
            "adb",
            {"devices", "-l"}
        );

    if (!result.success()) {
        qCritical().noquote()
            << "Błąd:"
            << result.standardError;

        return 1;
    }

    qInfo().noquote() << result.standardOutput;

    return 0;
}