#ifndef PHASE_H
#define PHASE_H

#include <QString>
#include <QDateTime>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Phase
{
public:
    // --- Constructors ---
    Phase();
    Phase(int idPhase, const QString &nomPhase, const QString &competenceRequise,
          const QString &etat, const QDateTime &dateDebut, const QDateTime &dateFinPrevue,
          const QDateTime &dateFinReelle, int idFabrication);

    // --- Getters ---
    int         getIdPhase()             const;
    QString     getNomPhase()            const;
    QString     getCompetenceRequise()   const;
    QString     getEtat()                const;
    QDateTime   getDateDebut()           const;
    QDateTime   getDateFinPrevue()       const;
    QDateTime   getDateFinReelle()       const;
    int         getIdFabrication()       const;

    // --- Setters ---
    void setIdPhase(int id);
    void setNomPhase(const QString &nom);
    void setCompetenceRequise(const QString &comp);
    void setEtat(const QString &etat);
    void setDateDebut(const QDateTime &date);
    void setDateFinPrevue(const QDateTime &date);
    void setDateFinReelle(const QDateTime &date);
    void setIdFabrication(int id);

    // --- CRUD ---
    bool            ajouter();                       // CREATE
    static QSqlQueryModel* afficher(int idFabrication = 0); // READ
    bool            supprimer(int idPhase);          // DELETE
    bool            modifier(int idPhase);           // UPDATE

    // --- Last error ---
    QString getLastError() const;

    // --- Input validation ---
    static bool validerNomPhase(const QString &nom);
    static bool validerCompetenceRequise(const QString &comp);
    static bool validerEtat(const QString &etat);
    static bool validerIdFabrication(int id);
    static QString normaliserEtat(const QString &etat);
    static QString normaliserCompetence(const QString &comp);

    // Returns a list of allowed states
    static QStringList etatsAutorises();
    static QStringList competencesAutorisees();

private:
    int     idPhase;
    QString nomPhase;
    QString competenceRequise;
    QString etat;
    QDateTime dateDebut;
    QDateTime dateFinPrevue;
    QDateTime dateFinReelle;
    int     idFabrication;

    QString lastErrMsg;  // stores the last error message
};

#endif // PHASE_H
