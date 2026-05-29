#ifndef BOIS_H
#define BOIS_H

#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QVector>

class Bois
{
public:
    // --- Constructors ---
    Bois();
    Bois(int idBois, const QString &nomBois, double longueur, double largeur, double epaisseur,
         int quantite, double prixUnitaire, const QString &fournisseur,
         const QString &resistance, double humiditeMax,
         const QString &usageRecommande, const QString &imageStock,
         const QString &imageAnalyse, int nombreNoeuds,
         const QString &classeQualite, const QDate &dateAnalyse);

    // --- Getters ---
    int getIdBois() const;
    QString getNomBois() const;
    double getLongueur() const;
    double getLargeur() const;
    double getEpaisseur() const;
    int getQuantite() const;
    double getPrixUnitaire() const;
    QString getFournisseur() const;
    QString getResistance() const;
    double getHumiditeMax() const;
    QString getUsageRecommande() const;
    QString getImageStock() const;
    QString getImageAnalyse() const;
    int getNombreNoeuds() const;
    QString getClasseQualite() const;
    QDate getDateAnalyse() const;

    // --- Setters ---
    void setIdBois(int id);
    void setNomBois(const QString &nom);
    void setLongueur(double l);
    void setLargeur(double l);
    void setEpaisseur(double e);
    void setQuantite(int q);
    void setPrixUnitaire(double p);
    void setFournisseur(const QString &f);
    void setResistance(const QString &r);
    void setHumiditeMax(double h);
    void setUsageRecommande(const QString &u);
    void setImageStock(const QString &img);
    void setImageAnalyse(const QString &img);
    void setNombreNoeuds(int n);
    void setClasseQualite(const QString &c);
    void setDateAnalyse(const QDate &d);

    // --- CRUD ---
    bool ajouter();                    // CREATE
    QSqlQueryModel* afficher();        // READ
    bool supprimer(int idBois);        // DELETE
    bool modifier(int idBois);         // UPDATE
    static bool ensureStorage(QString *errorMessage = nullptr);

    // --- Advanced Features ---
    QSqlQueryModel* filtrer(const QString &nom, double minPrix, double maxPrix, const QString &fournisseur);
    
    // --- Statistics ---
    QMap<QString, int> getQualityDistribution() const;
    QVector<QPair<QString, int>> getStockAnalysis() const;
    QVector<QPair<QString, double>> getPriceDistribution() const;
    QMap<QString, int> getSupplierDistribution() const;
    double getTotalStockValue() const;
    int getTotalStockQuantity() const;
    QString getTopSpecies() const;
    
    // --- AI & Recommendation ---
    struct RecommendationResult {
        int idBois;
        QString nomBois;
        double prixUnitaire;
        QString classeQualite;
        double score;
        QString reason;
        QString imageStock;
        double valueScore; 
    };
    struct DetectedNodeInfo {
        double positionX = 0.0;
        double positionY = 0.0;
        double diametreMm = 0.0;
        double confiance = 0.0;
        QString typeNoeud;
        QString severite;
    };
    static QList<RecommendationResult> recommanderBois(double budgetMax, const QString &usageRequis, const QString &resistanceRequise, double humiditeMaxRequis);
    static bool estImageBois(const QString &imagePath, int idBois = -1, QString *diagnostic = nullptr);
    static QString analyserImageBois(const QString &imagePath, int idBois = -1);
    static QVector<DetectedNodeInfo> getDerniersNoeudsDetectes();
    bool enregistrerResultatsAnalyse(int id, int nbNoeuds, const QString &classe, const QString &imageAnalyse);


    // --- Last error ---
    QString getLastError() const;

    // --- Input validation ---
    static bool validerNomBois(const QString &nom);
    static bool validerLongueur(double l);
    static bool validerLargeur(double l);
    static bool validerEpaisseur(double e);
    static bool validerQuantite(int q);
    static bool validerPrixUnitaire(double p);
    static bool validerFournisseur(const QString &f);
    static bool validerResistance(const QString &r);
    static bool validerHumidite(double h);
    static bool validerUsage(const QString &u);

private:
    int idBois;
    QString nomBois;
    double longueur;
    double largeur;
    double epaisseur;
    int quantite;
    double prixUnitaire;
    QString fournisseur;
    QString resistance;
    double humiditeMax;
    QString usageRecommande;
    QString imageStock;
    QString imageAnalyse;
    int nombreNoeuds;
    QString classeQualite;
    QDate dateAnalyse;

    QString lastErrMsg;
};

#endif // BOIS_H
