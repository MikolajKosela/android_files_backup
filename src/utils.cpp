#include "android_files_backup/utils.h"

#include <QString>
#include <QRegularExpression>
#include <QDebug>
#include <QDir>

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

void newDirectory(QString target) {
    QDir dir(target);

    if (!dir.exists()) {
        if (QDir().mkpath(target)) {
            qInfo() << "Utworzono nowy folder " << target;
        } else {
            qInfo() << "Nie udało się utworzyć folderu " << target;
        }
    } else {
        qInfo() << "Muszę usunąć zawartość starego folderu";

        for (const auto &file : dir.entryList(QDir::Files)) {
            //qInfo() << "Usuwam: " << file;
            dir.remove(file);
        }

    }
}

}