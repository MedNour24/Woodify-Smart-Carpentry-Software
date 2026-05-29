#include "phase.h"

#include <QDebug>
#include <QMetaType>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QStringList>
#include <utility>

// ============================================================
//  Constructors
// ============================================================

Phase::Phase()
    : idPhase(0),
      idFabrication(0)
{}

Phase::Phase(int idPhase, const QString &nomPhase, const QString &competenceRequise,
             const QString &etat, const QDateTime &dateDebut, const QDateTime &dateFinPrevue,
             const QDateTime &dateFinReelle, int idFabrication)
    : idPhase(idPhase),
      nomPhase(nomPhase),
      competenceRequise(competenceRequise),
      etat(etat),
      dateDebut(dateDebut),
      dateFinPrevue(dateFinPrevue),
      dateFinReelle(dateFinReelle),
      idFabrication(idFabrication)
{}

// ============================================================
//  Getters
// ============================================================

int     Phase::getIdPhase()             const { return idPhase; }
QString Phase::getNomPhase()            const { return nomPhase; }
QString Phase::getCompetenceRequise()   const { return competenceRequise; }
QString Phase::getEtat()                const { return etat; }
QDateTime Phase::getDateDebut()         const { return dateDebut; }
QDateTime Phase::getDateFinPrevue()     const { return dateFinPrevue; }
QDateTime Phase::getDateFinReelle()     const { return dateFinReelle; }
int     Phase::getIdFabrication()       const { return idFabrication; }
QString Phase::getLastError()           const { return lastErrMsg; }

// ============================================================
//  Setters
// ============================================================

void Phase::setIdPhase(int id)                      { idPhase = id; }
void Phase::setNomPhase(const QString &nom)         { nomPhase = nom; }
void Phase::setCompetenceRequise(const QString &comp) { competenceRequise = comp; }
void Phase::setEtat(const QString &e)               { etat = e; }
void Phase::setDateDebut(const QDateTime &date)     { dateDebut = date; }
void Phase::setDateFinPrevue(const QDateTime &date) { dateFinPrevue = date; }
void Phase::setDateFinReelle(const QDateTime &date) { dateFinReelle = date; }
void Phase::setIdFabrication(int id)                { idFabrication = id; }

// ============================================================
//  Allowed states and competencies
// ============================================================

QStringList Phase::etatsAutorises()
{
    return {"NON_COMMENCE", "EN_COURS", "TERMINE_BON", "TERMINE_ACCEPTABLE", "TERMINE_MAUVAIS"};
}

QStringList Phase::competencesAutorisees()
{
    return {"CNC", "Joinery", "Finishing", "Assembly", "Milling", "Design", "Quality Control", "Planification", "Gestion"};
}

// ============================================================
//  Input Validation
// ============================================================

bool Phase::validerNomPhase(const QString &nom)
{
    return nom.length() <= 100;
}

bool Phase::validerCompetenceRequise(const QString &comp)
{
    return competencesAutorisees().contains(normaliserCompetence(comp));
}

bool Phase::validerEtat(const QString &etat)
{
    return etatsAutorises().contains(normaliserEtat(etat));
}

bool Phase::validerIdFabrication(int id)
{
    return id > 0;
}

QString Phase::normaliserEtat(const QString &etat)
{
    return etat.trimmed().toUpper();
}

QString Phase::normaliserCompetence(const QString &comp)
{
    return comp.trimmed();
}

// ============================================================
//  CRUD Operations
// ============================================================

bool Phase::ajouter()
{
    lastErrMsg.clear();

    if (!validerNomPhase(nomPhase)) {
        lastErrMsg = "Nom de phase invalide.";
        return false;
    }
    if (!validerCompetenceRequise(competenceRequise)) {
        lastErrMsg = "Competence requise invalide.";
        return false;
    }
    if (!validerEtat(etat)) {
        lastErrMsg = "Etat invalide.";
        return false;
    }
    if (!validerIdFabrication(idFabrication)) {
        lastErrMsg = "ID Fabrication invalide.";
        return false;
    }

    // Get next ID by finding max and incrementing
    QSqlQuery maxQuery;
    maxQuery.prepare("SELECT NVL(MAX(id_phase), 0) FROM PHASE");
    int nextId = 1;
    if (maxQuery.exec() && maxQuery.next()) {
        nextId = maxQuery.value(0).toInt() + 1;
    }

    const QString debutStr = dateDebut.isValid() ? dateDebut.toString("yyyy-MM-dd HH:mm:ss") : QString();
    const QString finPrevueStr = dateFinPrevue.isValid() ? dateFinPrevue.toString("yyyy-MM-dd HH:mm:ss") : QString();
    const QString finReelleStr = dateFinReelle.isValid() ? dateFinReelle.toString("yyyy-MM-dd HH:mm:ss") : QString();

    QSqlQuery query;
    query.prepare(
        "INSERT INTO PHASE (id_phase, nom_phase, competence_requise, etat, date_debut, date_fin_prevue, date_fin_reelle, idFabrication) "
        "VALUES (:idPhase, :nom, :comp, :etat, "
        "TO_DATE(:dateDebut, 'YYYY-MM-DD HH24:MI:SS'), "
        "TO_DATE(:dateFinPrevue, 'YYYY-MM-DD HH24:MI:SS'), "
        "TO_DATE(:dateFinReelle, 'YYYY-MM-DD HH24:MI:SS'), "
        ":idFabrication)");
    query.bindValue(":idPhase", nextId);
    query.bindValue(":nom", nomPhase);
    query.bindValue(":comp", competenceRequise);
    query.bindValue(":etat", normaliserEtat(etat));
    query.bindValue(":dateDebut", debutStr.isEmpty() ? QVariant(QMetaType::fromType<QString>()) : QVariant(debutStr));
    query.bindValue(":dateFinPrevue", finPrevueStr.isEmpty() ? QVariant(QMetaType::fromType<QString>()) : QVariant(finPrevueStr));
    query.bindValue(":dateFinReelle", finReelleStr.isEmpty() ? QVariant(QMetaType::fromType<QString>()) : QVariant(finReelleStr));
    query.bindValue(":idFabrication", idFabrication);

    if (!query.exec()) {
        lastErrMsg = "Erreur lors de l'ajout: " + query.lastError().text();
        return false;
    }

    return true;
}

