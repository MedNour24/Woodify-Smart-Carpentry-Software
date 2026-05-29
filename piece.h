#ifndef PIECE_H
#define PIECE_H

#include <QDate>
#include <QDateTime>
#include <QSqlQueryModel>
#include <QString>
#include <QStringList>
#include <QVariant>
#include <QVector>

enum class PieceStatus
{
    Draft,
    Published
};

enum class PieceWorkflowStage
{
    Empty,
    Draft,
    Published,
    NestingValidated,
    AtelierValidated
};

struct PieceDraft
{
    QString nom;
    QString code;
    int quantite = 0;
    QString matiere;
    double longueurMm = 0.0;
    double largeurMm = 0.0;
    double epaisseurMm = 0.0;
    QString sourceGeneration = "MANUEL";
    int ordreAffichage = 0;
};

struct PieceWorkflowState
{
    int modeleId = 0;
    int boisId = 0;
    PieceWorkflowStage currentStage = PieceWorkflowStage::Empty;
    bool dirty = false;
    QString stateJson;
    QDateTime publishedAt;
    QDateTime nestingValidatedAt;
    QDateTime atelierValidatedAt;
    QDateTime stockConsumedAt;
    int consumedSheetQty = 0;

    bool isValidContext() const;
};

struct PieceAtelierReadiness
{
    int score = 0;
    bool canValidate = false;
    QString verdict;
    QStringList alerts;
    QStringList strengths;
};

class Piece
{
public:
    Piece();
    Piece(int idPiece, const QString &nomPiece, const QString &codePiece,
          int quantite, const QString &matiere, double longueurMm,
          double largeurMm, double epaisseurMm, const QDate &dateCreation,
          int idModele, int idBois,
          PieceStatus status = PieceStatus::Published,
          const QString &sourceGeneration = QString("FORMULAIRE"),
          const QDateTime &updatedAt = QDateTime());

    int getIdPiece() const;
    QString getNomPiece() const;
    QString getCodePiece() const;
    int getQuantite() const;
    QString getMatiere() const;
    double getLongueurMm() const;
    double getLargeurMm() const;
    double getEpaisseurMm() const;
    QDate getDateCreation() const;
    int getIdModele() const;
    int getIdBois() const;
    PieceStatus getStatus() const;
    QString getSourceGeneration() const;
    QDateTime getUpdatedAt() const;

    void setIdPiece(int id);
    void setNomPiece(const QString &nom);
    void setCodePiece(const QString &code);
    void setQuantite(int qty);
    void setMatiere(const QString &mat);
    void setLongueurMm(double valeur);
    void setLargeurMm(double valeur);
    void setEpaisseurMm(double valeur);
    void setDateCreation(const QDate &date);
    void setIdModele(int id);
    void setIdBois(int id);
    void setStatus(PieceStatus status);
    void setSourceGeneration(const QString &sourceGeneration);
    void setUpdatedAt(const QDateTime &updatedAt);

    bool ajouter();
    QSqlQueryModel *afficher();
    bool supprimer(int idPiece);
    bool modifier(int idPiece);

    QString getLastError() const;

    static bool ensureStorage(QString *errorMessage = nullptr);
    static bool validerNomPiece(const QString &nom);
    static bool validerCodePiece(const QString &code);
    static bool validerQuantite(int qty);
    static bool validerMatiere(const QString &matiere);
    static bool validerDimension(double valeur);
    static bool validerForeignKey(int id);
    static QString statusToString(PieceStatus status);
    static PieceStatus statusFromString(const QString &status);

private:
    int idPiece;
    QString nomPiece;
    QString codePiece;
    int quantite;
    QString matiere;
    double longueurMm;
    double largeurMm;
    double epaisseurMm;
    QDate dateCreation;
    int idModele;
    int idBois;
    PieceStatus status;
    QString sourceGeneration;
    QDateTime updatedAt;
    QString lastErrMsg;
};

class PieceRepository
{
public:
    static bool ensureStorage(QString *errorMessage = nullptr);
    static QVector<PieceDraft> loadPieces(int modeleId,
                                          int boisId,
                                          const QVector<PieceStatus> &statuses,
                                          QString *errorMessage = nullptr);
    static bool hasDraftPieces(int modeleId,
                               int boisId,
                               QString *errorMessage = nullptr);
    static int countPublishedPieces(int modeleId,
                                    int boisId,
                                    QString *errorMessage = nullptr);
    static bool loadById(int idPiece,
                         Piece &piece,
                         QString *errorMessage = nullptr);
    static bool addPublishedPiece(const Piece &piece,
                                  QString &errorMessage);
    static bool deletePiece(int idPiece,
                            QString &errorMessage);
    static bool updatePieceField(int idPiece,
                                 int column,
                                 const QVariant &value,
                                 QString &errorMessage);
    static bool updatePiece(const Piece &piece,
                            QString &errorMessage);
    static bool createOrReplaceDraftPieces(int modeleId,
                                           int boisId,
                                           const QVector<PieceDraft> &drafts,
                                           const QString &sourceGeneration,
                                           QString &errorMessage);
    static bool discardDraftContext(int modeleId,
                                    int boisId,
                                    QString &errorMessage);
    static bool publishContext(int modeleId,
                               int boisId,
                               bool replacePublished,
                               QString &errorMessage);
    static bool clonePublishedToDraftContext(int modeleId,
                                             int boisId,
                                             QString &errorMessage);
    static bool resolveMatiereForBois(int idBois,
                                      QString &matiere,
                                      QString *errorMessage = nullptr);
};

class PieceWorkflowStateRepository
{
public:
    static bool ensureStorage(QString *errorMessage = nullptr);
    static PieceWorkflowState loadWorkflowState(int modeleId,
                                                int boisId,
                                                QString *errorMessage = nullptr);
    static bool saveWorkflowState(const PieceWorkflowState &state,
                                  QString &errorMessage);
    static bool clearWorkflowState(int modeleId,
                                   int boisId,
                                   QString &errorMessage);
    static bool validateAtelierAndConsumeStock(int modeleId,
                                               int boisId,
                                               QString &errorMessage);
};

class PieceWorkflowService
{
public:
    static QString buildWorkingSetSignature(const QVector<PieceDraft> &drafts);
    static PieceAtelierReadiness evaluateAtelierReadiness(const QVector<PieceDraft> &drafts,
                                                          const QString &manualPlan,
                                                          int passes,
                                                          bool axisLocked,
                                                          bool keepOffcuts);
    static bool validateDraft(const PieceDraft &draft,
                              QString &errorMessage);
    static bool validateDrafts(const QVector<PieceDraft> &drafts,
                               QString &errorMessage);
};

#endif // PIECE_H
