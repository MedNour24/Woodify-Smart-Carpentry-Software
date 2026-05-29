#include "fabrication.h"

#include <QDebug>
#include <QDateTime>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QPair>
#include <QSet>
#include <QStringList>
#include <QVector>
#include <utility>

namespace {
constexpr double kReferenceAngleDeg = 90.0;
constexpr double kAngleToleranceDeg = 10.0;
constexpr int kAnglePenalty = 35;

bool fabricationHasAngleColumn()
{
    QSqlQuery metadataQuery;
    if (!metadataQuery.exec("SELECT * FROM FABRICATION WHERE 1 = 0")) {
        qWarning() << "Fabrication metadata query failed:" << metadataQuery.lastError().text();
        return false;
    }

    return metadataQuery.record().indexOf("ANGLE") >= 0;
}

bool fabricationHasTemperatureColumn()
{
    QSqlQuery metadataQuery;
    if (!metadataQuery.exec("SELECT * FROM FABRICATION WHERE 1 = 0")) {
        return false;
    }
    return metadataQuery.record().indexOf("TEMPERATURE_DETECTEE") >= 0;
}
}

// ============================================================
//  Constructors
// ============================================================

Fabrication::Fabrication()
    : idFabrication(0),
      nb_personnes(0),
      idModele(0),
      angle(90.0)
{}

Fabrication::Fabrication(int idFabrication, const QDate &date_debut,
                         const QDate &date_fin_prevue, const QDate &date_fin_reelle,
                         int nb_personnes, const QString &etat_fabrication, int idModele, double angle)
    : idFabrication(idFabrication),
      date_debut(date_debut),
      date_fin_prevue(date_fin_prevue),
      date_fin_reelle(date_fin_reelle),
      nb_personnes(nb_personnes),
      etat_fabrication(etat_fabrication),
      idModele(idModele),
      angle(angle)
{}

// ============================================================
//  Getters
// ============================================================

int     Fabrication::getIdFabrication()   const { return idFabrication; }
QDate   Fabrication::getDateDebut()       const { return date_debut; }
QDate   Fabrication::getDateFinPrevue()   const { return date_fin_prevue; }
QDate   Fabrication::getDateFinReelle()   const { return date_fin_reelle; }
int     Fabrication::getNbPersonnes()     const { return nb_personnes; }
QString Fabrication::getEtatFabrication() const { return etat_fabrication; }
int     Fabrication::getIdModele()        const { return idModele; }
double  Fabrication::getAngle()           const { return angle; }
QString Fabrication::getLastError()       const { return lastErrMsg; }

// ============================================================
//  Setters
// ============================================================

void Fabrication::setIdFabrication(int id)              { idFabrication = id; }
void Fabrication::setDateDebut(const QDate &date)       { date_debut = date; }
void Fabrication::setDateFinPrevue(const QDate &date)   { date_fin_prevue = date; }
void Fabrication::setDateFinReelle(const QDate &date)   { date_fin_reelle = date; }
void Fabrication::setNbPersonnes(int nb)                { nb_personnes = nb; }
void Fabrication::setEtatFabrication(const QString &e)  { etat_fabrication = e; }
void Fabrication::setIdModele(int id)                   { idModele = id; }
void Fabrication::setAngle(double a)                    { angle = a; }

// ============================================================
//  Allowed states
// ============================================================

QStringList Fabrication::etatsAutorises()
{
    return {"EN_ATTENTE", "EN_COURS", "TERMINEE", "ANNULEE"};
}

// ============================================================
//  Input Validation
// ============================================================

bool Fabrication::validerDateDebut(const QDate &date)
{
    return date.isValid();
}

bool Fabrication::validerDateFinPrevue(const QDate &dateDebut, const QDate &dateFinPrevue)
{
    return dateFinPrevue.isValid() && dateFinPrevue >= dateDebut;
}

bool Fabrication::validerNbPersonnes(int nb)
{
    return nb >= 1 && nb <= 1000;
}

bool Fabrication::validerEtat(const QString &etat)
{
    return etatsAutorises().contains(normaliserEtat(etat));
}

