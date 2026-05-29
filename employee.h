#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <QString>
#include <QDate>
#include <QDateTime>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QVector>

struct EmployeeKnnProfile
{
    int idEmployee = 0;
    QString nom;
    QString prenom;
    QString role;
    double classificationX = 0.0;
    double classificationY = 0.0;
    int idFabrication = 0;
};

struct EmployeeKnnRecommendation
{
    EmployeeKnnProfile employee;
    double distance = 0.0;
    double competenceScore = 0.0;
    QString qualityClass;
};

class Employee
{
public:
    // --- Constructors ---
    Employee();
    Employee(int idEmployee, const QString &CIN, const QString &mail,
             const QString &password, const QString &nom, const QString &prenom,
             const QDate &date_naissance, const QString &face_id,
             const QDateTime &face_ts, const QString &role,
             double classification_x, double classification_y);

    // --- Getters ---
    int getIdEmployee() const;
    QString getCIN() const;
    QString getMail() const;
    QString getPassword() const;
    QString getNom() const;
    QString getPrenom() const;
    QDate getDateNaissance() const;
    QString getTelephone() const;
    QString getPhotoPath() const;
    QDate getDateEmbauche() const;
    QString getTypeContrat() const;
    double getSalaireBase() const;
    QString getStatut() const;
    QString getNiveau() const;
    QString getFaceId() const;
    QDateTime getFaceTs() const;
    QString getRole() const;
    double getClassificationX() const;
    double getClassificationY() const;

    // --- Setters ---
    void setIdEmployee(int id);
    void setCIN(const QString &cin);
    void setMail(const QString &mail);
    void setPassword(const QString &password);
    void setNom(const QString &nom);
    void setPrenom(const QString &prenom);
    void setDateNaissance(const QDate &date);
    void setTelephone(const QString &telephone);
    void setPhotoPath(const QString &photoPath);
    void setDateEmbauche(const QDate &dateEmbauche);
    void setTypeContrat(const QString &typeContrat);
    void setSalaireBase(double salaireBase);
    void setStatut(const QString &statut);
    void setNiveau(const QString &niveau);
    void setFaceId(const QString &faceId);
    void setFaceTs(const QDateTime &faceTs);
    void setRole(const QString &role);
    void setClassificationX(double x);
    void setClassificationY(double y);

    // --- CRUD ---
    bool ajouter();                          // CREATE
    QSqlQueryModel* afficher(bool includeArchived = false);              // READ
    bool supprimer(int idEmployee);          // ARCHIVE (soft delete)
    bool supprimerDefinitivement(int idEmployee); // PURGE (hard delete)
    bool modifier(int idEmployee);           // UPDATE

    // --- Last error (set by ajouter/modifier/supprimer on failure) ---
    QString getLastError() const;

    // --- Input validation ---
    static bool validerCIN(const QString &cin);
    static bool validerMail(const QString &mail);
    static bool validerPassword(const QString &password);
    static bool validerNom(const QString &nom);
    static bool validerPrenom(const QString &prenom);
    static bool validerRole(const QString &role);
    static bool validerDateNaissance(const QDate &date);

    // Returns a list of allowed roles
    static QStringList rolesAutorises();
    static void setAuditActor(const QString &actor);

    // --- Fabrication assignment ---
    static bool assignerAFabrication(int idEmployee, int idFabrication);
    static bool desaffecter(int idEmployee);
    static bool recomputeClassification2D(int idEmployee);
    static bool getFaceIdByEmployeeId(int idEmployee, QString &faceIdOut);
    static bool proposerTopEmployePourFabrication(int idFabrication,
                                                  const QString &roleFilter,
                                                  EmployeeKnnRecommendation &outRecommendation);
    static bool exporterCsv(const QString &filePath, int &rowCountOut, QString &errorOut);
    static bool importerCsv(const QString &filePath,
                            int &successCountOut,
                            int &failCountOut,
                            QString &errorOut);

    // --- KNN (classification des employes) ---
    static QVector<EmployeeKnnProfile> chargerProfilsKnn(const QString &roleFilter = QString());
    static double calculerDistanceEuclidienne(double x1, double y1, double x2, double y2);
    static bool pointTacheDepuisFabrication(int idFabrication, double &taskX, double &taskY);
    static bool pointTacheDepuisPhase(int idPhase, double &taskX, double &taskY);
    static QVector<EmployeeKnnRecommendation> recommanderEmployesPourTache(double taskX,
                                                                           double taskY,
                                                                           int k,
                                                                           const QString &roleFilter = QString());
    static QVector<double> calculerProbabilitesKnn(const QVector<EmployeeKnnRecommendation> &recommendations);
    static bool appliquerFeedbackKnn(int idEmployee, double taskX, double taskY, bool confirme);
    static bool entrainerModeleKnn(int &updatedCountOut, QString &errorOut);
    static bool sauvegarderClasseEmploye(int idEmployee, const QString &classe, double score = -1.0);

private:
    int idEmployee;
    QString CIN;
    QString mail;
    QString password;
    QString nom;
    QString prenom;
    QDate date_naissance;
    QString telephone;
    QString photo_path;
    QDate date_embauche;
    QString type_contrat;
    double salaire_base;
    QString statut;
    QString niveau;
    QString face_id;
    QDateTime face_ts;
    QString role;
    double classification_x;
    double classification_y;

    QString lastErrMsg;  // stores the last error message
};

#endif // EMPLOYEE_H
