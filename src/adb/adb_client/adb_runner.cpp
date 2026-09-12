#include <qdebug.h>
#include <qfileinfo.h>
#include <qglobal.h>
#include <qnamespace.h>

#include <QFileInfo>
#include <QRegularExpression>
#include <QString>
#include <QStringList>
#include <expected>

#include "android_files_backup/adb/adb_client.h"
#include "android_files_backup/adb/adb_device.h"
#include "android_files_backup/process/process_runner.h"
#include "android_files_backup/result/result.h"

namespace android_files_backup {

std::expected<QStringList, QString> AdbClient::runForDevice(
    const AdbDevice &device, const QStringList &arguments) const {
  QStringList argumentsWithDevice = {"-s", device.serial};
  argumentsWithDevice.append(arguments);

  QString commend = "adb";
  for (const auto &arg : arguments) {
    commend += " " + arg;
  }
  /*
  qInfo() << commend;
  */

  const ProcessResult processResult = runProcess("adb", argumentsWithDevice);

  if (!processResult.success()) {
    return std::unexpected(
        QString("Wystąpił błąd podczas wykonywania komendy\n "
                "%1 \n dla urządzenia %2 \nLogi: %3 %4 \n")
            .arg(commend)
            .arg(QString(device.serial))
            .arg(processResult.standardOutput)
            .arg(processResult.standardError));
  }
  return processResult.standardOutput.split('\n', Qt::SkipEmptyParts);
}

}  // namespace android_files_backup