bool Fabrication::validerIdModele(int idModele)
{
    return idModele > 0;
}

QString Fabrication::normaliserEtat(const QString &etat)
{
    return etat.trimmed().toUpper();
}

bool Fabrication::validerTransitionEtat(const QString &ancienEtat, const QString &nouvelEtat)
{
    const QString from = normaliserEtat(ancienEtat);
    const QString to = normaliserEtat(nouvelEtat);
    if (!validerEtat(from) || !validerEtat(to)) {
        return false;
    }
    if (from == to) {
        return true;
    }

    if (from == "EN_ATTENTE") return to == "EN_COURS" || to == "ANNULEE";
    if (from == "EN_COURS") return to == "TERMINEE" || to == "ANNULEE";
    if (from == "TERMINEE") return false;
    if (from == "ANNULEE") return false;
    return false;
}

QString Fabrication::calculerRisqueRetard(const QDate &dateFinPrevue,
                                          const QDate &dateFinReelle,
                                          const QString &etatFabrication)
{
    const QString etat = normaliserEtat(etatFabrication);
    if (!dateFinPrevue.isValid()) {
        return "Inconnu";
    }

    if (dateFinReelle.isValid()) {
        const int delay = dateFinPrevue.daysTo(dateFinReelle);
        if (delay > 7) return "Eleve";
        if (delay > 2) return "Moyen";
        return "Faible";
    }

    if (etat == "TERMINEE") {
        return "Faible";
    }
    if (etat == "ANNULEE") {
        return "N/A";
    }

    const int currentDelay = dateFinPrevue.daysTo(QDate::currentDate());
    if (currentDelay > 7) return "Eleve";
    if (currentDelay > 2) return "Moyen";
    return "Faible";
}

double Fabrication::calculerProgressionPhases(int idFabrication,
                                              int *totalPhases,
                                              int *donePhases,
                                              int *inProgressPhases,
                                              QString *errorMessage)
{
    if (errorMessage) {
        errorMessage->clear();
    }
    if (idFabrication <= 0) {
        if (errorMessage) {
            *errorMessage = "ID fabrication invalide.";
        }
        return 0.0;
    }

    QSqlQuery query;
    query.prepare(
        "SELECT COUNT(*) AS TOTAL, "
        "NVL(SUM(CASE WHEN UPPER(TRIM(ETAT)) LIKE 'TERMINE%' THEN 1 ELSE 0 END), 0) AS DONE, "
        "NVL(SUM(CASE WHEN UPPER(TRIM(ETAT)) = 'EN_COURS' THEN 1 ELSE 0 END), 0) AS IN_PROGRESS "
        "FROM PHASE "
        "WHERE IDFABRICATION = :idFabrication");
    query.bindValue(":idFabrication", idFabrication);

    if (!query.exec() || !query.next()) {
        if (errorMessage) {
            *errorMessage = query.lastError().text();
        }
        return 0.0;
    }

    const int total = query.value(0).toInt();
    const int done = query.value(1).toInt();
    const int inProgress = query.value(2).toInt();

    if (totalPhases) *totalPhases = total;
    if (donePhases) *donePhases = done;
    if (inProgressPhases) *inProgressPhases = inProgress;

    if (total <= 0) {
        return 0.0;
    }

    const double ratio = (static_cast<double>(done) / static_cast<double>(total)) * 100.0;
    return qBound(0.0, ratio, 100.0);
}

