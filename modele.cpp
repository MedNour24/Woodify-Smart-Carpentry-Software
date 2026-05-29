#include "modele.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

Modele::Modele()
    : idModele(0),
      prixBase(0.0),
      coutEstime(0.0)
{}

Modele::Modele(int idModele, const QString &nomModele, const QString &categorie,
               const QString &dimensions, const QString &type, double prixBase,
               const QString &description, double coutEstime)
    : idModele(idModele),
      nomModele(nomModele),
      categorie(categorie),
      dimensions(dimensions),
      type(type),
      prixBase(prixBase),
      description(description),
      coutEstime(coutEstime)
{}

int Modele::getIdModele() const { return idModele; }
QString Modele::getNomModele() const { return nomModele; }
QString Modele::getCategorie() const { return categorie; }
QString Modele::getDimensions() const { return dimensions; }
QString Modele::getType() const { return type; }
double Modele::getPrixBase() const { return prixBase; }
QString Modele::getDescription() const { return description; }
double Modele::getCoutEstime() const { return coutEstime; }

void Modele::setIdModele(int id) { idModele = id; }
void Modele::setNomModele(const QString &nom) { nomModele = nom; }
void Modele::setCategorie(const QString &value) { categorie = value; }
void Modele::setDimensions(const QString &value) { dimensions = value; }
void Modele::setType(const QString &value) { type = value; }
void Modele::setPrixBase(double prix) { prixBase = prix; }
void Modele::setDescription(const QString &value) { description = value; }
void Modele::setCoutEstime(double cout) { coutEstime = cout; }

QString Modele::getLastError() const { return lastErrMsg; }

bool Modele::validerId(int id)
{
    return id > 0;
}

bool Modele::validerNomModele(const QString &nom)
{
    const QString value = nom.trimmed();
    return !value.isEmpty() && value.length() <= 100;
}

bool Modele::validerCategorie(const QString &value)
{
    return value.trimmed().length() <= 50;
}

bool Modele::validerDimensions(const QString &value)
{
    return value.trimmed().length() <= 100;
}

bool Modele::validerType(const QString &value)
{
    return value.trimmed().length() <= 50;
}

bool Modele::validerPrix(double prix)
{
    return prix >= 0.0;
}

bool Modele::validerCout(double cout)
{
    return cout >= 0.0;
}

bool Modele::ajouter()
{
    lastErrMsg.clear();

    if (!validerNomModele(nomModele)) {
        lastErrMsg = "Nom de modele invalide (1-100 caracteres).";
        return false;
    }
    if (!validerCategorie(categorie)) {
        lastErrMsg = "Categorie invalide (max 50 caracteres).";
        return false;
    }
    if (!validerDimensions(dimensions)) {
        lastErrMsg = "Dimensions invalides (max 100 caracteres).";
        return false;
    }
    if (!validerType(type)) {
        lastErrMsg = "Type invalide (max 50 caracteres).";
        return false;
    }
    if (!validerPrix(prixBase)) {
        lastErrMsg = "Prix de base invalide (>= 0).";
        return false;
    }
    if (!validerCout(coutEstime)) {
        lastErrMsg = "Cout estime invalide (>= 0).";
        return false;
    }

    int finalId = idModele;
    if (finalId > 0) {
        QSqlQuery check;
        check.prepare("SELECT COUNT(*) FROM MODELE WHERE IDMODELE = :id");
        check.bindValue(":id", finalId);
        if (!check.exec() || !check.next()) {
            lastErrMsg = "Impossible de verifier l'ID.";
            return false;
        }
        if (check.value(0).toInt() > 0) {
            lastErrMsg = "ID modele deja utilise.";
            return false;
        }
    } else {
        QSqlQuery queryId;
        queryId.prepare("SELECT NVL(MAX(IDMODELE), 0) + 1 FROM MODELE");
        if (!queryId.exec() || !queryId.next()) {
            lastErrMsg = "Impossible de generer l'ID automatiquement.";
            return false;
        }
        finalId = queryId.value(0).toInt();
    }

    QSqlQuery query;
    query.prepare(
        "INSERT INTO MODELE "
        "(IDMODELE, NOM_MODELE, CATEGORIE, DIMENSIONS, TYPE, PRIX_BASE, DESCRIPTION, COUT_ESTIME) "
        "VALUES "
        "(:idModele, :nomModele, :categorie, :dimensions, :type, :prixBase, :description, :coutEstime)"
        );

    query.bindValue(":idModele", finalId);
    query.bindValue(":nomModele", nomModele.trimmed());
    query.bindValue(":categorie", categorie.trimmed());
    query.bindValue(":dimensions", dimensions.trimmed());
    query.bindValue(":type", type.trimmed());
    query.bindValue(":prixBase", prixBase);
    query.bindValue(":description", description.trimmed());
    query.bindValue(":coutEstime", coutEstime);

    if (!query.exec()) {
        lastErrMsg = query.lastError().text();
        qDebug() << "Erreur Ajouter Modele:" << lastErrMsg;
        return false;
    }

    idModele = finalId;
    return true;
}

