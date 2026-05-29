#ifndef MODELE_H
#define MODELE_H

#include <QSqlQueryModel>
#include <QString>

class Modele
{
public:
    Modele();
    Modele(int idModele, const QString &nomModele, const QString &categorie,
           const QString &dimensions, const QString &type, double prixBase,
           const QString &description, double coutEstime);

    int getIdModele() const;
    QString getNomModele() const;
    QString getCategorie() const;
    QString getDimensions() const;
    QString getType() const;
    double getPrixBase() const;
    QString getDescription() const;
    double getCoutEstime() const;

    void setIdModele(int id);
    void setNomModele(const QString &nom);
    void setCategorie(const QString &categorie);
    void setDimensions(const QString &dimensions);
    void setType(const QString &type);
    void setPrixBase(double prix);
    void setDescription(const QString &description);
    void setCoutEstime(double cout);

    bool ajouter();
    QSqlQueryModel *afficher();
    bool supprimer(int idModele);
    bool modifier(int idModele);

    QString getLastError() const;

    static bool validerId(int id);
    static bool validerNomModele(const QString &nom);
    static bool validerCategorie(const QString &categorie);
    static bool validerDimensions(const QString &dimensions);
    static bool validerType(const QString &type);
    static bool validerPrix(double prix);
    static bool validerCout(double cout);

private:
    int idModele;
    QString nomModele;
    QString categorie;
    QString dimensions;
    QString type;
    double prixBase;
    QString description;
    double coutEstime;
    QString lastErrMsg;
};

#endif // MODELE_H