bool Fabrication::genererPhasesParDefaut(int idFabrication, QString *errorMessage)
{
    if (errorMessage) {
        errorMessage->clear();
    }
    if (idFabrication <= 0) {
        if (errorMessage) {
            *errorMessage = "ID fabrication invalide.";
        }
        return false;
    }

    int existingCount = 0;
    QSqlQuery existingQuery;
    existingQuery.prepare("SELECT COUNT(*) FROM PHASE WHERE IDFABRICATION = :idFabrication");
    existingQuery.bindValue(":idFabrication", idFabrication);
    if (!existingQuery.exec() || !existingQuery.next()) {
        if (errorMessage) {
            *errorMessage = existingQuery.lastError().text();
        }
        return false;
    }
    existingCount = existingQuery.value(0).toInt();
    if (existingCount >= 5) {
        if (errorMessage) {
            *errorMessage = "Cette fabrication contient deja 5 etapes ou plus.";
        }
        return true;
    }

    QSqlQuery maxIdQuery;
    if (!maxIdQuery.exec("SELECT NVL(MAX(ID_PHASE), 0) FROM PHASE") || !maxIdQuery.next()) {
        if (errorMessage) {
            *errorMessage = maxIdQuery.lastError().text();
        }
        return false;
    }
    int nextId = maxIdQuery.value(0).toInt() + 1;

    while (existingCount < 5) {
        QSqlQuery insertQuery;
        insertQuery.prepare(
            "INSERT INTO PHASE (ID_PHASE, NOM_PHASE, COMPETENCE_REQUISE, ETAT, DATE_DEBUT, DATE_FIN_PREVUE, DATE_FIN_REELLE, IDFABRICATION) "
            "VALUES (:idPhase, :nom, 'Planification', 'NON_COMMENCE', NULL, NULL, NULL, :idFabrication)");
        insertQuery.bindValue(":idPhase", nextId++);
        insertQuery.bindValue(":nom", QString());
        insertQuery.bindValue(":idFabrication", idFabrication);
        if (!insertQuery.exec()) {
            if (errorMessage) {
                *errorMessage = insertQuery.lastError().text();
            }
            return false;
        }
        ++existingCount;
    }

    return true;
}

