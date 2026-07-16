#pragma once

#include <QRegularExpression>
#include <QString>

namespace android_files_backup {

QRegularExpression fromWildCardToRegularExpression(QString wildCard);
void newDirectory(QString target);

} // namespace android_files_backup