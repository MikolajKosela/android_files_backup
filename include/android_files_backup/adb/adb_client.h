#pragma once

#include <qdebug.h>
#include <qfileinfo.h>
#include <qglobal.h>
#include <qnamespace.h>

#include <QFileInfo>
#include <QRegularExpression>
#include <QString>
#include <QStringList>
#include <expected>

namespace android_files_backup {

struct AdbClientResult {
  bool started = false;
  bool finished = false;
  int exitCode = -1;

  QString standardOutput;
  QString standardError;

  [[nodiscard]] bool success() const {
    return started && finished && exitCode == 0;
  }
};

class AdbClient {
 public:
  [[nodiscard]] std::expected<QStringList, QString> shell(
      const QString serial, const QStringList &arguments_,
      int timeoutMs = 30'000);

  [[nodiscard]] std::expected<QString, QString> getState(
      const QString serial, const QStringList &arguments_,
      int timeoutMs = 30'000);

  [[nodiscard]] std::expected<void, QString> pull(const QString serial,
                                                  const QStringList &arguments_,
                                                  int timeoutMs = 30'000);

  [[nodiscard]] std::expected<QStringList, QString> devices(
      const QStringList &arguments_, int timeoutMs = 30'000);

 private:
  [[nodiscard]] AdbClientResult runAdbProcess(const QString &program,
                                              const QStringList &arguments_,
                                              int timeoutMs = 30'000);
};

}  // namespace android_files_backup