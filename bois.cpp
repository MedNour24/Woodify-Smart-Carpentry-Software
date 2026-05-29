#include "bois.h"

#include <QDebug>
#include <QRegularExpression>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QRandomGenerator>
#include <QFileInfo>
#include <QDir>

#include <QImage>
#include <QColor>
#include <QPainter>
#include <QPen>
#include <QtMath>

namespace {
struct DetectedKnotRow {
    double x = 0.0;
    double y = 0.0;
    double diameter = 0.0;
    double confidence = 0.0;
    QString type;
    QString severity;
};

QList<DetectedKnotRow> g_lastDetectedKnots;
QString g_lastDetectedImagePath;
constexpr double kReferenceSurfaceM2 = 0.04;

bool isOracleObjectPresent(const QString &objectType, const QString &objectName)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM USER_OBJECTS WHERE OBJECT_TYPE = :objectType AND OBJECT_NAME = :objectName");
    query.bindValue(":objectType", objectType.trimmed().toUpper());
    query.bindValue(":objectName", objectName.trimmed().toUpper());

    if (!query.exec() || !query.next()) {
        return false;
    }

    return query.value(0).toInt() > 0;
}

bool ensureBoisNoeudsStorage(QString *errorMessage = nullptr)
{
    if (errorMessage) {
        errorMessage->clear();
    }

    if (isOracleObjectPresent("TABLE", "BOIS_NOEUDS")) {
        return true;
    }

    QSqlQuery query;
    if (!query.exec(
            "CREATE TABLE BOIS_NOEUDS ("
            "    id_noeud         NUMBER PRIMARY KEY,"
            "    idBois           NUMBER NOT NULL,"
            "    numero_noeud     NUMBER NOT NULL,"
            "    position_x       NUMBER(10,3),"
            "    position_y       NUMBER(10,3),"
            "    diametre_mm      NUMBER(10,2),"
            "    type_noeud       VARCHAR2(30),"
            "    confiance        NUMBER(3,1),"
            "    severite         VARCHAR2(20),"
            "    date_detection   TIMESTAMP DEFAULT SYSTIMESTAMP,"
            "    image_resultat   VARCHAR2(255),"
            "    notes            VARCHAR2(500),"
            "    CONSTRAINT fk_noeud_bois FOREIGN KEY (idBois) REFERENCES BOIS(idBois) ON DELETE CASCADE,"
            "    CONSTRAINT chk_confiance CHECK (confiance >= 0 AND confiance <= 1),"
            "    CONSTRAINT chk_type_noeud CHECK (type_noeud IN ('Principal', 'Secondaire', 'Gerçure')),"
            "    CONSTRAINT chk_severite CHECK (severite IN ('Faible', 'Moyen', 'Élevé'))"
            ")")) {
        const QString errorText = query.lastError().text();
        if (!errorText.contains("ORA-00955")) {
            if (errorMessage) {
                *errorMessage = errorText;
            }
            return false;
        }
    }

    if (!query.exec("CREATE SEQUENCE bois_noeud_seq START WITH 1 INCREMENT BY 1 NOCACHE NOCYCLE")) {
        const QString errorText = query.lastError().text();
        if (!errorText.contains("ORA-00955")) {
            if (errorMessage) {
                *errorMessage = errorText;
            }
            return false;
        }
    }

    query.exec("CREATE INDEX idx_bois_noeuds_bois ON BOIS_NOEUDS(idBois)");
    query.exec("CREATE INDEX idx_bois_noeuds_date ON BOIS_NOEUDS(date_detection)");
    return true;
}

int getHistoricalNodeSamples(int idBois)
{
    if (idBois <= 0 || !isOracleObjectPresent("TABLE", "BOIS_NOEUDS")) {
        return 0;
    }

    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM BOIS_NOEUDS WHERE idBois = :idBois");
    query.bindValue(":idBois", idBois);

    if (!query.exec() || !query.next()) {
        return 0;
    }

    return query.value(0).toInt();
}

double getHistoricalAverageNodeDiameter(int idBois)
{
    if (idBois <= 0 || !isOracleObjectPresent("TABLE", "BOIS_NOEUDS")) {
        return 0.0;
    }

    QSqlQuery query;
    query.prepare("SELECT NVL(AVG(diametre_mm), 0) FROM BOIS_NOEUDS WHERE idBois = :idBois");
    query.bindValue(":idBois", idBois);

    if (!query.exec() || !query.next()) {
        return 0.0;
    }

    return query.value(0).toDouble();
}

bool boisColumnExists(const QString &columnName, QString *errorMessage = nullptr)
{
    QSqlQuery query;
    query.prepare(
        "SELECT COUNT(*) "
        "FROM USER_TAB_COLUMNS "
        "WHERE TABLE_NAME = 'BOIS' AND COLUMN_NAME = :columnName");
    query.bindValue(":columnName", columnName.trimmed().toUpper());

    if (!query.exec() || !query.next()) {
        if (errorMessage) {
            *errorMessage = query.lastError().text();
        }
        return false;
    }

    return query.value(0).toInt() > 0;
}

bool ensureBoisSchema(QString *errorMessage = nullptr)
{
    if (errorMessage) {
        errorMessage->clear();
    }

    QString columnError;
    const bool hasEpaisseur = boisColumnExists("EPAISSEUR", &columnError);
    if (!columnError.isEmpty()) {
        if (errorMessage) {
            *errorMessage = columnError;
        }
        return false;
    }

    if (!hasEpaisseur) {
        QSqlQuery addColumnQuery;
        if (!addColumnQuery.exec("ALTER TABLE BOIS ADD (EPAISSEUR NUMBER DEFAULT 18)")) {
            if (errorMessage) {
                *errorMessage = addColumnQuery.lastError().text();
            }
            return false;
        }
    }

    QSqlQuery normalizeQuery;
    if (!normalizeQuery.exec("UPDATE BOIS SET EPAISSEUR = 18 WHERE EPAISSEUR IS NULL")) {
        if (errorMessage) {
            *errorMessage = normalizeQuery.lastError().text();
        }
        return false;
    }

    return true;
}
}

Bois::Bois()
    : idBois(0),
    longueur(0.0),
    largeur(0.0),
    epaisseur(0.0),
    quantite(0),
    prixUnitaire(0.0),
    humiditeMax(0.0),
    nombreNoeuds(0)
{}

Bois::Bois(int idBois, const QString &nomBois, double longueur, double largeur, double epaisseur,
           int quantite, double prixUnitaire, const QString &fournisseur,
           const QString &resistance, double humiditeMax,
           const QString &usageRecommande, const QString &imageStock,
           const QString &imageAnalyse, int nombreNoeuds,
           const QString &classeQualite, const QDate &dateAnalyse)
    : idBois(idBois),
    nomBois(nomBois),
    longueur(longueur),
    largeur(largeur),
    epaisseur(epaisseur),
    quantite(quantite),
    prixUnitaire(prixUnitaire),
    fournisseur(fournisseur),
    resistance(resistance),
    humiditeMax(humiditeMax),
    usageRecommande(usageRecommande),
    imageStock(imageStock),
    imageAnalyse(imageAnalyse),
    nombreNoeuds(nombreNoeuds),
    classeQualite(classeQualite),
    dateAnalyse(dateAnalyse)
{}

int      Bois::getIdBois()          const { return idBois; }
QString  Bois::getNomBois()         const { return nomBois; }
double   Bois::getLongueur()        const { return longueur; }
double   Bois::getLargeur()         const { return largeur; }
double   Bois::getEpaisseur()       const { return epaisseur; }
int      Bois::getQuantite()        const { return quantite; }
double   Bois::getPrixUnitaire()    const { return prixUnitaire; }
QString  Bois::getFournisseur()     const { return fournisseur; }
QString  Bois::getResistance()      const { return resistance; }
double   Bois::getHumiditeMax()     const { return humiditeMax; }
QString  Bois::getUsageRecommande() const { return usageRecommande; }
QString  Bois::getImageStock()      const { return imageStock; }
QString  Bois::getImageAnalyse()    const { return imageAnalyse; }
int      Bois::getNombreNoeuds()    const { return nombreNoeuds; }
QString  Bois::getClasseQualite()   const { return classeQualite; }
QDate    Bois::getDateAnalyse()     const { return dateAnalyse; }
QString  Bois::getLastError()       const { return lastErrMsg; }

bool Bois::ensureStorage(QString *errorMessage)
{
    return ensureBoisSchema(errorMessage);
}

void Bois::setIdBois(int id)                   { idBois = id; }
void Bois::setNomBois(const QString &nom)      { nomBois = nom; }
void Bois::setLongueur(double l)               { longueur = l; }
void Bois::setLargeur(double l)                { largeur = l; }
void Bois::setEpaisseur(double e)              { epaisseur = e; }
void Bois::setQuantite(int q)                  { quantite = q; }
void Bois::setPrixUnitaire(double p)           { prixUnitaire = p; }
void Bois::setFournisseur(const QString &f)    { fournisseur = f; }
void Bois::setResistance(const QString &r)     { resistance = r; }
void Bois::setHumiditeMax(double h)            { humiditeMax = h; }
void Bois::setUsageRecommande(const QString &u){ usageRecommande = u; }
void Bois::setImageStock(const QString &img)   { imageStock = img; }
void Bois::setImageAnalyse(const QString &img) { imageAnalyse = img; }
void Bois::setNombreNoeuds(int n)              { nombreNoeuds = n; }
void Bois::setClasseQualite(const QString &c)  { classeQualite = c; }
void Bois::setDateAnalyse(const QDate &d)      { dateAnalyse = d; }

