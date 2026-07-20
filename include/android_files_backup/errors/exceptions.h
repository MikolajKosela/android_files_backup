#pragma once

#include <QString>

#include <stdexcept>

namespace android_files_backup {

class ApplicationException : public std::runtime_error {
  public:
    explicit ApplicationException(const QString &message)
        : std::runtime_error(message.toStdString()) {}
    using std::runtime_error::runtime_error;
};

class AdbException : public ApplicationException {
  public:
    using ApplicationException::ApplicationException;
};

class BackupException : public ApplicationException {
  public:
    using ApplicationException::ApplicationException;
};

} // namespace android_files_backup