bool Fabrication::avancerPhase(int idPhase, QString *errorMessage)
{
    if (errorMessage) {
        errorMessage->clear();
    }
    if (idPhase <= 0) {
        if (errorMessage) {
            *errorMessage = "ID phase invalide.";
        }
        return false;
    }

    QSqlQuery phaseQuery;
    phaseQuery.prepare("SELECT ETAT, IDFABRICATION, DATE_DEBUT FROM PHASE WHERE ID_PHASE = :idPhase");
    phaseQuery.bindValue(":idPhase", idPhase);
    if (!phaseQuery.exec() || !phaseQuery.next()) {
        if (errorMessage) {
            *errorMessage = phaseQuery.lastError().text();
        }
        return false;
    }

    const QString currentEtat = phaseQuery.value(0).toString().trimmed().toUpper();
    const int idFabrication = phaseQuery.value(1).toInt();
    const QDateTime currentStart = phaseQuery.value(2).toDateTime();
    if (currentEtat.startsWith("TERMINE")) {
        if (errorMessage) {
            *errorMessage = "La phase est deja terminee.";
        }
        return false;
    }

    QSqlDatabase db = QSqlDatabase::database();
    const bool hasTransaction = db.isValid();
    if (hasTransaction && !db.transaction()) {
        if (errorMessage) {
            *errorMessage = db.lastError().text();
        }
        return false;
    }

    const QDateTime now = QDateTime::currentDateTime();
    const QString nowStr = now.toString("yyyy-MM-dd HH:mm:ss");
    const QString startNowStr = (currentStart.isValid() ? currentStart : now).toString("yyyy-MM-dd HH:mm:ss");
    bool ok = false;

    if (currentEtat == "EN_COURS") {
        QSqlQuery finishQuery;
        finishQuery.prepare(
            "UPDATE PHASE "
            "SET ETAT = 'TERMINE_BON', "
            "    DATE_DEBUT = CASE WHEN DATE_DEBUT IS NULL THEN TO_DATE(:startNow, 'YYYY-MM-DD HH24:MI:SS') ELSE DATE_DEBUT END, "
            "    DATE_FIN_REELLE = TO_DATE(:endNow, 'YYYY-MM-DD HH24:MI:SS') "
            "WHERE ID_PHASE = :idPhase");
        finishQuery.bindValue(":startNow", startNowStr);
        finishQuery.bindValue(":endNow", nowStr);
        finishQuery.bindValue(":idPhase", idPhase);
        ok = finishQuery.exec() && finishQuery.numRowsAffected() > 0;
        if (!ok && errorMessage) {
            *errorMessage = finishQuery.lastError().text();
        }

        if (ok) {
            QSqlQuery nextQuery;
            nextQuery.prepare(
                "SELECT ID_PHASE, DATE_DEBUT, ETAT FROM ("
                "    SELECT ID_PHASE, DATE_DEBUT, ETAT "
                "    FROM PHASE "
                "    WHERE IDFABRICATION = :idFab AND ID_PHASE > :idPhase "
                "    ORDER BY ID_PHASE"
                ") WHERE ROWNUM = 1");
            nextQuery.bindValue(":idFab", idFabrication);
            nextQuery.bindValue(":idPhase", idPhase);

            if (nextQuery.exec() && nextQuery.next()) {
                const int nextPhaseId = nextQuery.value(0).toInt();
                const QDateTime nextStart = nextQuery.value(1).toDateTime();
                const QString nextEtat = nextQuery.value(2).toString().trimmed().toUpper();
                if (nextPhaseId > 0 && !nextStart.isValid()
                    && (nextEtat.isEmpty() || nextEtat == "NON_COMMENCE")) {
                    QSqlQuery startNextQuery;
                    startNextQuery.prepare(
                        "UPDATE PHASE "
                        "SET ETAT = 'EN_COURS', DATE_DEBUT = TO_DATE(:startNow, 'YYYY-MM-DD HH24:MI:SS') "
                        "WHERE ID_PHASE = :idPhase "
                        "AND (ETAT IS NULL OR UPPER(TRIM(ETAT)) = 'NON_COMMENCE') "
                        "AND DATE_DEBUT IS NULL");
                    startNextQuery.bindValue(":startNow", nowStr);
                    startNextQuery.bindValue(":idPhase", nextPhaseId);
                    if (!startNextQuery.exec()) {
                        ok = false;
                        if (errorMessage) {
                            *errorMessage = startNextQuery.lastError().text();
                        }
                    }
                }
            } else if (nextQuery.lastError().isValid()) {
                ok = false;
                if (errorMessage) {
                    *errorMessage = nextQuery.lastError().text();
                }
            }
        }
    } else {
        QSqlQuery startQuery;
        startQuery.prepare(
            "UPDATE PHASE "
            "SET ETAT = 'EN_COURS', "
            "    DATE_DEBUT = CASE WHEN DATE_DEBUT IS NULL THEN TO_DATE(:startNow, 'YYYY-MM-DD HH24:MI:SS') ELSE DATE_DEBUT END "
            "WHERE ID_PHASE = :idPhase");
        startQuery.bindValue(":startNow", nowStr);
        startQuery.bindValue(":idPhase", idPhase);
        ok = startQuery.exec() && startQuery.numRowsAffected() > 0;
        if (!ok && errorMessage) {
            *errorMessage = startQuery.lastError().text();
        }
    }

    if (hasTransaction) {
        if (ok) {
            if (!db.commit()) {
                if (errorMessage) {
                    *errorMessage = db.lastError().text();
                }
                return false;
            }
        } else {
            db.rollback();
        }
    }

    return ok;
}

QSqlQueryModel *Fabrication::afficherPhases(int idFabrication)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setHeaderData(0, Qt::Horizontal, "ID Phase");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Etat");

    if (idFabrication <= 0) {
        return model;
    }

    QSqlQuery query;
    query.prepare(
        "SELECT ID_PHASE, NOM_PHASE, NVL(ETAT, 'NON_COMMENCE') AS ETAT_PHASE "
        "FROM PHASE "
        "WHERE IDFABRICATION = :idFabrication "
        "ORDER BY ID_PHASE");
    query.bindValue(":idFabrication", idFabrication);
    if (!query.exec()) {
        qDebug() << "Erreur Afficher Phases:" << query.lastError().text();
        return model;
    }

    model->setQuery(std::move(query));
    if (model->lastError().isValid()) {
        qDebug() << "Erreur Modele Phases:" << model->lastError().text();
    }
    return model;
}