bool Bois::validerNomBois(const QString &nom)
{
    return !nom.trimmed().isEmpty() && nom.trimmed().length() <= 50;
}

bool Bois::validerLongueur(double l)
{
    return l > 0;
}

bool Bois::validerLargeur(double l)
{
    return l > 0;
}

bool Bois::validerEpaisseur(double e)
{
    return e > 0;
}

bool Bois::validerQuantite(int q)
{
    return q >= 0;
}

bool Bois::validerPrixUnitaire(double p)
{
    return p >= 0;
}

bool Bois::validerFournisseur(const QString &f)
{
    return !f.trimmed().isEmpty() && f.trimmed().length() <= 100;
}

bool Bois::validerResistance(const QString &r)
{
    return r.trimmed().isEmpty() || r.trimmed().length() <= 100;
}

bool Bois::validerHumidite(double h)
{
    return h >= 0 && h <= 100;
}

bool Bois::validerUsage(const QString &u)
{
    return u.trimmed().isEmpty() || u.trimmed().length() <= 200;
}

// ============================================// ============================================================
//  CRUD � Afficher (READ)
// ============================================================
//  CRUD — Ajouter (CREATE)
// ============================================// ============================================================
//  CRUD � Afficher (READ)
// ============================================================
bool Bois::ajouter()
{
    lastErrMsg.clear();
    if (!ensureStorage(&lastErrMsg)) {
        return false;
    }

    // ------ Contrôle de saisie ------
    if (!validerNomBois(nomBois)) { lastErrMsg = "Nom du bois invalide (1-50 caractères)."; return false; }
    if (!validerLongueur(longueur)) { lastErrMsg = "Longueur invalide (doit être > 0)."; return false; }
    if (!validerLargeur(largeur)) { lastErrMsg = "Largeur invalide (doit être > 0)."; return false; }
    if (!validerEpaisseur(epaisseur)) { lastErrMsg = "Épaisseur invalide (doit être > 0)."; return false; }
    if (!validerQuantite(quantite)) { lastErrMsg = "Quantité invalide (doit être >= 0)."; return false; }
    if (!validerPrixUnitaire(prixUnitaire)) { lastErrMsg = "Prix unitaire invalide (doit être >= 0)."; return false; }
    if (!validerFournisseur(fournisseur)) { lastErrMsg = "Fournisseur invalide."; return false; }
    // Note: Resistance, Humidity, and Usage are now optional

    // ------ Auto-generate ID via Oracle SEQUENCE ------
    // Ensure the sequence exists (ORA-00955 = already exists, safely ignored)
    {
        QSqlQuery ensureSeq;
        ensureSeq.exec(
            "DECLARE v_max NUMBER; "
            "BEGIN "
            "  SELECT NVL(MAX(IDBOIS), 0) + 1 INTO v_max FROM BOIS; "
            "  EXECUTE IMMEDIATE 'CREATE SEQUENCE BOIS_SEQ START WITH ' || v_max; "
            "EXCEPTION WHEN OTHERS THEN "
            "  IF SQLCODE != -955 THEN RAISE; END IF; "
            "END;");
    }
    QSqlQuery queryId;
    queryId.prepare("SELECT BOIS_SEQ.NEXTVAL FROM DUAL");
    if (!queryId.exec() || !queryId.next()) {
        lastErrMsg = "Impossible de générer l'ID via la séquence BOIS_SEQ.";
        return false;
    }
    int autoId = queryId.value(0).toInt();

    // ------ Requête SQL ------
    QSqlQuery query;
    query.prepare(
        "INSERT INTO BOIS "
        "(IDBOIS, NOMBOIS, LONGUEUR, LARGEUR, EPAISSEUR, QUANTITE, PRIX_UNITAIRE, "
        " FOURNISSEUR, RESISTANCE, HUMIDITE_MAX, USAGE_RECOMMANDE, "
        " IMAGE_STOCK, IMAGE_ANALYSE, NOMBRE_NOEUDS, CLASSE_QUALITE, DATE_ANALYSE) "
        "VALUES "
        "(:idBois, :nomBois, :longueur, :largeur, :epaisseur, :quantite, :prixUnitaire, "
        " :fournisseur, :resistance, :humiditeMax, :usageRecommande, "
        " :imageStock, :imageAnalyse, :nombreNoeuds, :classeQualite, :dateAnalyse)"
        );

    query.bindValue(":idBois",          autoId);
    query.bindValue(":nomBois",         nomBois);
    query.bindValue(":longueur",        longueur);
    query.bindValue(":largeur",         largeur);
    query.bindValue(":epaisseur",       epaisseur);
    query.bindValue(":quantite",        quantite);
    query.bindValue(":prixUnitaire",    prixUnitaire);
    query.bindValue(":fournisseur",     fournisseur);
    query.bindValue(":resistance",      resistance);
    query.bindValue(":humiditeMax",     humiditeMax);
    query.bindValue(":usageRecommande", usageRecommande);
    query.bindValue(":imageStock",      imageStock);
    query.bindValue(":imageAnalyse",    imageAnalyse);
    query.bindValue(":nombreNoeuds",    nombreNoeuds);
    query.bindValue(":classeQualite",   classeQualite);
    query.bindValue(":dateAnalyse",     dateAnalyse);

    if (!query.exec()) {
        lastErrMsg = query.lastError().text();
        qDebug() << "Erreur Ajouter Bois:" << lastErrMsg;
        return false;
    }

    qDebug() << "Bois ajouté avec succès (id:" << autoId << ")";
    return true;
}
// ============================================================
//  CRUD � Afficher (READ)
// ============================================================

QSqlQueryModel* Bois::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QString errorMessage;
    if (!ensureStorage(&errorMessage)) {
        qDebug() << "Erreur schema Bois:" << errorMessage;
        return model;
    }

    model->setQuery(
        "SELECT IDBOIS, NOMBOIS, LONGUEUR, LARGEUR, EPAISSEUR, QUANTITE, PRIX_UNITAIRE, "
        "FOURNISSEUR, RESISTANCE, HUMIDITE_MAX, USAGE_RECOMMANDE, "
        "NOMBRE_NOEUDS, CLASSE_QUALITE, "
        "TO_CHAR(DATE_ANALYSE, 'DD/MM/YYYY') AS DATE_ANALYSE "
        "FROM BOIS "
        "ORDER BY IDBOIS"
        );

    if (model->lastError().isValid()) {
        qDebug() << "Erreur Afficher Bois:" << model->lastError().text();
    }

    model->setHeaderData(0,  Qt::Horizontal, "ID");
    model->setHeaderData(1,  Qt::Horizontal, "Nom");
    model->setHeaderData(2,  Qt::Horizontal, "Longueur");
    model->setHeaderData(3,  Qt::Horizontal, "Largeur");
    model->setHeaderData(4,  Qt::Horizontal, " \u00c9paisseur");
    model->setHeaderData(5,  Qt::Horizontal, "Quantit\u00e9");
    model->setHeaderData(6,  Qt::Horizontal, "Prix Unitaire");
    model->setHeaderData(7,  Qt::Horizontal, "Fournisseur");
    model->setHeaderData(8,  Qt::Horizontal, "R\u00e9sistance");
    model->setHeaderData(9,  Qt::Horizontal, "Humidit\u00e9 Max");
    model->setHeaderData(10, Qt::Horizontal, "Usage Recommand\u00e9");
    model->setHeaderData(11, Qt::Horizontal, "Nbre N\u0153uds");
    model->setHeaderData(12, Qt::Horizontal, "Classe Qualit\u00e9");
    model->setHeaderData(13, Qt::Horizontal, "Date Analyse");

    return model;
}

// ============================================// ============================================================
//  CRUD � Afficher (READ)
// ============================================================
//  CRUD — Supprimer (DELETE)
// ============================================// ============================================================
//  CRUD � Afficher (READ)
// ============================================================

