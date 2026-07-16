#include "android_files_backup/process/process_runner.h"

#include <QProcess>
#include <QStringLiteral>

namespace android_files_backup {

bool ProcessResult::success() const {
    return started && finished && exitCode == 0;
}

ProcessResult runProcess(const QString &program, const QStringList &arguments,
                         int timeoutMs) {
    QProcess process;
    ProcessResult result;

    process.start(program, arguments);

    result.started = process.waitForStarted();

    if (!result.started) {
        result.standardError = process.errorString();
        return result;
    }

    result.finished = process.waitForFinished(timeoutMs);

    if (!result.finished) {
        process.kill();
        process.waitForFinished();

        result.standardError = QStringLiteral("Przekroczono limit czasu: ") +
                               process.errorString();

        return result;
    }

    result.exitCode = process.exitCode();

    result.standardOutput = QString::fromUtf8(process.readAllStandardOutput());

    result.standardError = QString::fromUtf8(process.readAllStandardError());

    return result;
}

} // namespace android_files_backup