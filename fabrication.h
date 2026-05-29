#ifndef FABRICATION_H
#define FABRICATION_H

#include <QString>
#include <QStringList>
#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Fabrication
{
public:
    // --- Constructors ---
    Fabrication();
    Fabrication(int idFabrication, const QDate &date_debut,
                const QDate &date_fin_prevue, const QDate &date_fin_reelle,
                int nb_personnes, const QString &etat_fabrication, int idModele, double angle = 90.0);

    // --- Getters ---
    int     getIdFabrication()   const;
    QDate   getDateDebut()       const;
    QDate   getDateFinPrevue()   const;
    QDate   getDateFinReelle()   const;
    int     getNbPersonnes()     const;
    QString getEtatFabrication() const;
    int     getIdModele()        const;
    double  getAngle()           const;

    // --- Setters ---
    void setIdFabrication(int id);
    void setDateDebut(const QDate &date);
    void setDateFinPrevue(const QDate &date);
    void setDateFinReelle(const QDate &date);
    void setNbPersonnes(int nb);
    void setEtatFabrication(const QString &etat);
    void setIdModele(int idModele);
    void setAngle(double angle);

    // --- CRUD ---
    bool            ajouter();                       // CREATE
    QSqlQueryModel* afficher();                      // READ
    bool            supprimer(int idFabrication);    // DELETE
    bool            modifier(int idFabrication);     // UPDATE

    // --- Last error ---
    QString getLastError() const;

    // --- Input validation ---
    static bool validerDateDebut(const QDate &date);
    static bool validerDateFinPrevue(const QDate &dateDebut, const QDate &dateFinPrevue);
    static bool validerNbPersonnes(int nb);
    static bool validerEtat(const QString &etat);
    static bool validerIdModele(int idModele);
    static QString normaliserEtat(const QString &etat);
    static bool validerTransitionEtat(const QString &ancienEtat, const QString &nouvelEtat);

    // Returns a list of allowed states
    static QStringList etatsAutorises();
    static QString calculerRisqueRetard(const QDate &dateFinPrevue,
                                        const QDate &dateFinReelle,
                                        const QString &etatFabrication);

    // Advanced business features around PHASE
    static double calculerProgressionPhases(int idFabrication,
                                            int *totalPhases = nullptr,
                                            int *donePhases = nullptr,
                                            int *inProgressPhases = nullptr,
                                            QString *errorMessage = nullptr);
    static bool genererPhasesParDefaut(int idFabrication, QString *errorMessage = nullptr);
    static bool avancerPhase(int idPhase, QString *errorMessage = nullptr);
    static QSqlQueryModel *afficherPhases(int idFabrication);

    // --- Score d'anomalie ---
    static int calculerScoreAnomalie(const QDate &dateDebut, const QDate &dateFinPrevue,
                                     const QDate &dateFinReelle, int nbPersonnes,
                                     const QString &etat, int idModele, double angle = 90.0);
    static QString obtenirExplicationScore(const QDate &dateDebut, const QDate &dateFinPrevue,
                                          const QDate &dateFinReelle, int nbPersonnes,
                                          const QString &etat, int idModele, double angle = 90.0);

    /** Scenario DHT : colonne TEMPERATURE_DETECTEE (Oracle). */
    static bool mettreAJourTemperatureDetectee(int idFabrication, double temperatureCelsius,
                                                 QString *errorMessage = nullptr);
    static double lireTemperatureDetecteeDepuisDb(int idFabrication, bool *ok = nullptr);

private:
    int     idFabrication;
    QDate   date_debut;
    QDate   date_fin_prevue;
    QDate   date_fin_reelle;
    int     nb_personnes;
    QString etat_fabrication;
    int     idModele;
    double  angle;  // angle de fabrication en degrés

    QString lastErrMsg;  // stores the last error message
};

#endif // FABRICATION_H