QSqlQueryModel *Modele::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(
        "SELECT IDMODELE, NOM_MODELE, CATEGORIE, DIMENSIONS, TYPE, "
        "PRIX_BASE, DESCRIPTION, COUT_ESTIME "
        "FROM MODELE "
        "ORDER BY IDMODELE"
        );

    if (model->lastError().isValid()) {
        qDebug() << "Erreur Afficher Modele:" << model->lastError().text();
    }

    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Categorie");
    model->setHeaderData(3, Qt::Horizontal, "Dimensions");
    model->setHeaderData(4, Qt::Horizontal, "Type");
    model->setHeaderData(5, Qt::Horizontal, "Prix Base");
    model->setHeaderData(6, Qt::Horizontal, "Description");
    model->setHeaderData(7, Qt::Horizontal, "Cout Estime");

    return model;
}

bool Modele::supprimer(int idModele)
{
    lastErrMsg.clear();

    if (!validerId(idModele)) {
        lastErrMsg = "ID Modele invalide.";
        return false;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM MODELE WHERE IDMODELE = :id");
    query.bindValue(":id", idModele);

    if (!query.exec()) {
        lastErrMsg = query.lastError().text();
        qDebug() << "Erreur Supprimer Modele:" << lastErrMsg;
        return false;
    }

    return true;
}

bool Modele::modifier(int idModele)
{
    lastErrMsg.clear();

    if (!validerId(idModele)) {
        lastErrMsg = "ID Modele invalide.";
        return false;
    }
    if (!validerNomModele(nomModele)) {
        lastErrMsg = "Nom de modele invalide (1-100 caracteres).";
        return false;
    }
    if (!validerCategorie(categorie)) {
        lastErrMsg = "Categorie invalide (max 50 caracteres).";
        return false;
    }
    if (!validerDimensions(dimensions)) {
        lastErrMsg = "Dimensions invalides (max 100 caracteres).";
        return false;
    }
    if (!validerType(type)) {
        lastErrMsg = "Type invalide (max 50 caracteres).";
        return false;
    }
    if (!validerPrix(prixBase)) {
        lastErrMsg = "Prix de base invalide (>= 0).";
        return false;
    }
    if (!validerCout(coutEstime)) {
        lastErrMsg = "Cout estime invalide (>= 0).";
        return false;
    }

    QSqlQuery query;
    query.prepare(
        "UPDATE MODELE SET "
        "NOM_MODELE = :nomModele, "
        "CATEGORIE = :categorie, "
        "DIMENSIONS = :dimensions, "
        "TYPE = :type, "
        "PRIX_BASE = :prixBase, "
        "DESCRIPTION = :description, "
        "COUT_ESTIME = :coutEstime "
        "WHERE IDMODELE = :idModele"
        );

    query.bindValue(":nomModele", nomModele.trimmed());
    query.bindValue(":categorie", categorie.trimmed());
    query.bindValue(":dimensions", dimensions.trimmed());
    query.bindValue(":type", type.trimmed());
    query.bindValue(":prixBase", prixBase);
    query.bindValue(":description", description.trimmed());
    query.bindValue(":coutEstime", coutEstime);
    query.bindValue(":idModele", idModele);

    if (!query.exec()) {
        lastErrMsg = query.lastError().text();
        qDebug() << "Erreur Modifier Modele:" << lastErrMsg;
        return false;
    }

    return true;
}