bool Bois::supprimer(int idBois)
{
    lastErrMsg.clear();
    if (!ensureStorage(&lastErrMsg)) {
        return false;
    }

    if (idBois <= 0) {
        lastErrMsg = "ID Bois invalide.";
        return false;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM BOIS WHERE IDBOIS = :id");
    query.bindValue(":id", idBois);

    if (!query.exec()) {
        lastErrMsg = query.lastError().text();
        qDebug() << "Erreur Supprimer Bois:" << lastErrMsg;
        return false;
    }

    if (query.numRowsAffected() == 0) {
        lastErrMsg = "Aucun bois trouvé avec cet ID.";
        return false;
    }

    qDebug() << "Bois supprimé avec succès (id:" << idBois << ")";
    return true;
}

// ============================================// ============================================================
//  CRUD � Afficher (READ)
// ============================================================
//  CRUD — Modifier (UPDATE)
// ============================================// ============================================================
//  CRUD � Afficher (READ)
// ============================================================

bool Bois::modifier(int idBois)
{
    lastErrMsg.clear();
    if (!ensureStorage(&lastErrMsg)) {
        return false;
    }

    if (idBois <= 0) {
        lastErrMsg = "ID Bois invalide.";
        return false;
    }

    // ------ Contrôle de saisie ------
    if (!validerNomBois(nomBois)) { lastErrMsg = "Nom du bois invalide (1-50 caractères)."; return false; }
    if (!validerLongueur(longueur)) { lastErrMsg = "Longueur invalide (doit être > 0)."; return false; }
    if (!validerLargeur(largeur)) { lastErrMsg = "Largeur invalide (doit être > 0)."; return false; }
    if (!validerEpaisseur(epaisseur)) { lastErrMsg = "Épaisseur invalide (doit être > 0)."; return false; }
    if (!validerQuantite(quantite)) { lastErrMsg = "Quantité invalide (doit être >= 0)."; return false; }
    if (!validerPrixUnitaire(prixUnitaire)) { lastErrMsg = "Prix unitaire invalide (doit être >= 0)."; return false; }
    if (!validerFournisseur(fournisseur)) { lastErrMsg = "Fournisseur invalide."; return false; }
    // Note: Resistance, Humidity, and Usage are now optional

    // ------ Requête SQL ------
    QSqlQuery query;
    query.prepare(
        "UPDATE BOIS SET "
        "NOMBOIS = :nomBois, "
        "LONGUEUR = :longueur, "
        "LARGEUR = :largeur, "
        "EPAISSEUR = :epaisseur, "
        "QUANTITE = :quantite, "
        "PRIX_UNITAIRE = :prixUnitaire, "
        "FOURNISSEUR = :fournisseur, "
        "RESISTANCE = :resistance, "
        "HUMIDITE_MAX = :humiditeMax, "
        "USAGE_RECOMMANDE = :usageRecommande, "
        "IMAGE_STOCK = :imageStock, "
        "IMAGE_ANALYSE = :imageAnalyse, "
        "NOMBRE_NOEUDS = :nombreNoeuds, "
        "CLASSE_QUALITE = :classeQualite, "
        "DATE_ANALYSE = :dateAnalyse "
        "WHERE IDBOIS = :idBois"
        );

    query.bindValue(":nomBois",         nomBois);
    query.bindValue(":longueur",        longueur);
    query.bindValue(":largeur",         largeur);
    query.bindValue(":epaisseur",       epaisseur);
    query.bindValue(":quantite",        quantite);
    query.bindValue(":prixUnitaire",    prixUnitaire);
    query.bindValue(":fournisseur",     fournisseur);
    query.bindValue(":resistance",      resistance);
    query.bindValue(":humiditeMax",     humiditeMax);
    query.bindValue(":usageRecommande", usageRecommande);
    query.bindValue(":imageStock",      imageStock);
    query.bindValue(":imageAnalyse",    imageAnalyse);
    query.bindValue(":nombreNoeuds",    nombreNoeuds);
    query.bindValue(":classeQualite",   classeQualite);
    query.bindValue(":dateAnalyse",     dateAnalyse);
    query.bindValue(":idBois",          idBois);

    if (!query.exec()) {
        lastErrMsg = query.lastError().text();
        qDebug() << "Erreur Modifier Bois:" << lastErrMsg;
        return false;
    }

    if (query.numRowsAffected() == 0) {
        lastErrMsg = "Aucun bois trouvé avec cet ID.";
        return false;
    }

    qDebug() << "Bois modifié avec succès (id:" << idBois << ")";
    return true;
}

// ============================================// ============================================================
//  CRUD � Afficher (READ)
// ============================================================
//  Advanced Features — Filtrer
// ============================================// ============================================================
//  CRUD � Afficher (READ)
// ============================================================
QSqlQueryModel* Bois::filtrer(const QString &nom, double minPrix, double maxPrix, const QString &fournisseur)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QString errorMessage;
    if (!ensureStorage(&errorMessage)) {
        qDebug() << "Erreur schema Bois:" << errorMessage;
        return model;
    }

    QSqlQuery query;
    QString baseStr = "SELECT IDBOIS, NOMBOIS, LONGUEUR, LARGEUR, QUANTITE, PRIX_UNITAIRE, "
                      "FOURNISSEUR, RESISTANCE, HUMIDITE_MAX, USAGE_RECOMMANDE, "
                      "NOMBRE_NOEUDS, CLASSE_QUALITE, "
                      "TO_CHAR(DATE_ANALYSE, 'DD/MM/YYYY') AS DATE_ANALYSE "
                      "FROM BOIS WHERE 1=1 ";

    if (!nom.isEmpty()) {
        baseStr += " AND LOWER(NOMBOIS) LIKE LOWER(:nom)";
    }
    if (minPrix >= 0 && maxPrix >= minPrix && maxPrix > 0) {
        baseStr += " AND PRIX_UNITAIRE BETWEEN :minPrix AND :maxPrix";
    }
    if (!fournisseur.isEmpty() && fournisseur != "Tous") {
        baseStr += " AND LOWER(FOURNISSEUR) = LOWER(:fournisseur)";
    }

    baseStr += " ORDER BY IDBOIS";

    query.prepare(baseStr);

    if (!nom.isEmpty()) {
        query.bindValue(":nom", "%" + nom + "%");
    }
    if (minPrix >= 0 && maxPrix >= minPrix && maxPrix > 0) {
        query.bindValue(":minPrix", minPrix);
        query.bindValue(":maxPrix", maxPrix);
    }
    if (!fournisseur.isEmpty() && fournisseur != "Tous") {
        query.bindValue(":fournisseur", fournisseur);
    }

    query.exec();
    model->setQuery(std::move(query));

    if (model->lastError().isValid()) {
        qDebug() << "Erreur filtrer Bois:" << model->lastError().text();
    }

    model->setHeaderData(0,  Qt::Horizontal, "ID");
    model->setHeaderData(1,  Qt::Horizontal, "Nom");
    model->setHeaderData(2,  Qt::Horizontal, "Longueur");
    model->setHeaderData(3,  Qt::Horizontal, "Largeur");
    model->setHeaderData(4,  Qt::Horizontal, "Quantité");
    model->setHeaderData(5,  Qt::Horizontal, "Prix Unitaire");
    model->setHeaderData(6,  Qt::Horizontal, "Fournisseur");
    model->setHeaderData(7,  Qt::Horizontal, "Résistance");
    model->setHeaderData(8,  Qt::Horizontal, "Humidité Max");
    model->setHeaderData(9,  Qt::Horizontal, "Usage Recommandé");
    model->setHeaderData(10, Qt::Horizontal, "Nbre Nœuds");
    model->setHeaderData(11, Qt::Horizontal, "Classe Qualité");
    model->setHeaderData(12, Qt::Horizontal, "Date Analyse");

    return model;
}

// ============================================// ============================================================
//  CRUD � Afficher (READ)
// ============================================================
//  Statistics
// ============================================// ============================================================
//  CRUD � Afficher (READ)
// ============================================================

#include "woodexpert.h"

QMap<QString, int> Bois::getQualityDistribution() const
{
    QMap<QString, int> stats;
    QString errorMessage;
    if (!ensureStorage(&errorMessage)) {
        qDebug() << "Erreur schema Bois:" << errorMessage;
        return stats;
    }

    // We retrieve all records to perform "Smart Inference" on unclassified data
    QSqlQuery query("SELECT NOMBOIS, CLASSE_QUALITE FROM BOIS");
    while (query.next()) {
        QString name = query.value(0).toString();
        QString cl = query.value(1).toString().trimmed();

        // --- SMART INFERENCE ---
        // If the database field is empty, we determine the quality based on WoodExpert properties
        if (cl.isEmpty()) {
            WoodExpert::WoodProps props = WoodExpert::getInfo(name);
            if (props.resistanceMecanique >= 8) cl = "A";
            else if (props.resistanceMecanique >= 5) cl = "B";
            else cl = "C";
        }

        if (cl.isEmpty()) cl = "Non class\u00e9"; // Fallback for unknown
        stats[cl]++;
    }
    return stats;
}

QVector<QPair<QString, int>> Bois::getStockAnalysis() const
{
    QVector<QPair<QString, int>> stats;
    QString errorMessage;
    if (!ensureStorage(&errorMessage)) {
        qDebug() << "Erreur schema Bois:" << errorMessage;
        return stats;
    }

    QSqlQuery query("SELECT NOMBOIS, SUM(QUANTITE) FROM BOIS GROUP BY NOMBOIS ORDER BY SUM(QUANTITE) DESC");
    while (query.next()) {
        stats.append({query.value(0).toString(), query.value(1).toInt()});
    }
    return stats;
}

QVector<QPair<QString, double>> Bois::getPriceDistribution() const
{
    QVector<QPair<QString, double>> stats;
    QString errorMessage;
    if (!ensureStorage(&errorMessage)) {
        qDebug() << "Erreur schema Bois:" << errorMessage;
        return stats;
    }

    QSqlQuery query("SELECT NOMBOIS, AVG(PRIX_UNITAIRE) FROM BOIS GROUP BY NOMBOIS ORDER BY AVG(PRIX_UNITAIRE) DESC");
    while (query.next()) {
        stats.append({query.value(0).toString(), query.value(1).toDouble()});
    }
    return stats;
}

QMap<QString, int> Bois::getSupplierDistribution() const
{
    QMap<QString, int> stats;
    QString errorMessage;
    if (!ensureStorage(&errorMessage)) {
        qDebug() << "Erreur schema Bois:" << errorMessage;
        return stats;
    }

    QSqlQuery query("SELECT FOURNISSEUR, COUNT(*) FROM BOIS GROUP BY FOURNISSEUR");
    while (query.next()) {
        QString f = query.value(0).toString();
        if (f.isEmpty()) f = "Inconnu";
        stats[f] = query.value(1).toInt();
    }
    return stats;
}

