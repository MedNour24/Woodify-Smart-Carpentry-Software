#ifndef WOODEXPERT_H
#define WOODEXPERT_H

#include <QString>
#include <QMap>
#include <QStringList>

class WoodExpert {
public:
    struct WoodProps {
        QString description;
        int resistanceMecanique; // 1-10
        int resistanceHumidite;  // 1-10
        QStringList usageIdeal;
        QString conseilPro;
    };

    static WoodProps getInfo(const QString &woodName);
    static QString getAdvice(const QString &woodName);
    static QStringList getAllWoodNames();

private:
    static QMap<QString, WoodProps> knowledgeBase;
    static void initKnowledge();
};

#endif // WOODEXPERT_H
