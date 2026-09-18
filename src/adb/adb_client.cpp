#include "android_files_backup/adb/adb_client.h"

#include <qglobal.h>

#include <QProcess>
#include <expected>

namespace android_files_backup {

AdbClientResult AdbClient::runAdbProcess(const QString &program,
                                         const QStringList &arguments,
                                         int timeoutMs) {
  QProcess process;
  AdbClientResult result;

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

    result.standardError =
        QStringLiteral("Przekroczono limit czasu: ") + process.errorString();

    return result;
  }

  result.exitCode = process.exitCode();

  result.standardOutput = QString::fromUtf8(process.readAllStandardOutput());

  result.standardError = QString::fromUtf8(process.readAllStandardError());

  return result;
}

[[nodiscard]] std::expected<QStringList, QString> AdbClient::shell(
    const QString serial, const QStringList &arguments_, int timeoutMs) {
  QStringList arguments = {"-s", serial, "shell"};
  arguments.append(arguments_);

  const AdbClientResult result = runAdbProcess("adb", arguments, timeoutMs);
  if (result.success()) {
    return result.standardOutput.split('\n', Qt::SkipEmptyParts);
  } else {
    return std::unexpected(
        "Błąd przy wykonywaniu polecenia konsoli androida \nadb " +
        arguments.join("") + "\n" + result.standardError);
  }
}

[[nodiscard]] std::expected<QString, QString> AdbClient::getState(
    const QString serial, const QStringList &arguments_, int timeoutMs) {
  QStringList arguments = {"-s", serial, "get-state"};
  arguments.append(arguments_);

  const AdbClientResult result = runAdbProcess("adb", arguments, timeoutMs);

  if (result.success()) {
    return result.standardOutput.trimmed();
  } else {
    return std::unexpected("Błąd przy pobieraniu stanu urządzenia \nadb " +
                           arguments.join("") + "\n" + result.standardError);
  }
}

[[nodiscard]] std::expected<QStringList, QString> AdbClient::devices(
    const QStringList &arguments_, int timeoutMs) {
  QStringList arguments = {"devices"};
  arguments.append(arguments_);

  const AdbClientResult result = runAdbProcess("adb", arguments, timeoutMs);
  if (result.success()) {
    return result.standardOutput.split('\n', Qt::SkipEmptyParts);
  } else {
    return std::unexpected("Błąd przy listowaniu urządzeń\nadb " +
                           arguments.join("") + "\n" + result.standardError);
  }
}

[[nodiscard]] std::expected<void, QString> AdbClient::pull(
    const QString serial, const QStringList &arguments_, int timeoutMs) {
  QStringList arguments = {"-s", serial, "pull"};
  arguments.append(arguments_);

  const AdbClientResult result = runAdbProcess("adb", arguments, timeoutMs);

  if (result.success()) {
    return {};
  } else {
    return std::unexpected("Błąd przy przesyłaniu plików na komputer\nadb " +
                           arguments.join("") + "\n" + result.standardError);
  }
}

}  // namespace android_files_backup