double Bois::getTotalStockValue() const
{
    QString errorMessage;
    if (!ensureStorage(&errorMessage)) {
        qDebug() << "Erreur schema Bois:" << errorMessage;
        return 0.0;
    }

    QSqlQuery query("SELECT SUM(QUANTITE * PRIX_UNITAIRE) FROM BOIS");
    if (query.next()) return query.value(0).toDouble();
    return 0.0;
}

int Bois::getTotalStockQuantity() const
{
    QString errorMessage;
    if (!ensureStorage(&errorMessage)) {
        qDebug() << "Erreur schema Bois:" << errorMessage;
        return 0;
    }

    QSqlQuery query("SELECT SUM(QUANTITE) FROM BOIS");
    if (query.next()) return query.value(0).toInt();
    return 0;
}

QString Bois::getTopSpecies() const
{
    QString errorMessage;
    if (!ensureStorage(&errorMessage)) {
        qDebug() << "Erreur schema Bois:" << errorMessage;
        return "N/A";
    }

    QSqlQuery query("SELECT NOMBOIS FROM BOIS GROUP BY NOMBOIS ORDER BY SUM(QUANTITE) DESC");
    if (query.next()) return query.value(0).toString();
    return "N/A";
}

// ============================================// ============================================================
//  CRUD � Afficher (READ)
// ============================================================
//  AI & Recommendation
// ============================================// ============================================================
//  CRUD � Afficher (READ)
// ============================================================

QList<Bois::RecommendationResult> Bois::recommanderBois(double budgetMax, const QString &usageRequis, const QString &resistanceRequise, double humiditeMaxRequis)
{
    QList<RecommendationResult> results;
    QString errorMessage;
    if (!ensureStorage(&errorMessage)) {
        qDebug() << "Erreur schema Bois:" << errorMessage;
        return results;
    }

    QSqlQuery query;
    query.prepare("SELECT IDBOIS, NOMBOIS, LONGUEUR, LARGEUR, EPAISSEUR, QUANTITE, PRIX_UNITAIRE, "
                  "FOURNISSEUR, RESISTANCE, HUMIDITE_MAX, USAGE_RECOMMANDE, "
                  "IMAGE_STOCK, IMAGE_ANALYSE, NOMBRE_NOEUDS, CLASSE_QUALITE, DATE_ANALYSE "
                  "FROM BOIS");

    if (!query.exec()) {
        qDebug() << "Recommendation query failed:" << query.lastError().text();
        return results;
    }

    // =========================================================================
    // HELPER LAMBDAS
    // =========================================================================
    auto normalize = [](QString text) {
        text = text.trimmed().toLower();
        text.replace("é", "e").replace("è", "e").replace("ê", "e").replace("ë", "e");
        text.replace("�", "a").replace("�", "a").replace("�", "a");
        text.replace("î", "i").replace("ï", "i");
        text.replace("ô", "o").replace("ö", "o");
        text.replace("ù", "u").replace("û", "u").replace("ü", "u");
        text.replace("ç", "c");
        return text;
    };

    auto resistanceLevel = [&](const QString &r) {
        const QString v = normalize(r);
        if (v.contains("tres") && v.contains("forte")) return 4;
        if (v.contains("forte")) return 3;
        if (v.contains("moyenne")) return 2;
        if (v.contains("faible")) return 1;
        return 0;
    };

    auto qualityLevel = [&](const QString &q) {
        const QString v = normalize(q);
        if (v == "a") return 3;
        if (v == "b") return 2;
        if (v == "c") return 1;
        return 0;
    };

    // =========================================================================
    // SCORING FUNCTION - 7 MATERIALS SUPPORT
    // =========================================================================
    auto calculateScore = [&](const Bois &wood,
                              const QString &usageRequis,
                              int requestedResistanceLevel,
                              double humiditeMaxRequis,
                              double budgetMax) -> double
    {
        const QString u = normalize(usageRequis);
        const QString n = normalize(wood.getNomBois());
        const double price = wood.getPrixUnitaire();
        const int quantity = wood.getQuantite();
        const double longueur = wood.getLongueur();
        const double largeur = wood.getLargeur();
        const double epaisseur = wood.getEpaisseur();
        const double surface = qMax(0.0, longueur) * qMax(0.0, largeur);

        const bool isMdf = n.contains("mdf");
        const bool isContreplaque = n.contains("contreplaq");
        const bool isAgglomere = n.contains("agglomere");
        const bool isMelamineBlanc = n.contains("melamine");
        const bool isStratifie = n.contains("stratif");
        const bool isChenePlaque = n.contains("chene") && n.contains("plaq");
        const bool isPanneauMassif = n.contains("panneau massif") || (n.contains("massif") && !isChenePlaque);

        const bool isStructure = (u.contains("structure") || u.contains("porteur") || u.contains("ossature"));
        const bool isEbenisterie = (u.contains("ebenisterie") || u.contains("fine") || u.contains("meuble"));
        const bool isFinition = (u.contains("finition") || u.contains("decoration") || u.contains("revetement"));
        const bool isExterieur = (u.contains("exterieur") || u.contains("terrasse") || u.contains("facade"));

        double score = 0.0;

        // 1) Humidity first.
        if (humiditeMaxRequis >= 70.0) {
            if (isStratifie) score += 72.0;
            else if (isContreplaque) score += 60.0;
            else if (isPanneauMassif) score += 34.0;
            else if (isMdf) score -= 26.0;
            else if (isAgglomere) score -= 30.0;
            else if (isMelamineBlanc) score -= 14.0;
        } else if (humiditeMaxRequis >= 40.0) {
            if (isStratifie) score += 28.0;
            else if (isContreplaque) score += 22.0;
            else if (isPanneauMassif) score += 10.0;
            else if (isMdf) score -= 6.0;
            else if (isAgglomere) score -= 8.0;
        } else if (humiditeMaxRequis > 0.0) {
            if (isStratifie) score -= 8.0;
            if (isMdf) score += 4.0;
            if (isAgglomere) score += 2.0;
        }

        // 2) Usage.
        if (isStructure) {
            if (isContreplaque) score += 42.0;
            else if (isPanneauMassif) score += 38.0;
            else if (isMdf) score += 12.0;
            else if (isAgglomere) score += 4.0;
            else if (isStratifie) score -= 4.0;
            else if (isMelamineBlanc) score -= 6.0;
        } else if (isFinition) {
            if (isStratifie) score += 40.0;
            else if (isMelamineBlanc) score += 36.0;
            else if (isContreplaque) score += 8.0;
            else if (isPanneauMassif) score += 4.0;
            else if (isMdf) score -= 2.0;
        } else if (isEbenisterie) {
            if (isPanneauMassif) score += 34.0;
            else if (isMdf) score += 30.0;
            else if (isChenePlaque) score += 16.0;
            else if (isContreplaque) score += 10.0;
            else if (isAgglomere) score += 2.0;
        } else if (isExterieur) {
            if (isStratifie) score += 38.0;
            else if (isContreplaque) score += 34.0;
            else if (isPanneauMassif) score += 18.0;
            else if (isMdf) score -= 6.0;
            else if (isAgglomere) score -= 8.0;
        } else {
            if (isContreplaque) score += 18.0;
            else if (isPanneauMassif) score += 16.0;
            else if (isMdf) score += 10.0;
            else if (isStratifie) score += 8.0;
            else if (isMelamineBlanc) score += 6.0;
            else if (isAgglomere) score += 4.0;
        }

        // 3) Resistance.
        if (requestedResistanceLevel >= 4) {
            if (isPanneauMassif) score += 34.0;
            else if (isContreplaque) score += 30.0;
            else if (isMdf) score += 4.0;
            else if (isAgglomere) score -= 6.0;
        } else if (requestedResistanceLevel == 3) {
            if (isContreplaque) score += 28.0;
            else if (isPanneauMassif) score += 22.0;
            else if (isMdf) score += 8.0;
            else if (isStratifie) score += 2.0;
            else if (isAgglomere) score -= 4.0;
        } else if (requestedResistanceLevel == 2) {
            if (isMdf) score += 18.0;
            else if (isStratifie) score += 16.0;
            else if (isChenePlaque) score += 8.0;
            else if (isContreplaque) score += 6.0;
        } else if (requestedResistanceLevel == 1) {
            if (isAgglomere) score += 18.0;
            else if (isMelamineBlanc) score += 16.0;
            else if (isMdf) score += 6.0;
            else if (isContreplaque || isPanneauMassif) score -= 6.0;
        }

        // 4) Stock and geometry.
        if (quantity <= 0) {
            score -= 18.0;
        } else if (quantity >= 100) {
            score += 8.0;
        } else if (quantity >= 40) {
            score += 5.0;
        } else if (quantity >= 10) {
            score += 2.0;
        }

        if (epaisseur > 0.0) {
            if ((isStructure || isExterieur) && epaisseur >= 18.0) score += 3.0;
            if ((isStructure || isExterieur) && epaisseur < 12.0) score -= 3.0;
            if (isFinition && epaisseur >= 8.0 && epaisseur <= 20.0) score += 1.0;
            if (isEbenisterie && epaisseur >= 12.0 && epaisseur <= 25.0) score += 1.0;
        }

        if (surface > 0.0) {
            if ((isStructure || isExterieur) && surface >= 1.0) score += 2.0;
            if ((isStructure || isExterieur) && surface < 0.25) score -= 2.0;
        }

        // 5) Budget, intentionally weak.
        if (budgetMax > 0.0 && price > 0.0) {
            const double ratio = price / budgetMax;

            if (price > budgetMax) {
                const double overRatio = (price - budgetMax) / qMax(1.0, budgetMax);
                score -= qBound(0.0, overRatio * 8.0, 8.0);
            } else {
                if (ratio <= 0.40) score += 4.0;
                else if (ratio <= 0.65) score += 2.0;
                else if (ratio >= 0.95) score -= 1.0;
            }

            if (budgetMax < 300.0) {
                if (isAgglomere) score += 8.0;
                else if (isMelamineBlanc) score += 6.0;
                else if (isMdf) score += 2.0;
                else if (isContreplaque || isPanneauMassif || isChenePlaque) score -= 3.0;
            } else if (budgetMax <= 700.0) {
                if (isMdf) score += 4.0;
                else if (isStratifie || isContreplaque) score += 2.0;
            } else {
                if (isContreplaque || isPanneauMassif || isChenePlaque) score += 6.0;
            }
        }

        // 6) Final stability adjustments.
        if (humiditeMaxRequis >= 70.0) {
            if (isMdf) score -= 12.0;
            if (isAgglomere) score -= 14.0;
            if (isStratifie) score += 8.0;
            if (isContreplaque) score += 4.0;
        }

        if (isMdf && (isStructure || requestedResistanceLevel >= 3 || humiditeMaxRequis >= 70.0)) {
            score -= 6.0;
        }

        if (isPanneauMassif && (isStructure || requestedResistanceLevel >= 3)) {
            score += 6.0;
        }

        return qBound(0.0, score, 100.0);
    };

    // =========================================================================
    // MAIN LOOP: Process all woods and compute scores
    // =========================================================================
    const QString usageNorm = normalize(usageRequis);
    const int requestedResistanceLevel = resistanceLevel(resistanceRequise);

    while (query.next()) {
        Bois b(query.value(0).toInt(), query.value(1).toString(), query.value(2).toDouble(),
               query.value(3).toDouble(), query.value(4).toDouble(), // epaisseur
               query.value(5).toInt(), query.value(6).toDouble(),
               query.value(7).toString(), query.value(8).toString(), query.value(9).toDouble(),
               query.value(10).toString(), query.value(11).toString(), query.value(12).toString(),
               query.value(13).toInt(), query.value(14).toString(), query.value(15).toDate());

        // Calculate score
        double score = calculateScore(b, usageRequis, requestedResistanceLevel, humiditeMaxRequis, budgetMax);

        // Build reason string
        QStringList reasons;
        const QString woodName = b.getNomBois();
        const QString woodNorm = normalize(woodName);

        if (usageNorm.contains("structure")) reasons << "Adapté à la structure";
        else if (usageNorm.contains("ebenisterie")) reasons << "Idéal pour ébénisterie";
        else if (usageNorm.contains("finition")) reasons << "Bon pour finition";
        else if (usageNorm.contains("exterieur")) reasons << "Approprié extérieur";

        if (requestedResistanceLevel > 0 && resistanceLevel(b.getResistance()) >= requestedResistanceLevel) {
            reasons << "Résistance conforme";
        }
        if (humiditeMaxRequis > 0 && b.getHumiditeMax() > humiditeMaxRequis) {
            reasons << "Humidité OK";
        }
        if (b.getQuantite() <= 0) {
            reasons << "Stock indisponible";
        } else if (b.getQuantite() >= 40) {
            reasons << "Stock élevé";
        }
        if (budgetMax > 0.0) {
            reasons << ((b.getPrixUnitaire() <= budgetMax) ? "Prix compatible budget" : "Prix au-dessus du budget");
        }
        reasons << ("Classe " + b.getClasseQualite());

        QString finalReason = reasons.join(" • ");
        if (finalReason.length() > 120) finalReason = finalReason.left(117) + "...";

        RecommendationResult res = {b.getIdBois(), b.getNomBois(), b.getPrixUnitaire(),
                                    b.getClasseQualite(), score, finalReason, b.getImageStock(), score};
        results.append(res);
    }

    // =========================================================================
    // SORT: Descending by score (best first)
    // =========================================================================
    std::sort(results.begin(), results.end(), [](const RecommendationResult &a, const RecommendationResult &b) {
        if (!qFuzzyCompare(a.score + 1.0, b.score + 1.0)) {
            return a.score > b.score;
        }
        return a.prixUnitaire < b.prixUnitaire;
    });

    return results;
}