// ============================================================
//  CRUD - Ajouter (CREATE)
// ============================================================

bool Fabrication::ajouter()
{
    lastErrMsg.clear();

    // ------ Contrôle de saisie ------
    if (!validerDateDebut(date_debut)) {
        lastErrMsg = "Date de début invalide.";
        return false;
    }
    if (!validerDateFinPrevue(date_debut, date_fin_prevue)) {
        lastErrMsg = "Date de fin prévue invalide (doit être >= date de début).";
        return false;
    }
    if (!validerNbPersonnes(nb_personnes)) {
        lastErrMsg = "Nombre de personnes invalide (entre 1 et 1000).";
        return false;
    }
    if (!validerEtat(etat_fabrication)) {
        lastErrMsg = "État de fabrication invalide.";
        return false;
    }
    if (!validerIdModele(idModele)) {
        lastErrMsg = "ID Modèle invalide.";
        return false;
    }
    // ------ Auto-generate ID ------
    QSqlQuery queryId;
    queryId.prepare("SELECT NVL(MAX(IDFABRICATION), 0) + 1 FROM FABRICATION");
    if (!queryId.exec() || !queryId.next()) {
        lastErrMsg = "Impossible de générer l'ID automatiquement.";
        return false;
    }
    int autoId = queryId.value(0).toInt();

    // ------ Requête SQL ------
    const bool hasAngleColumn = fabricationHasAngleColumn();
    QSqlQuery query;
    if (hasAngleColumn) {
        query.prepare(
            "INSERT INTO FABRICATION "
            "(IDFABRICATION, DATE_DEBUT, DATE_FIN_PREVUE, DATE_FIN_REELLE, "
            " NB_PERSONNES, ETAT_FABRICATION, IDMODELE, ANGLE) "
            "VALUES "
            "(:idFabrication, :date_debut, :date_fin_prevue, :date_fin_reelle, "
            " :nb_personnes, :etat_fabrication, :idModele, :angle)"
        );
    } else {
        query.prepare(
            "INSERT INTO FABRICATION "
            "(IDFABRICATION, DATE_DEBUT, DATE_FIN_PREVUE, DATE_FIN_REELLE, "
            " NB_PERSONNES, ETAT_FABRICATION, IDMODELE) "
            "VALUES "
            "(:idFabrication, :date_debut, :date_fin_prevue, :date_fin_reelle, "
            " :nb_personnes, :etat_fabrication, :idModele)"
        );
    }

    query.bindValue(":idFabrication",   autoId);
    query.bindValue(":date_debut",      date_debut);
    query.bindValue(":date_fin_prevue", date_fin_prevue);
    query.bindValue(":date_fin_reelle",
                    date_fin_reelle.isValid()
                        ? QVariant(date_fin_reelle)
                        : QVariant(QMetaType::fromType<QDate>()));
    query.bindValue(":nb_personnes",    nb_personnes);
    query.bindValue(":etat_fabrication", etat_fabrication.toUpper());
    query.bindValue(":idModele",        idModele);
    if (hasAngleColumn) {
        query.bindValue(":angle", angle);
    }

    if (!query.exec()) {
        lastErrMsg = query.lastError().text();
        qDebug() << "Erreur Ajouter Fabrication:" << lastErrMsg;
        return false;
    }
    idFabrication = autoId;
    qDebug() << "Fabrication ajoutee avec succes (id:" << autoId << ")";
    return true;
}

// ============================================================
//  CRUD - Afficher (READ)
// ============================================================

