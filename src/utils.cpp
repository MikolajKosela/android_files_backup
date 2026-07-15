#include "android_files_backup/utils.h"

#include <QString>
#include <QRegularExpression>

namespace android_files_backup {

QRegularExpression fromWildCardToRegularExpression(QString wildCard) {
    QString regexText =
        QRegularExpression::wildcardToRegularExpression(wildCard);

    QRegularExpression pattern(
        regexText,
        QRegularExpression::CaseInsensitiveOption
    );

    return pattern;
}

}