QString Bois::analyserImageBois(const QString &imagePath, int idBois)
{
    if (imagePath.isEmpty()) return "Fichier invalide";

    g_lastDetectedKnots.clear();
    g_lastDetectedImagePath = QDir::cleanPath(imagePath);

    QString woodDiagnostic;
    if (!Bois::estImageBois(imagePath, idBois, &woodDiagnostic)) {
        const QString details = woodDiagnostic.isEmpty() ? "L'image ne semble pas etre une photo de bois." : woodDiagnostic;
        return "ERREUR_NON_BOIS: " + details;
    }

#if USE_OPENCV
    // --- PROFESSIONNAL OPENCV PIPELINE ---
    cv::Mat img = cv::imread(imagePath.toStdString());
    if (img.empty()) return "Erreur OpenCV : Chargement image impossible";

    cv::Mat gray;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

    // Suppression des veines du bois (Flou Gaussien important)
    cv::GaussianBlur(gray, gray, cv::Size(9, 9), 2);

    // D\u00e9tection Hough avec param\u00e8tres stricts
    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(gray, circles, cv::HOUGH_GRADIENT, 1, 80, 100, 40, 15, 120);

    std::vector<cv::Vec3f> filtered;
    for (const auto& c : circles) {
        cv::Point center(cvRound(c[0]), cvRound(c[1]));
        int r = cvRound(c[2]);

        // --- FILTRAGE PAR INTENSIT\u00c9 (CRITIQUE) ---
        // Un n\u0153ud est plus sombre que le bois environnant
        cv::Rect roi(center.x - r/2, center.y - r/2, r, r);
        roi &= cv::Rect(0, 0, gray.cols, gray.rows);
        double meanInside = cv::mean(gray(roi))[0];

        cv::Rect roiOuter(center.x - r, center.y - r, r*2, r*2);
        roiOuter &= cv::Rect(0, 0, gray.cols, gray.rows);
        double meanOutside = cv::mean(gray(roiOuter))[0];

        int ovalSupport = 0;
        int evaluatedDirections = 0;
        int quadrantHits[4] = {0, 0, 0, 0};
        const int innerRadius = qMax(2, static_cast<int>(qRound(r * 0.45)));
        const int outerRadius = qMax(innerRadius + 3, static_cast<int>(qRound(r * 1.10)));
        for (int i = 0; i < 16; ++i) {
            const double a = (2.0 * 3.14159265358979323846 * i) / 16.0;
            const int ix = center.x + static_cast<int>(qRound(innerRadius * qCos(a)));
            const int iy = center.y + static_cast<int>(qRound(innerRadius * qSin(a)));
            const int ox = center.x + static_cast<int>(qRound(outerRadius * qCos(a)));
            const int oy = center.y + static_cast<int>(qRound(outerRadius * qSin(a)));
            if (ix < 0 || ix >= gray.cols || iy < 0 || iy >= gray.rows
                || ox < 0 || ox >= gray.cols || oy < 0 || oy >= gray.rows) {
                continue;
            }
            ++evaluatedDirections;
            const int innerSample = gray.at<uchar>(iy, ix);
            const int outerSample = gray.at<uchar>(oy, ox);
            if (outerSample - innerSample >= 5) {
                ++ovalSupport;
                const int q = i / 4;
                quadrantHits[q]++;
            }
        }

        const int diameterPx = r * 2;
        const double supportRatio = evaluatedDirections > 0
                                        ? static_cast<double>(ovalSupport) / static_cast<double>(evaluatedDirections)
                                        : 0.0;
        int coveredQuadrants = 0;
        for (int q = 0; q < 4; ++q) {
            if (quadrantHits[q] > 0) {
                ++coveredQuadrants;
            }
        }

        const double contrastDelta = meanOutside - meanInside;
        const bool baseOpenCvValid = contrastDelta >= 8.0
                                     && diameterPx >= 22
                                     && evaluatedDirections >= 8
                                     && supportRatio >= 0.60
                                     && coveredQuadrants >= 3;
        const bool weakButStructuredOpenCvValid = contrastDelta >= 4.0
                                                  && diameterPx >= 24
                                                  && evaluatedDirections >= 8
                                                  && supportRatio >= 0.68
                                                  && coveredQuadrants >= 3;
        if (baseOpenCvValid || weakButStructuredOpenCvValid) {
            filtered.push_back(c);
        }
    }

    cv::Mat resultImg = img.clone();
    for (const auto& c : filtered) {
        cv::Point center(cvRound(c[0]), cvRound(c[1]));
        const int baseRadius = cvRound(c[2]);
        const int drawRadius = qMax(12, static_cast<int>(qRound(baseRadius * 1.35)));
        const cv::Scalar mainColor(0, 0, 255); // #FF0000 in BGR
        const cv::Scalar glowOuter(70, 70, 255);
        const cv::Scalar glowMid(45, 45, 255);
        const cv::Scalar glowInner(25, 25, 255);

        // Strong multi-layer glow improves readability on complex wood textures.
        cv::circle(resultImg, center, drawRadius + 8, glowOuter, 4, cv::LINE_AA);
        cv::circle(resultImg, center, drawRadius + 5, glowMid, 5, cv::LINE_AA);
        cv::circle(resultImg, center, drawRadius + 2, glowInner, 6, cv::LINE_AA);

        cv::circle(resultImg, center, drawRadius, mainColor, 7, cv::LINE_AA);
    }

    QFileInfo fi(imagePath);
    QString resName = "analysed_pro_" + fi.fileName();
    cv::imwrite((fi.absolutePath() + "/" + resName).toStdString(), resultImg);

    for (int i = 0; i < static_cast<int>(filtered.size()); ++i) {
        const cv::Vec3f &c = filtered.at(static_cast<size_t>(i));
        const double xNorm = gray.cols > 0 ? qBound(0.0, static_cast<double>(c[0]) / gray.cols, 1.0) : 0.0;
        const double yNorm = gray.rows > 0 ? qBound(0.0, static_cast<double>(c[1]) / gray.rows, 1.0) : 0.0;
        const double diameter = qMax(2.0, static_cast<double>(c[2]) * 2.0);
        const double confidence = qBound(0.35, 0.55 + (diameter / 220.0), 0.98);
        g_lastDetectedKnots.append({xNorm, yNorm, diameter, confidence, "Principal", "Moyen"});
    }
    g_lastDetectedImagePath = QDir::cleanPath(fi.absolutePath() + "/" + resName);

    int n = static_cast<int>(filtered.size());
    const double surfaceM2 = kReferenceSurfaceM2;
    const double knotsPerM2 = n / surfaceM2;
    QString cl = (n < 3) ? "A" : (n < 7 ? "B" : "C");
    return "Classe: " + cl
           + " | N\u0153uds: " + QString::number(n)
           + " | Surface (m²): " + QString::number(surfaceM2, 'f', 4)
           + " | N\u0153uds/m²: " + QString::number(knotsPerM2, 'f', 2)
           + " | Image: " + resName;

#else
    // --- INTELLIGENT NATIVE DETECTOR (Blob Analysis) ---
    QImage rawImg(imagePath);
    if (rawImg.isNull()) return "Erreur : Chargement image impossible";

    QImage img = rawImg;

    // Auto-Leveling / Contrast Stretching
    int minV = 255, maxV = 0;
    for (int y = 0; y < img.height(); y += 4) {
        for (int x = 0; x < img.width(); x += 4) {
            int v = qGray(img.pixel(x,y));
            if (v < minV) minV = v;
            if (v > maxV) maxV = v;
        }
    }
    if (maxV > minV) {
        for (int y = 0; y < img.height(); ++y) {
            for (int x = 0; x < img.width(); ++x) {
                QRgb p = img.pixel(x,y);
                int r = qBound(0, (qRed(p) - minV) * 255 / (maxV - minV), 255);
                int g = qBound(0, (qGreen(p) - minV) * 255 / (maxV - minV), 255);
                int b = qBound(0, (qBlue(p) - minV) * 255 / (maxV - minV), 255);
                img.setPixel(x, y, qRgb(r,g,b));
            }
        }
    }

    // Step 2: Integral Image Calculation for Fast Local Adaptive Thresholding (Hyper-Intelligence)
    QImage graySharp = img.convertToFormat(QImage::Format_Grayscale8);
    int width = graySharp.width();
    int height = graySharp.height();

    QVector<QVector<long>> integral(height + 1, QVector<long>(width + 1, 0));
    for (int y = 0; y < height; ++y) {
        long rowSum = 0;
        const uchar *line = graySharp.scanLine(y);
        for (int x = 0; x < width; ++x) {
            rowSum += line[x];
            integral[y+1][x+1] = rowSum + integral[y][x+1];
        }
    }

    QVector<bool> visited(width * height, false);

    struct KnotBlob { QPoint center; int area; double circularity; int width; int height; };
    QList<KnotBlob> detectedKnots;

    // Adaptive Thresholding logic removed as unused

    // Dynamic Threshold: Knots are dark spots relative to average
    // Real knots are very dark (typically < 100 on an 8-bit scale after leveling)
    // int darkThreshold = qMin(110, qMax(25, avgGray - 45));

    // Step 4: Multi-Scale Scanning (Adaptive Window: 41x41)
    int windowSize = 41;
    int halfWin = windowSize / 2;
    int thresholdOffset = 25;

    for (int y = halfWin + 5; y < height - halfWin - 5; ++y) {
        for (int x = halfWin + 5; x < width - halfWin - 5; ++x) {
            int idx = y * width + x;
            if (visited[idx]) continue;

            int valSharp = qGray(graySharp.pixel(x, y));

            // Fast Area Sum using Integral Image
            int x1 = x - halfWin, y1 = y - halfWin;
            int x2 = x + halfWin, y2 = y + halfWin;
            long areaSum = integral[y2+1][x2+1] - integral[y1][x2+1] - integral[y2+1][x1] + integral[y1][x1];
            int localAvg = areaSum / (windowSize * windowSize);

            // Detection: significantly darker than local neighborhood
            if (valSharp < (localAvg - thresholdOffset)) {
                // BFS Cluster Analysis
                QList<QPoint> queue;
                queue.append(QPoint(x, y));
                visited[idx] = true;

                long sumX = 0, sumY = 0;
                int count = 0;
                int minX = x, maxX = x, minY = y, maxY = y;

                while (!queue.isEmpty()) {
                    QPoint p = queue.takeFirst();
                    count++;
                    sumX += p.x(); sumY += p.y();
                    minX = qMin(minX, p.x()); maxX = qMax(maxX, p.x());
                    minY = qMin(minY, p.y()); maxY = qMax(maxY, p.y());

                    static const int dx[] = {1, -1, 0, 0};
                    static const int dy[] = {0, 0, 1, -1};
                    for (int i = 0; i < 4; ++i) {
                        int nx = p.x() + dx[i];
                        int ny = p.y() + dy[i];
                        if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                            int nIdx = ny * width + nx;
                            int nVal = qGray(graySharp.pixel(nx, ny));

                            // Re-calculate local check inside BFS for grouping
                            int nx1 = qMax(0, nx - 10), ny1 = qMax(0, ny - 10);
                            int nx2 = qMin(width-1, nx + 10), ny2 = qMin(height-1, ny + 10);
                            long nAreaSum = integral[ny2+1][nx2+1] - integral[ny1][nx2+1] - integral[ny2+1][nx1] + integral[ny1][nx1];
                            int nLocalAvg = nAreaSum / ((nx2-nx1+1)*(ny2-ny1+1));

                            if (!visited[nIdx] && nVal < (nLocalAvg - 10)) {
                                visited[nIdx] = true;
                                queue.append(QPoint(nx, ny));
                            }
                        }
                    }
                    if (count > 30000) break;
                }

                // Step 5: Advanced Filtering (Size, Shape & Context)
                if (count > 80 && count < 30000) {
                    int w = maxX - minX + 1;
                    int h = maxY - minY + 1;

                    // Circularity Check: Fill rate of bounding box
                    double fillRate = (double)count / (w * h);
                    double aspectRatio = (double)qMax(w, h) / qMin(w, h);

                    // Step 5: High-Precision Local Contrast Validation
                    // We check 8 points around the blob to ensure it's truly isolated
                    QPoint center(sumX/count, sumY/count);
                    // int innerGray = qGray(graySharp.pixel(center));

                    int checkDist = qMax(w, h) / 2 + 10;
                    int sumOuter = 0;
                    int sampleCount = 0;
                    int dx_c[] = {1, -1, 0, 0, 1, 1, -1, -1};
                    int dy_c[] = {0, 0, 1, -1, 1, -1, 1, -1};
                    for (int i=0; i<8; ++i) {
                        int ox = qBound(0, center.x() + dx_c[i] * checkDist, width-1);
                        int oy = qBound(0, center.y() + dy_c[i] * checkDist, height-1);
                        sumOuter += qGray(graySharp.pixel(ox, oy));
                        sampleCount++;
                    }
                    const int avgOuter = sampleCount > 0 ? (sumOuter / sampleCount) : 0;
                    const int centerGray = qGray(graySharp.pixel(center));

                    const double equivalentDiameter = 2.0 * qSqrt(static_cast<double>(count) / 3.14159265358979323846);
                    const int innerRadius = qMax(3, static_cast<int>(qRound(qMin(w, h) * 0.30)));
                    const int outerRadius = qMax(innerRadius + 4, static_cast<int>(qRound(qMax(w, h) * 0.55)));
                    int ovalSupport = 0;
                    int evaluatedDirections = 0;
                    int quadrantHits[4] = {0, 0, 0, 0};
                    for (int i = 0; i < 16; ++i) {
                        const double a = (2.0 * 3.14159265358979323846 * i) / 16.0;
                        const int ix = center.x() + static_cast<int>(qRound(innerRadius * qCos(a)));
                        const int iy = center.y() + static_cast<int>(qRound(innerRadius * qSin(a)));
                        const int ox = center.x() + static_cast<int>(qRound(outerRadius * qCos(a)));
                        const int oy = center.y() + static_cast<int>(qRound(outerRadius * qSin(a)));
                        if (ix < 0 || ix >= width || iy < 0 || iy >= height
                            || ox < 0 || ox >= width || oy < 0 || oy >= height) {
                            continue;
                        }
                        ++evaluatedDirections;
                        const int innerSample = qGray(graySharp.pixel(ix, iy));
                        const int outerSample = qGray(graySharp.pixel(ox, oy));
                        if (outerSample - innerSample >= 5) {
                            ++ovalSupport;
                            const int q = i / 4;
                            quadrantHits[q]++;
                        }
                    }
                    const double supportRatio = evaluatedDirections > 0
                                                    ? static_cast<double>(ovalSupport) / static_cast<double>(evaluatedDirections)
                                                    : 0.0;

                    int coveredQuadrants = 0;
                    for (int q = 0; q < 4; ++q) {
                        if (quadrantHits[q] > 0) {
                            ++coveredQuadrants;
                        }
                    }

                    const double aSemi = qMax(1.0, w / 2.0);
                    const double bSemi = qMax(1.0, h / 2.0);
                    const double areaEllipse = 3.14159265358979323846 * aSemi * bSemi;
                    const double hRam = qPow(aSemi - bSemi, 2.0) / qPow(aSemi + bSemi, 2.0);
                    const double perimeterEllipse = 3.14159265358979323846 * (aSemi + bSemi) * (1.0 + (3.0 * hRam) / (10.0 + qSqrt(qMax(1.0e-9, 4.0 - 3.0 * hRam))));
                    const double circularity = perimeterEllipse > 0.0
                                                   ? (4.0 * 3.14159265358979323846 * static_cast<double>(count)) / (perimeterEllipse * perimeterEllipse)
                                                   : 0.0;
                    const double fillVsEllipse = areaEllipse > 0.0
                                                     ? static_cast<double>(count) / areaEllipse
                                                     : 0.0;

                    const int contrastDelta = avgOuter - centerGray;

                    // Real knots: compact and closed regions, not isolated dark dots.
                    const bool baseNativeValid = aspectRatio < 2.0
                                                 && fillRate > 0.38
                                                 && equivalentDiameter >= 13.5
                                                 && qMin(w, h) >= 12
                                                 && contrastDelta >= 4
                                                 && evaluatedDirections >= 8
                                                 && supportRatio >= 0.58
                                                 && coveredQuadrants >= 3
                                                 && circularity >= 0.36
                                                 && fillVsEllipse >= 0.42;

                    const bool weakButStructuredNativeValid = aspectRatio < 1.85
                                                              && fillRate > 0.40
                                                              && equivalentDiameter >= 13.0
                                                              && qMin(w, h) >= 9
                                                              && contrastDelta >= 1
                                                              && evaluatedDirections >= 7
                                                              && supportRatio >= 0.64
                                                              && coveredQuadrants >= 3
                                                              && circularity >= 0.42
                                                              && fillVsEllipse >= 0.48;

                    if (baseNativeValid || weakButStructuredNativeValid) {
                        detectedKnots.append({ center, count, circularity, w, h });
                    }
                }
            }
        }
    }

    QList<KnotBlob> finalKnots;
    finalKnots.reserve(detectedKnots.size());

    const double historicalDiameter = getHistoricalAverageNodeDiameter(idBois);
    const int historicalSamples = getHistoricalNodeSamples(idBois);
    const double minDiameter = (historicalSamples >= 10 && historicalDiameter > 0.0)
                                   ? qBound(8.0, historicalDiameter * 0.65, 22.0)
                                   : 12.0;

    for (const KnotBlob &k : detectedKnots) {
        const double diameterFromArea = 2.0 * qSqrt(qMax(1.0, static_cast<double>(k.area)) / 3.14159);
        const double diameterFromBox = static_cast<double>(qMax(k.width, k.height));
        const double diameter = qMax(diameterFromArea, diameterFromBox * 0.9);
        if (diameter < minDiameter) {
            continue;
        }

        bool merged = false;
        for (KnotBlob &existing : finalKnots) {
            const double dx = static_cast<double>(existing.center.x() - k.center.x());
            const double dy = static_cast<double>(existing.center.y() - k.center.y());
            const double dist = qSqrt(dx * dx + dy * dy);
            if (dist < qMax(diameter * 0.75, 10.0)) {
                if (k.area > existing.area) {
                    existing = k;
                }
                merged = true;
                break;
            }
        }

        if (!merged) {
            finalKnots.append(k);
        }
    }

    QImage res = img.copy();
    QPainter p(&res);
    p.setRenderHint(QPainter::Antialiasing);
    const QColor mainCircleColor("#FF0000");
    for (const KnotBlob &k : finalKnots) {
        // Use a larger margin so the full knot stays clearly inside the visual ring.
        const int dominant = qMax(k.width, k.height);
        const int r = qMax(12, static_cast<int>(qRound(dominant * 0.78)));

        // Multi-layer red glow for high visibility and a cleaner professional style.
        p.setPen(QPen(QColor(255, 0, 0, 65), 15));
        p.drawEllipse(k.center, r + 6, r + 6);

        p.setPen(QPen(QColor(255, 0, 0, 105), 11));
        p.drawEllipse(k.center, r + 3, r + 3);

        p.setPen(QPen(QColor(255, 0, 0, 150), 8));
        p.drawEllipse(k.center, r + 1, r + 1);

        p.setPen(QPen(mainCircleColor, 7));
        p.drawEllipse(k.center, r, r);
    }
    p.end();

    QFileInfo fi(imagePath);
    QString name = "analysed_ai_native_" + fi.fileName();
    res.save(fi.absolutePath() + "/" + name);

    for (const KnotBlob &k : finalKnots) {
        const double xNorm = width > 0 ? qBound(0.0, static_cast<double>(k.center.x()) / width, 1.0) : 0.0;
        const double yNorm = height > 0 ? qBound(0.0, static_cast<double>(k.center.y()) / height, 1.0) : 0.0;
        const double diameterFromArea = 2.0 * qSqrt(qMax(1.0, static_cast<double>(k.area)) / 3.14159);
        const double diameterFromBox = static_cast<double>(qMax(k.width, k.height));
        const double diameter = qMax(diameterFromArea, diameterFromBox * 0.9);
        const double sizeQuality = qBound(0.0, qMin(k.width, k.height) / qMax(1.0, static_cast<double>(qMax(k.width, k.height))), 1.0);
        const double confidence = qBound(0.30, 0.35 + (k.circularity * 0.45) + (sizeQuality * 0.20), 0.98);
        const QString severity = (diameter < 18.0) ? "Faible" : ((diameter < 36.0) ? "Moyen" : "Élevé");
        g_lastDetectedKnots.append({xNorm, yNorm, diameter, confidence, "Principal", severity});
    }
    g_lastDetectedImagePath = QDir::cleanPath(fi.absolutePath() + "/" + name);

    int n = finalKnots.size();
    const double surfaceM2 = kReferenceSurfaceM2;
    const double knotsPerM2 = n / surfaceM2;
    QString cl = (n < 3) ? "A" : (n < 8 ? "B" : "C");
    return "Classe: " + cl
           + " | N\u0153uds: " + QString::number(n)
           + " | Surface (m²): " + QString::number(surfaceM2, 'f', 4)
           + " | N\u0153uds/m²: " + QString::number(knotsPerM2, 'f', 2)
           + " | Image: " + name;
