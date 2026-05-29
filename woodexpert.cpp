#include "woodexpert.h"

QMap<QString, WoodExpert::WoodProps> WoodExpert::knowledgeBase;

void WoodExpert::initKnowledge() {
    if (!knowledgeBase.isEmpty()) return;

    knowledgeBase["MDF (M\u00e9dium)"] = {
        "Panneau composite de fibres de bois et de liant synth\u00e9tique.", 
        4, 2, 
        {"Meubles int\u00e9rieurs", "Placards", "D\u00e9coration"},
        "Tr\u00e8s facile \u00e0 peindre. \u00c0 \u00e9viter absolument dans les zones humides."
    };

    knowledgeBase["Contreplaqu\u00e9"] = {
        "Panneau form\u00e9 de fines couches de bois crois\u00e9es et coll\u00e9es.", 
        8, 6, 
        {"Structures", "Am\u00e9nagement robuste", "Fonds de meubles"},
        "Tr\u00e8s r\u00e9sistant mécaniquement. Id\u00e9al pour les structures portantes."
    };

    knowledgeBase["Agglom\u00e9r\u00e9"] = {
        "Particules de bois press\u00e9es avec de la r\u00e9sine.", 
        3, 1, 
        {"Am\u00e9nagement provisoire", "Rayonnages \u00e9conomiques"},
        "Solution la plus \u00e9conomique, mais tr\u00e8s fragile aux chocs et \u00e0 l'eau."
    };

    knowledgeBase["M\u00e9lamin\u00e9 Blanc"] = {
        "Agglom\u00e9r\u00e9 recouvert d'une feuille d\u00e9corative impr\u00e9gn\u00e9e de r\u00e9sine.", 
        4, 5, 
        {"Cuisines", "Salles de bain", "Rayonnages"},
        "Surface r\u00e9sistante et facile \u00e0 nettoyer. Ne n\u00e9cessite pas de peinture."
    };

    knowledgeBase["Stratifi\u00e9"] = {
        "Couches de papier kraft r\u00e9sin\u00e9 sous haute pression.", 
        9, 8, 
        {"Plans de travail", "Sols", "Surfaces d'usage intensif"},
        "Extr\u00eamement dur et r\u00e9sistant aux rayures. Parfait pour les cuisines."
    };

    knowledgeBase["Ch\u00eane Plaqu\u00e9"] = {
        "Support (MDF/Agglo) avec une fine feuille de ch\u00eane v\u00e9ritable.", 
        6, 3, 
        {"Meubles haut de gamme", "Panneautage d\u00e9coratif"},
        "Combine l'esth\u00e9tique du bois noble avec la stabilit\u00e9 du panneau."
    };

    knowledgeBase["Panneau Massif"] = {
        "Pi\u00e8ces de bois massif assembl\u00e9es pour former un panneau.", 
        10, 4, 
        {"Tables", "Escaliers", "Menuiserie traditionnelle"},
        "Le plus prestigieux. N\u00e9cessite un traitement (vernis/huile) et travaille avec le temps."
    };
}

WoodExpert::WoodProps WoodExpert::getInfo(const QString &woodName) {
    initKnowledge();
    for (auto it = knowledgeBase.begin(); it != knowledgeBase.end(); ++it) {
        if (woodName.contains(it.key(), Qt::CaseInsensitive)) return it.value();
    }
    return {"Type de bois inconnu.", 5, 5, {"Usage g\u00e9n\u00e9ral"}, "Consulter un sp\u00e9cialiste."};
}

QString WoodExpert::getAdvice(const QString &woodName) {
    WoodProps props = getInfo(woodName);
    QString advice = "--- ANALYSE DE L'EXPERT AI ---\n";
    advice += "Propri\u00e9t\u00e9s : " + props.description + "\n";
    advice += "R\u00e9sistence M\u00e9canique : " + QString::number(props.resistanceMecanique) + "/10\n";
    advice += "R\u00e9sistence Humidit\u00e9 : " + QString::number(props.resistanceHumidite) + "/10\n";
    advice += "Usages recommand\u00e9s : " + props.usageIdeal.join(", ") + ".\n";
    advice += "Conseil Pro : " + props.conseilPro;
    return advice;
}

QStringList WoodExpert::getAllWoodNames() {
    initKnowledge();
    return knowledgeBase.keys();
}
