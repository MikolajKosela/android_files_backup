#pragma once 

#include <QString>
#include <QRegularExpression>

namespace android_files_backup {

QRegularExpression fromWildCardToRegularExpression(QString wildCard);
void newDirectory(QString target);

}