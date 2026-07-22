#include "android_files_backup/utils/utils.h"

#include <QDebug>
#include <QDir>
#include <QRegularExpression>
#include <QString>

namespace android_files_backup {

QRegularExpression fromWildCardToRegularExpression(QString wildCard) {
    QString regexText =
        QRegularExpression::wildcardToRegularExpression(wildCard);

    QRegularExpression pattern(regexText,
                               QRegularExpression::CaseInsensitiveOption);

    return pattern;
}

void newDirectory(QString target) {
    QDir dir(target);

    if (!dir.exists()) {
        QDir().mkpath(target);
    } else {
        for (const auto &file : dir.entryList(QDir::Files)) {
            dir.remove(file);
        }
    }
}

} // namespace android_files_backup