QSqlQueryModel* Fabrication::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(
        "SELECT IDFABRICATION, "
        "TO_CHAR(DATE_DEBUT, 'DD/MM/YYYY') AS DATE_DEBUT, "
        "TO_CHAR(DATE_FIN_PREVUE, 'DD/MM/YYYY') AS DATE_FIN_PREVUE, "
        "TO_CHAR(DATE_FIN_REELLE, 'DD/MM/YYYY') AS DATE_FIN_REELLE, "
        "NB_PERSONNES, ETAT_FABRICATION, IDMODELE "
        "FROM FABRICATION "
        "ORDER BY IDFABRICATION"
    );

    if (model->lastError().isValid()) {
        qDebug() << "Erreur Afficher Fabrication:" << model->lastError().text();
    }

    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Date debut");
    model->setHeaderData(2, Qt::Horizontal, "Date fin prevue");
    model->setHeaderData(3, Qt::Horizontal, "Date fin reelle");
    model->setHeaderData(4, Qt::Horizontal, "Nombre de personnes");
    model->setHeaderData(5, Qt::Horizontal, "Etat");
    model->setHeaderData(6, Qt::Horizontal, "ID modele");

    return model;
}

// ============================================================
//  CRUD - Supprimer (DELETE)
// ============================================================

bool Fabrication::supprimer(int idFabrication)
{
    lastErrMsg.clear();

    if (idFabrication <= 0) {
        lastErrMsg = "ID Fabrication invalide.";
        return false;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM FABRICATION WHERE IDFABRICATION = :id");
    query.bindValue(":id", idFabrication);

    if (!query.exec()) {
        lastErrMsg = query.lastError().text();
        qDebug() << "Erreur Supprimer Fabrication:" << lastErrMsg;
        return false;
    }

    if (query.numRowsAffected() == 0) {
        lastErrMsg = "Aucune fabrication trouvée avec cet ID.";
        return false;
    }
    qDebug() << "Fabrication supprimee avec succes (id:" << idFabrication << ")";
    return true;
}

// ============================================================
//  CRUD - Modifier (UPDATE)
// ============================================================

bool Fabrication::modifier(int idFabrication)
{
    lastErrMsg.clear();

    if (idFabrication <= 0) {
        lastErrMsg = "ID Fabrication invalide.";
        return false;
    }

    // ------ Contrôle de saisie ------
    if (!validerDateDebut(date_debut)) {
        lastErrMsg = "Date de début invalide.";
        return false;
    }
    if (!validerDateFinPrevue(date_debut, date_fin_prevue)) {
        lastErrMsg = "Date de fin prévue invalide (doit être >= date de début).";
        return false;
    }
    if (!validerNbPersonnes(nb_personnes)) {
        lastErrMsg = "Nombre de personnes invalide (entre 1 et 1000).";
        return false;
    }
    if (!validerEtat(etat_fabrication)) {
        lastErrMsg = "État de fabrication invalide.";
        return false;
    }
    if (!validerIdModele(idModele)) {
        lastErrMsg = "ID Modèle invalide.";
        return false;
    }
    // ------ Requête SQL ------
    const bool hasAngleColumn = fabricationHasAngleColumn();
    QSqlQuery query;
    if (hasAngleColumn) {
        query.prepare(
            "UPDATE FABRICATION SET "
            "DATE_DEBUT = :date_debut, "
            "DATE_FIN_PREVUE = :date_fin_prevue, "
            "DATE_FIN_REELLE = :date_fin_reelle, "
            "NB_PERSONNES = :nb_personnes, "
            "ETAT_FABRICATION = :etat_fabrication, "
            "IDMODELE = :idModele, "
            "ANGLE = :angle "
            "WHERE IDFABRICATION = :idFabrication"
        );
    } else {
        query.prepare(
            "UPDATE FABRICATION SET "
            "DATE_DEBUT = :date_debut, "
            "DATE_FIN_PREVUE = :date_fin_prevue, "
            "DATE_FIN_REELLE = :date_fin_reelle, "
            "NB_PERSONNES = :nb_personnes, "
            "ETAT_FABRICATION = :etat_fabrication, "
            "IDMODELE = :idModele "
            "WHERE IDFABRICATION = :idFabrication"
        );
    }

    query.bindValue(":date_debut",       date_debut);
    query.bindValue(":date_fin_prevue",  date_fin_prevue);
    query.bindValue(":date_fin_reelle",
                    date_fin_reelle.isValid()
                        ? QVariant(date_fin_reelle)
                        : QVariant(QMetaType::fromType<QDate>()));
    query.bindValue(":nb_personnes",     nb_personnes);
    query.bindValue(":etat_fabrication", etat_fabrication.toUpper());
    query.bindValue(":idModele",         idModele);
    if (hasAngleColumn) {
        query.bindValue(":angle", angle);
    }
    query.bindValue(":idFabrication",    idFabrication);

    if (!query.exec()) {
        lastErrMsg = query.lastError().text();
        qDebug() << "Erreur Modifier Fabrication:" << lastErrMsg;
        return false;
    }

    if (query.numRowsAffected() == 0) {
        lastErrMsg = "Aucune fabrication trouvée avec cet ID.";
        return false;
    }
    qDebug() << "Fabrication modifiee avec succes (id:" << idFabrication << ")";
    return true;
}

// ============================================================
//  Score d'anomalie
// ============================================================

int Fabrication::calculerScoreAnomalie(const QDate &dateDebut, const QDate &dateFinPrevue,
                                       const QDate &dateFinReelle, int nbPersonnes,
                                       const QString &etat, int idModele, double angle)
{
    int score = 100; // Score initial parfait

    // Vérifier date de début
    if (!dateDebut.isValid() || dateDebut > QDate::currentDate()) {
        score -= 15; // Date invalide ou future
    }

    // Vérifier date de fin prévue
    if (!dateFinPrevue.isValid() || dateFinPrevue < dateDebut) {
        score -= 20; // Date invalide ou ant?rieure ? d?but
    }

    // Vérifier date de fin réelle
    if (dateFinReelle.isValid()) {
        if (dateFinReelle < dateDebut) {
            score -= 25; // Fin avant d?but
        } else if (dateFinReelle > dateFinPrevue.addDays(7)) {
            score -= 20; // Retard important
        }
    }

    // Vérifier nombre de personnes
    if (nbPersonnes < 1 || nbPersonnes > 20) {
        score -= 10; // Nombre invalide
    }

    // Vérifier état
    QString etatNorm = normaliserEtat(etat);
    if (etatNorm == "ANNULEE") {
        score -= 50; // Annulation
    } else if (etatNorm == "EN_ATTENTE" && dateDebut < QDate::currentDate().addDays(-30)) {
        score -= 10; // En attente trop longtemps
    }

    // Vérifier ID modèle
    if (idModele <= 0) {
        score -= 30; // Mod?le invalide
    }

    // Vérifier l'angle de fabrication
    if (qFabs(angle - kReferenceAngleDeg) >= kAngleToleranceDeg) {
        score -= kAnglePenalty;
    }

    // Score minimum 0
    return qMax(0, score);
}

// ============================================================
//  Explication du score d'anomalie
// ============================================================

QString Fabrication::obtenirExplicationScore(const QDate &dateDebut, const QDate &dateFinPrevue,
                                             const QDate &dateFinReelle, int nbPersonnes,
                                             const QString &etat, int idModele, double angle)
{
    QString explication = "Analyse des critères d'anomalie :\n\n";
    int penalites = 0;

    // Vérifier date de début
    if (!dateDebut.isValid() || dateDebut > QDate::currentDate()) {
        explication += "❌ Date de début : invalide ou future (-15 pts)\n";
        penalites += 15;
    } else {
        explication += "✓ Date de début : valide\n";
    }

    // Vérifier date de fin prévue
    if (!dateFinPrevue.isValid() || dateFinPrevue < dateDebut) {
        explication += "❌ Date de fin prévue : invalide ou antérieure au début (-20 pts)\n";
        penalites += 20;
    } else {
        explication += "✓ Date de fin prévue : cohérente\n";
    }

    // Vérifier date de fin réelle
    if (dateFinReelle.isValid()) {
        if (dateFinReelle < dateDebut) {
            explication += "❌ Date de fin réelle : antérieure au début (-25 pts)\n";
            penalites += 25;
        } else if (dateFinReelle > dateFinPrevue.addDays(7)) {
            explication += "❌ Retard important : dépassement > 7 jours (-20 pts)\n";
            penalites += 20;
        } else {
            explication += "✓ Date de fin réelle : conforme\n";
        }
    } else {
        explication += "ℹ Date de fin réelle : non renseignée\n";
    }

    // Vérifier nombre de personnes
    if (nbPersonnes < 1 || nbPersonnes > 20) {
        explication += "❌ Nombre de personnes : invalide (< 1 ou > 20) (-10 pts)\n";
        penalites += 10;
    } else {
        explication += QString("✓ Nombre de personnes : %1 (valide)\n").arg(nbPersonnes);
    }

    // Vérifier état
    QString etatNorm = normaliserEtat(etat);
    if (etatNorm == "ANNULEE") {
        explication += "❌ État : ANNULEE (-50 pts)\n";
        penalites += 50;
    } else if (etatNorm == "EN_ATTENTE" && dateDebut < QDate::currentDate().addDays(-30)) {
        explication += "❌ En attente depuis > 30 jours (-10 pts)\n";
        penalites += 10;
    } else {
        explication += QString("✓ État : %1\n").arg(etatNorm);
    }

    // Vérifier ID modèle
    if (idModele <= 0) {
        explication += "❌ Modèle : invalide (-30 pts)\n";
        penalites += 30;
    } else {
        explication += QString("✓ Modèle : %1\n").arg(idModele);
    }

    // Vérifier l'angle de fabrication
    const double angleDelta = qFabs(angle - kReferenceAngleDeg);
    if (angleDelta >= kAngleToleranceDeg) {
        explication += QString("❌ Angle de fabrication : %1° (écart %2°, tolérance ±%3° autour de 90°) (-%4 pts)\n")
                           .arg(angle, 0, 'f', 2)
                           .arg(angleDelta, 0, 'f', 2)
                           .arg(kAngleToleranceDeg, 0, 'f', 0)
                           .arg(kAnglePenalty);
        penalites += kAnglePenalty;
    } else {
        explication += QString("✓ Angle de fabrication : %1° (dans la tolérance ±%2°)\n")
                           .arg(angle, 0, 'f', 2)
                           .arg(kAngleToleranceDeg, 0, 'f', 0);
    }

    explication += "\n";
    int scoreCalcule = 100 - penalites;
    scoreCalcule = qMax(0, scoreCalcule);
    explication += QString("Score final : %1/100").arg(scoreCalcule);

    return explication;
}

bool Fabrication::mettreAJourTemperatureDetectee(int idFabrication, double temperatureCelsius,
                                                 QString *errorMessage)
{
    if (idFabrication <= 0) {
        if (errorMessage) {
            *errorMessage = QStringLiteral("ID fabrication invalide.");
        }
        return false;
    }
    if (!fabricationHasTemperatureColumn()) {
        if (errorMessage) {
            *errorMessage = QStringLiteral("Colonne TEMPERATURE_DETECTEE introuvable (mettre a jour le schema Oracle).");
        }
        return false;
    }

    QSqlQuery query;
    query.prepare(QStringLiteral("UPDATE FABRICATION SET TEMPERATURE_DETECTEE = :temp WHERE IDFABRICATION = :id"));
    query.bindValue(QStringLiteral(":temp"), temperatureCelsius);
    query.bindValue(QStringLiteral(":id"), idFabrication);

    if (!query.exec()) {
        if (errorMessage) {
            *errorMessage = query.lastError().text();
        }
        return false;
    }
    return true;
}

double Fabrication::lireTemperatureDetecteeDepuisDb(int idFabrication, bool *ok)
{
    if (ok) {
        *ok = false;
    }
    if (idFabrication <= 0 || !fabricationHasTemperatureColumn()) {
        return 0.0;
    }

    QSqlQuery query;
    query.prepare(QStringLiteral("SELECT TEMPERATURE_DETECTEE FROM FABRICATION WHERE IDFABRICATION = :id"));
    query.bindValue(QStringLiteral(":id"), idFabrication);
    if (!query.exec() || !query.next()) {
        return 0.0;
    }

    const QVariant v = query.value(0);
    if (v.isNull()) {
        return 0.0;
    }
    if (ok) {
        *ok = true;
    }
    return v.toDouble();
}