QSqlQueryModel *Phase::afficher(int idFabrication)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QString queryStr = "SELECT id_phase, nom_phase, competence_requise, etat, "
                       "TO_CHAR(date_debut, 'DD/MM/YYYY HH24:MI') as date_debut, "
                       "TO_CHAR(date_fin_prevue, 'DD/MM/YYYY HH24:MI') as date_fin_prevue, "
                       "TO_CHAR(date_fin_reelle, 'DD/MM/YYYY HH24:MI') as date_fin_reelle "
                       "FROM PHASE";

    if (idFabrication > 0) {
        queryStr += " WHERE idFabrication = :idFab";
    }

    queryStr += " ORDER BY id_phase";

    QSqlQuery query;
    query.prepare(queryStr);
    if (idFabrication > 0) {
        query.bindValue(":idFab", idFabrication);
    }

    if (!query.exec()) {
        qDebug() << "Erreur affichage phases:" << query.lastError().text();
        delete model;
        return nullptr;
    }

    model->setQuery(std::move(query));
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Nom Phase");
    model->setHeaderData(2, Qt::Horizontal, "Competence");
    model->setHeaderData(3, Qt::Horizontal, "Etat");
    model->setHeaderData(4, Qt::Horizontal, "Date Debut");
    model->setHeaderData(5, Qt::Horizontal, "Date Fin Prevue");
    model->setHeaderData(6, Qt::Horizontal, "Date Fin Reelle");

    return model;
}

bool Phase::supprimer(int idPhase)
{
    lastErrMsg.clear();

    if (idPhase <= 0) {
        lastErrMsg = "ID Phase invalide.";
        return false;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM PHASE WHERE id_phase = :id");
    query.bindValue(":id", idPhase);

    if (!query.exec()) {
        lastErrMsg = "Erreur lors de la suppression: " + query.lastError().text();
        return false;
    }

    if (query.numRowsAffected() == 0) {
        lastErrMsg = "Aucune phase trouvee avec cet ID.";
        return false;
    }

    return true;
}

bool Phase::modifier(int idPhase)
{
    lastErrMsg.clear();

    if (idPhase <= 0) {
        lastErrMsg = "ID Phase invalide.";
        return false;
    }
    if (!validerNomPhase(nomPhase)) {
        lastErrMsg = "Nom de phase invalide.";
        return false;
    }
    if (!validerCompetenceRequise(competenceRequise)) {
        lastErrMsg = "Competence requise invalide.";
        return false;
    }
    if (!validerEtat(etat)) {
        lastErrMsg = "Etat invalide.";
        return false;
    }
    if (!validerIdFabrication(idFabrication)) {
        lastErrMsg = "ID Fabrication invalide.";
        return false;
    }

    const QString debutStr = dateDebut.isValid() ? dateDebut.toString("yyyy-MM-dd HH:mm:ss") : QString();
    const QString finPrevueStr = dateFinPrevue.isValid() ? dateFinPrevue.toString("yyyy-MM-dd HH:mm:ss") : QString();
    const QString finReelleStr = dateFinReelle.isValid() ? dateFinReelle.toString("yyyy-MM-dd HH:mm:ss") : QString();

    QSqlQuery query;
    query.prepare(
        "UPDATE PHASE SET "
        "nom_phase = :nom, "
        "competence_requise = :comp, "
        "etat = :etat, "
        "date_debut = TO_DATE(:dateDebut, 'YYYY-MM-DD HH24:MI:SS'), "
        "date_fin_prevue = TO_DATE(:dateFinPrevue, 'YYYY-MM-DD HH24:MI:SS'), "
        "date_fin_reelle = TO_DATE(:dateFinReelle, 'YYYY-MM-DD HH24:MI:SS'), "
        "idFabrication = :idFabrication "
        "WHERE id_phase = :idPhase");
    query.bindValue(":nom", nomPhase);
    query.bindValue(":comp", competenceRequise);
    query.bindValue(":etat", normaliserEtat(etat));
    query.bindValue(":dateDebut", debutStr.isEmpty() ? QVariant(QMetaType::fromType<QString>()) : QVariant(debutStr));
    query.bindValue(":dateFinPrevue", finPrevueStr.isEmpty() ? QVariant(QMetaType::fromType<QString>()) : QVariant(finPrevueStr));
    query.bindValue(":dateFinReelle", finReelleStr.isEmpty() ? QVariant(QMetaType::fromType<QString>()) : QVariant(finReelleStr));
    query.bindValue(":idFabrication", idFabrication);
    query.bindValue(":idPhase", idPhase);

    if (!query.exec()) {
        lastErrMsg = "Erreur lors de la modification: " + query.lastError().text();
        return false;
    }

    if (query.numRowsAffected() == 0) {
        lastErrMsg = "Aucune phase trouvee avec cet ID.";
        return false;
    }

    return true;
}