#endif
}

QVector<Bois::DetectedNodeInfo> Bois::getDerniersNoeudsDetectes()
{
    QVector<DetectedNodeInfo> nodes;
    nodes.reserve(g_lastDetectedKnots.size());

    for (const DetectedKnotRow &knot : g_lastDetectedKnots) {
        DetectedNodeInfo node;
        node.positionX = knot.x;
        node.positionY = knot.y;
        node.diametreMm = knot.diameter;
        node.confiance = knot.confidence;
        node.typeNoeud = knot.type;
        node.severite = knot.severity;
        nodes.append(node);
    }

    return nodes;
}

bool Bois::estImageBois(const QString &imagePath, int idBois, QString *diagnostic)
{
    if (diagnostic) {
        diagnostic->clear();
    }

    if (imagePath.trimmed().isEmpty()) {
        if (diagnostic) *diagnostic = "Fichier image vide.";
        return false;
    }

    QImage img(imagePath);
    if (img.isNull()) {
        if (diagnostic) *diagnostic = "Chargement image impossible.";
        return false;
    }

    const QImage sample = img.scaled(96, 96, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    const int total = sample.width() * sample.height();
    if (total < 1024) {
        if (diagnostic) *diagnostic = "Resolution insuffisante pour verifier la texture.";
        return false;
    }

    int woodLikeCount = 0;
    int warmLikeCount = 0;
    int coldCount = 0;
    int neutralCount = 0;
    int edgeCount = 0;
    int edgeSamples = 0;
    int strongEdgeCount = 0;

    QVector<double> orientationBins(18, 0.0);
    double orientationEnergy = 0.0;

    double meanGray = 0.0;
    double m2Gray = 0.0;
    int nGray = 0;

    for (int y = 0; y < sample.height(); ++y) {
        for (int x = 0; x < sample.width(); ++x) {
            const QColor col = sample.pixelColor(x, y);
            const int h = col.hsvHue();
            const int s = col.hsvSaturation();
            const int v = col.value();

            const bool woodHue = (h >= 8 && h <= 55) || (h >= 330 && h <= 359);
            const bool woodLike = woodHue && s >= 28 && v >= 25 && v <= 245;
            if (woodLike) {
                ++woodLikeCount;
            }

            if (col.red() >= col.green() - 12 && col.green() >= col.blue() - 20) {
                ++warmLikeCount;
            }

            if (h >= 70 && h <= 260 && s >= 45) {
                ++coldCount;
            }

            if (s < 20) {
                ++neutralCount;
            }

            const double gray = static_cast<double>(qGray(col.rgb()));
            ++nGray;
            const double delta = gray - meanGray;
            meanGray += delta / nGray;
            const double delta2 = gray - meanGray;
            m2Gray += delta * delta2;

            if (x > 0 && y > 0) {
                const int gx = qAbs(qGray(sample.pixel(x, y)) - qGray(sample.pixel(x - 1, y)));
                const int gy = qAbs(qGray(sample.pixel(x, y)) - qGray(sample.pixel(x, y - 1)));
                const int g = gx + gy;
                if (g > 32) {
                    ++edgeCount;
                }
                if (g > 58) {
                    ++strongEdgeCount;
                }
                ++edgeSamples;
            }

            if (x > 0 && y > 0 && x < sample.width() - 1 && y < sample.height() - 1) {
                const int gxDir = qGray(sample.pixel(x + 1, y)) - qGray(sample.pixel(x - 1, y));
                const int gyDir = qGray(sample.pixel(x, y + 1)) - qGray(sample.pixel(x, y - 1));
                const double mag = qSqrt(static_cast<double>(gxDir * gxDir + gyDir * gyDir));
                if (mag > 24.0) {
                    double angle = qAtan2(static_cast<double>(gyDir), static_cast<double>(gxDir)) * 180.0 / 3.141592653589793;
                    if (angle < 0.0) {
                        angle += 180.0;
                    }
                    int bin = static_cast<int>(angle / 10.0);
                    if (bin < 0) bin = 0;
                    if (bin >= orientationBins.size()) bin = orientationBins.size() - 1;
                    orientationBins[bin] += mag;
                    orientationEnergy += mag;
                }
            }
        }
    }

    const double woodRatio = static_cast<double>(woodLikeCount) / total;
    const double warmRatio = static_cast<double>(warmLikeCount) / total;
    const double coldRatio = static_cast<double>(coldCount) / total;
    const double neutralRatio = static_cast<double>(neutralCount) / total;
    const double edgeRatio = edgeSamples > 0 ? static_cast<double>(edgeCount) / edgeSamples : 0.0;
    const double strongEdgeRatio = edgeSamples > 0 ? static_cast<double>(strongEdgeCount) / edgeSamples : 0.0;
    const double variance = (nGray > 1) ? (m2Gray / (nGray - 1)) : 0.0;
    const double stddev = qSqrt(variance);

    double dominantOrientationRatio = 0.0;
    if (orientationEnergy > 0.0) {
        double maxBin = 0.0;
        for (double b : orientationBins) {
            if (b > maxBin) {
                maxBin = b;
            }
        }
        dominantOrientationRatio = maxBin / orientationEnergy;
    }

    const int historicalSamples = getHistoricalNodeSamples(idBois);
    const double historicalDiameter = getHistoricalAverageNodeDiameter(idBois);
    const bool hasHistory = historicalSamples >= 8;
    const bool lowTextureFamily = hasHistory && historicalDiameter < 14.0;

    const double minWoodRatio = lowTextureFamily ? 0.10 : 0.12;
    const double maxColdRatio = lowTextureFamily ? 0.62 : 0.58;
    const double minWarmRatio = lowTextureFamily ? 0.18 : 0.20;
    const double maxNeutralRatio = lowTextureFamily ? 0.78 : 0.74;
    const double minStdDev = lowTextureFamily ? 5.0 : 6.0;
    const double maxStrongEdgeRatio = lowTextureFamily ? 0.18 : 0.16;
    const double minDominantOrientation = lowTextureFamily ? 0.11 : 0.13;

    // Hard reject rules: extremely unlikely to be wood.
    if (woodRatio < 0.06) {
        if (diagnostic) {
            *diagnostic = "Image refusee: presque aucun ton bois detecte.";
        }
        return false;
    }

    if (coldRatio > 0.78) {
        if (diagnostic) {
            *diagnostic = "Image refusee: dominante froide tres elevee, incompatible avec du bois.";
        }
        return false;
    }

    if (neutralRatio > 0.90 && warmRatio < 0.12) {
        if (diagnostic) {
            *diagnostic = "Image refusee: scene trop neutre/grise sans signature bois.";
        }
        return false;
    }

    if (woodRatio < minWoodRatio) {
        if (diagnostic) {
            *diagnostic = "Image refusee: les tons bois detectes sont insuffisants (" + QString::number(woodRatio * 100.0, 'f', 1) + "%).";
        }
        return false;
    }

    if (warmRatio < minWarmRatio && woodRatio < 0.16) {
        if (diagnostic) {
            *diagnostic = "Image refusee: dominance chromatique non compatible avec le bois.";
        }
        return false;
    }

    if (coldRatio > maxColdRatio) {
        if (diagnostic) {
            *diagnostic = "Image refusee: trop de tons froids (ciel/eau/herbe) pour une photo de bois.";
        }
        return false;
    }

    if (neutralRatio > maxNeutralRatio && woodRatio < 0.15) {
        if (diagnostic) {
            *diagnostic = "Image refusee: photo trop neutre (gris/noir/blanc), texture bois non confirmee.";
        }
        return false;
    }

    if (stddev < minStdDev && edgeRatio < 0.010) {
        if (diagnostic) {
            *diagnostic = "Image refusee: texture trop uniforme pour du bois naturel.";
        }
        return false;
    }

    if (strongEdgeRatio > maxStrongEdgeRatio && woodRatio < 0.26) {
        if (diagnostic) {
            *diagnostic = "Image refusee: texture trop complexe/non fibreuse pour du bois.";
        }
        return false;
    }

    if (dominantOrientationRatio < minDominantOrientation) {
        if (diagnostic) {
            *diagnostic = "Image refusee: les fibres du bois ne sont pas detectees de maniere coherente.";
        }
        return false;
    }

    return true;
}

bool Bois::enregistrerResultatsAnalyse(int id, int nbNoeuds, const QString &classe, const QString &imageAnalyse)
{
    lastErrMsg.clear();
    if (!ensureStorage(&lastErrMsg)) {
        return false;
    }

    QSqlQuery query;
    query.prepare(
        "UPDATE BOIS SET "
        "  NOMBRE_NOEUDS = :nb, "
        "  CLASSE_QUALITE = :cl, "
        "  DATE_ANALYSE = :dt, "
        "  IMAGE_ANALYSE = :img "
        "WHERE IDBOIS = :id"
        );
    query.bindValue(":nb",  nbNoeuds);
    query.bindValue(":cl",  classe);
    query.bindValue(":dt",  QDate::currentDate());
    query.bindValue(":img", imageAnalyse);
    query.bindValue(":id",  id);

    if (!query.exec()) {
        lastErrMsg = "Erreur DB : " + query.lastError().text();
        return false;
    }

    QString noeudsStorageError;
    if (!ensureBoisNoeudsStorage(&noeudsStorageError)) {
        lastErrMsg = "Erreur BOIS_NOEUDS : " + noeudsStorageError;
        return false;
    }

    const QString imageKey = imageAnalyse.trimmed().isEmpty()
                                 ? QFileInfo(g_lastDetectedImagePath).fileName()
                                 : imageAnalyse.trimmed();

    QSqlQuery purgeQuery;
    purgeQuery.prepare(
        "DELETE FROM BOIS_NOEUDS "
        "WHERE IDBOIS = :id AND UPPER(NVL(IMAGE_RESULTAT, '-')) = UPPER(:img)");
    purgeQuery.bindValue(":id", id);
    purgeQuery.bindValue(":img", imageKey);
    if (!purgeQuery.exec()) {
        lastErrMsg = "Erreur purge BOIS_NOEUDS : " + purgeQuery.lastError().text();
        return false;
    }

    const int nodesToPersist = qMin(nbNoeuds, g_lastDetectedKnots.size());
    for (int i = 0; i < nodesToPersist; ++i) {
        const DetectedKnotRow &knot = g_lastDetectedKnots.at(i);
        QSqlQuery insertNode;
        insertNode.prepare(
            "INSERT INTO BOIS_NOEUDS ("
            "  id_noeud, idBois, numero_noeud, position_x, position_y, diametre_mm,"
            "  type_noeud, confiance, severite, date_detection, image_resultat, notes"
            ") VALUES ("
            "  BOIS_NOEUD_SEQ.NEXTVAL, :idBois, :numero, :x, :y, :diam,"
            "  :type, :conf, :sev, SYSTIMESTAMP, :img, :notes"
            ")");

        insertNode.bindValue(":idBois", id);
        insertNode.bindValue(":numero", i + 1);
        insertNode.bindValue(":x", knot.x);
        insertNode.bindValue(":y", knot.y);
        insertNode.bindValue(":diam", knot.diameter);
        insertNode.bindValue(":type", knot.type.isEmpty() ? QString("Principal") : knot.type);
        insertNode.bindValue(":conf", qBound(0.0, knot.confidence, 1.0));
        insertNode.bindValue(":sev", knot.severity.isEmpty() ? QString("Moyen") : knot.severity);
        insertNode.bindValue(":img", imageKey);
        insertNode.bindValue(":notes", QString("Analyse IA auto"));

        if (!insertNode.exec()) {
            lastErrMsg = "Erreur insertion BOIS_NOEUDS : " + insertNode.lastError().text();
            return false;
        }
    }

    return true;
}
