#include "piece.h"

#include <QCryptographicHash>
#include <QDateTime>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpression>
#include <QSet>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include <algorithm>

namespace {

QString normalizeCodeToken(QString value)
{
    value = value.trimmed().toUpper();
    value.replace(QRegularExpression("[^A-Z0-9]+"), "-");
    value.replace(QRegularExpression("-{2,}"), "-");
    value.remove(QRegularExpression("^-+|-+$"));
    return value.isEmpty() ? QString("PIECE") : value.left(50);
}

QString sanitizeSourceGeneration(const QString &sourceGeneration)
{
    const QString normalized = sourceGeneration.trimmed().toUpper();
    if (normalized == "AUTO" || normalized == "AUTOMATIQUE" || normalized == "AUTO IA" || normalized == "AUTO_IA") {
        return "AUTO_IA";
    }
    if (normalized == "FORM" || normalized == "FORMULAIRE") {
        return "FORMULAIRE";
    }
    if (normalized == "MANUAL" || normalized == "MANUEL") {
        return "MANUEL";
    }
    return normalized.isEmpty() ? QString("MANUEL") : normalized.left(20);
}

QString workflowStageToString(PieceWorkflowStage stage)
{
    switch (stage) {
    case PieceWorkflowStage::Draft:
        return "BROUILLON";
    case PieceWorkflowStage::Published:
        return "PUBLIE";
    case PieceWorkflowStage::NestingValidated:
        return "NESTING_VALIDE";
    case PieceWorkflowStage::AtelierValidated:
        return "ATELIER_VALIDE";
    case PieceWorkflowStage::Empty:
    default:
        return "VIDE";
    }
}

PieceWorkflowStage workflowStageFromString(const QString &stage)
{
    const QString normalized = stage.trimmed().toUpper();
    if (normalized == "BROUILLON") {
        return PieceWorkflowStage::Draft;
    }
    if (normalized == "PUBLIE") {
        return PieceWorkflowStage::Published;
    }
    if (normalized == "NESTING_VALIDE") {
        return PieceWorkflowStage::NestingValidated;
    }
    if (normalized == "ATELIER_VALIDE") {
        return PieceWorkflowStage::AtelierValidated;
    }
    return PieceWorkflowStage::Empty;
}

bool tableExists(const QString &tableName, QString *errorMessage = nullptr)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM USER_TABLES WHERE TABLE_NAME = :tableName");
    query.bindValue(":tableName", tableName.trimmed().toUpper());
    if (!query.exec() || !query.next()) {
        if (errorMessage) {
            *errorMessage = query.lastError().text();
        }
        return false;
    }
    return query.value(0).toInt() > 0;
}

bool columnExists(const QString &tableName,
                  const QString &columnName,
                  QString *errorMessage = nullptr)
{
    QSqlQuery query;
    query.prepare(
        "SELECT COUNT(*) "
        "FROM USER_TAB_COLUMNS "
        "WHERE TABLE_NAME = :tableName AND COLUMN_NAME = :columnName");
    query.bindValue(":tableName", tableName.trimmed().toUpper());
    query.bindValue(":columnName", columnName.trimmed().toUpper());
    if (!query.exec() || !query.next()) {
        if (errorMessage) {
            *errorMessage = query.lastError().text();
        }
        return false;
    }
    return query.value(0).toInt() > 0;
}

bool constraintExists(const QString &constraintName, QString *errorMessage = nullptr)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM USER_CONSTRAINTS WHERE CONSTRAINT_NAME = :constraintName");
    query.bindValue(":constraintName", constraintName.trimmed().toUpper());
    if (!query.exec() || !query.next()) {
        if (errorMessage) {
            *errorMessage = query.lastError().text();
        }
        return false;
    }
    return query.value(0).toInt() > 0;
}

bool indexExists(const QString &indexName, QString *errorMessage = nullptr)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM USER_INDEXES WHERE INDEX_NAME = :indexName");
    query.bindValue(":indexName", indexName.trimmed().toUpper());
    if (!query.exec() || !query.next()) {
        if (errorMessage) {
            *errorMessage = query.lastError().text();
        }
        return false;
    }
    return query.value(0).toInt() > 0;
}

bool ensureIndex(const QString &indexName,
                 const QString &createSql,
                 QString *errorMessage = nullptr)
{
    QString existsError;
    const bool exists = indexExists(indexName, &existsError);
    if (!existsError.isEmpty() && !exists) {
        if (errorMessage) {
            *errorMessage = existsError;
        }
        return false;
    }
    if (exists) {
        return true;
    }

    QSqlQuery query;
    if (!query.exec(createSql)) {
        const QSqlError sqlError = query.lastError();
        const QString message = sqlError.text().trimmed();
        // ORA-00955: name is already used by an existing object.
        if (message.contains("ORA-00955", Qt::CaseInsensitive)) {
            return true;
        }
        if (errorMessage) {
            *errorMessage = message;
        }
        return false;
    }
    return true;
}

bool workflowStateExists(int modeleId, int boisId, QString *errorMessage = nullptr)
{
    QSqlQuery query;
    query.prepare(
        "SELECT COUNT(*) FROM PIECE_WORKFLOW_STATE "
        "WHERE IDMODELE = :idModele AND IDBOIS = :idBois");
    query.bindValue(":idModele", modeleId);
    query.bindValue(":idBois", boisId);
    if (!query.exec() || !query.next()) {
        if (errorMessage) {
            *errorMessage = query.lastError().text();
        }
        return false;
    }
    return query.value(0).toInt() > 0;
}

bool recordExists(const QString &tableName,
                  const QString &idColumn,
                  int id,
                  QString *errorMessage = nullptr)
{
    QSqlQuery query;
    query.prepare(QString("SELECT COUNT(*) FROM %1 WHERE %2 = :id").arg(tableName, idColumn));
    query.bindValue(":id", id);
    if (!query.exec() || !query.next()) {
        if (errorMessage) {
            *errorMessage = query.lastError().text();
        }
        return false;
    }
    return query.value(0).toInt() > 0;
}

QString makeUniqueContextCode(const QString &rawBase,
                              int modeleId,
                              int boisId,
                              QSet<QString> &usedCodes)
{
    const QString base = normalizeCodeToken(rawBase);
    QString candidate = base.isEmpty() ? QString("PIECE-M%1-B%2").arg(modeleId).arg(boisId) : base;
    if (!usedCodes.contains(candidate)) {
        usedCodes.insert(candidate);
        return candidate;
    }

    const QString contextualBase = normalizeCodeToken(QString("%1-M%2-B%3").arg(candidate).arg(modeleId).arg(boisId));
    candidate = contextualBase;
    int suffix = 2;
    while (usedCodes.contains(candidate)) {
        candidate = normalizeCodeToken(QString("%1-%2").arg(contextualBase).arg(suffix++));
    }
    usedCodes.insert(candidate);
    return candidate;
}

QString toOracleDateTimeString(const QDateTime &value)
{
    return value.isValid() ? value.toString("yyyy-MM-dd HH:mm:ss") : QString();
}

QString workflowLockMessage()
{
    return "Contexte atelier verrouille, lancer une nouvelle revision pour modifier ce jeu.";
}

QString normalizeWorkflowText(QString value)
{
    value = value.trimmed().toUpper();
    value.replace(QRegularExpression("\\s+"), " ");
    return value;
}

QString buildTextSignature(const QString &text)
{
    const QString normalized = normalizeWorkflowText(text);
    if (normalized.isEmpty()) {
        return QString();
    }

    return QString::fromLatin1(QCryptographicHash::hash(normalized.toUtf8(),
                                                        QCryptographicHash::Sha256)
                                   .toHex());
}

QJsonObject parseWorkflowStateObject(const QString &stateJson)
{
    QJsonParseError parseError;
    const QJsonDocument document = QJsonDocument::fromJson(stateJson.toUtf8(), &parseError);
    if (parseError.error != QJsonParseError::NoError || !document.isObject()) {
        return {};
    }
    return document.object();
}

QJsonObject workflowSignaturesObject(const QJsonObject &root)
{
    return root.value("signatures").toObject();
}

QString workflowSignatureValue(const QJsonObject &root, const QString &key)
{
    return workflowSignaturesObject(root).value(key).toString().trimmed();
}

void setWorkflowSignatureValue(QJsonObject &root,
                               const QString &key,
                               const QString &value)
{
    QJsonObject signatures = workflowSignaturesObject(root);
    if (value.trimmed().isEmpty()) {
        signatures.remove(key);
    } else {
        signatures.insert(key, value.trimmed());
    }
    root.insert("signatures", signatures);
}

bool isWorkflowContextLocked(int modeleId,
                             int boisId,
                             QString *errorMessage = nullptr)
{
    if (errorMessage) {
        errorMessage->clear();
    }
    if (!Piece::validerForeignKey(modeleId) || !Piece::validerForeignKey(boisId)) {
        return false;
    }

    QString existsError;
    const bool workflowTableExists = tableExists("PIECE_WORKFLOW_STATE", &existsError);
    if (!existsError.isEmpty() && !workflowTableExists) {
        if (errorMessage) {
            *errorMessage = existsError;
        }
        return false;
    }
    if (!workflowTableExists) {
        return false;
    }

    QSqlQuery query;
    query.prepare(
        "SELECT STOCK_CONSUMED_AT "
        "FROM PIECE_WORKFLOW_STATE "
        "WHERE IDMODELE = :idModele AND IDBOIS = :idBois");
    query.bindValue(":idModele", modeleId);
    query.bindValue(":idBois", boisId);
    if (!query.exec()) {
        if (errorMessage) {
            *errorMessage = query.lastError().text();
        }
        return false;
    }
    if (!query.next()) {
        return false;
    }
    return query.value(0).toDateTime().isValid();
}

bool refreshWorkflowStateAfterMutation(int modeleId,
                                       int boisId,
                                       const QString &reason,
                                       QString *errorMessage = nullptr)
{
    if (errorMessage) {
        errorMessage->clear();
    }
    if (!Piece::validerForeignKey(modeleId) || !Piece::validerForeignKey(boisId)) {
        return true;
    }

    QString draftError;
    const bool hasDrafts = PieceRepository::hasDraftPieces(modeleId, boisId, &draftError);
    if (!draftError.isEmpty()) {
        if (errorMessage) {
            *errorMessage = draftError;
        }
        return false;
    }

    QString publishedError;
    const int publishedCount = PieceRepository::countPublishedPieces(modeleId, boisId, &publishedError);
    if (!publishedError.isEmpty()) {
        if (errorMessage) {
            *errorMessage = publishedError;
        }
        return false;
    }

    if (!hasDrafts && publishedCount <= 0) {
        QString clearError;
        const bool cleared = PieceWorkflowStateRepository::clearWorkflowState(modeleId, boisId, clearError);
        if (!cleared && errorMessage) {
            *errorMessage = clearError;
        }
        return cleared;
    }

    PieceWorkflowState state = PieceWorkflowStateRepository::loadWorkflowState(modeleId, boisId, errorMessage);
    if (errorMessage && !errorMessage->isEmpty()) {
        return false;
    }
    if (!state.isValidContext()) {
        state.modeleId = modeleId;
        state.boisId = boisId;
    }

    QJsonObject root = parseWorkflowStateObject(state.stateJson);
    root.insert("workflowWarning", reason);
    root.insert("atelierNeedsRevalidation", true);

    QJsonObject nesting = root.value("nesting").toObject();
    nesting.insert("summary", QString("Nesting a recalculer apres modification du jeu de pieces."));
    nesting.insert("log", QString());
    nesting.insert("usedSheets", 0);
    nesting.insert("unplacedCount", 0);
    nesting.insert("placements", QJsonArray());
    root.insert("nesting", nesting);

    const QVector<PieceDraft> draftPieces = PieceRepository::loadPieces(modeleId,
                                                                        boisId,
                                                                        {PieceStatus::Draft},
                                                                        nullptr);
    const QVector<PieceDraft> publishedPieces = PieceRepository::loadPieces(modeleId,
                                                                            boisId,
                                                                            {PieceStatus::Published},
                                                                            nullptr);
    setWorkflowSignatureValue(root,
                              "draftSignature",
                              PieceWorkflowService::buildWorkingSetSignature(draftPieces));
    setWorkflowSignatureValue(root,
                              "publishedSignature",
                              PieceWorkflowService::buildWorkingSetSignature(publishedPieces));
    setWorkflowSignatureValue(root, "nestingSignature", QString());
    setWorkflowSignatureValue(root,
                              "manualPlanSignature",
                              buildTextSignature(root.value("manualPlan").toString()));

    state.currentStage = hasDrafts ? PieceWorkflowStage::Draft : PieceWorkflowStage::Published;
    state.dirty = hasDrafts;
    state.nestingValidatedAt = QDateTime();
    state.atelierValidatedAt = QDateTime();
    if (!state.stockConsumedAt.isValid()) {
        state.consumedSheetQty = 0;
    }
    state.stateJson = QString::fromUtf8(QJsonDocument(root).toJson(QJsonDocument::Compact));
    QString saveError;
    const bool saved = PieceWorkflowStateRepository::saveWorkflowState(state, saveError);
    if (!saved && errorMessage) {
        *errorMessage = saveError;
    }
    return saved;
}

bool dropLegacyPieceCodeConstraint(QString *errorMessage = nullptr)
{
    QSqlQuery query;
    const QString sql =
        "SELECT uc.CONSTRAINT_NAME "
        "FROM USER_CONSTRAINTS uc "
        "JOIN USER_CONS_COLUMNS ucc ON uc.CONSTRAINT_NAME = ucc.CONSTRAINT_NAME "
        "WHERE uc.TABLE_NAME = 'PIECE' AND uc.CONSTRAINT_TYPE = 'U' "
        "GROUP BY uc.CONSTRAINT_NAME "
        "HAVING COUNT(*) = 1 "
        "AND MAX(CASE WHEN ucc.COLUMN_NAME = 'CODEPIECE' THEN 1 ELSE 0 END) = 1";

    if (!query.exec(sql)) {
        if (errorMessage) {
            *errorMessage = query.lastError().text();
        }
        return false;
    }

    while (query.next()) {
        const QString constraintName = query.value(0).toString().trimmed();
        if (constraintName.isEmpty()) {
            continue;
        }

        QSqlQuery dropQuery;
        if (!dropQuery.exec(QString("ALTER TABLE PIECE DROP CONSTRAINT %1").arg(constraintName))) {
            if (errorMessage) {
                *errorMessage = dropQuery.lastError().text();
            }
            return false;
        }
    }

    return true;
}

bool ensurePieceStatusSchema(QString *errorMessage = nullptr)
{
    QString tableError;
    if (!tableExists("PIECE", &tableError)) {
        if (errorMessage) {
            *errorMessage = tableError.isEmpty() ? QString("La table PIECE est introuvable.") : tableError;
        }
        return false;
    }

    struct ColumnPatch {
        const char *columnName;
        const char *alterSql;
    };

    const ColumnPatch patches[] = {
        {"STATUT", "ALTER TABLE PIECE ADD (STATUT VARCHAR2(20) DEFAULT 'PUBLIE')"},
        {"SOURCE_GENERATION", "ALTER TABLE PIECE ADD (SOURCE_GENERATION VARCHAR2(20) DEFAULT 'FORMULAIRE')"},
        {"UPDATED_AT", "ALTER TABLE PIECE ADD (UPDATED_AT DATE)"},
        {"LONGUEUR_PLAQUE", "ALTER TABLE PIECE ADD (LONGUEUR_PLAQUE NUMBER(10,2))"},
        {"LARGEUR_PLAQUE", "ALTER TABLE PIECE ADD (LARGEUR_PLAQUE NUMBER(10,2))"},
        {"EPAISSEUR_PLAQUE", "ALTER TABLE PIECE ADD (EPAISSEUR_PLAQUE NUMBER(10,2))"},
        {"SURFACE_PLAQUE", "ALTER TABLE PIECE ADD (SURFACE_PLAQUE NUMBER(18,2))"},
        {"VOLUME_PLAQUE", "ALTER TABLE PIECE ADD (VOLUME_PLAQUE NUMBER(18,2))"},
        {"SOURCE_DETECTION", "ALTER TABLE PIECE ADD (SOURCE_DETECTION VARCHAR2(50))"},
        {"DATE_SCAN", "ALTER TABLE PIECE ADD (DATE_SCAN DATE)"}
    };

    for (const ColumnPatch &patch : patches) {
        QString columnError;
        const bool exists = columnExists("PIECE", patch.columnName, &columnError);
        if (!columnError.isEmpty()) {
            if (errorMessage) {
                *errorMessage = columnError;
            }
            return false;
        }
        if (!exists) {
            QSqlQuery alterQuery;
            if (!alterQuery.exec(QString::fromLatin1(patch.alterSql))) {
                if (errorMessage) {
                    *errorMessage = alterQuery.lastError().text();
                }
                return false;
            }
        }
    }

    QSqlQuery normalizeQuery;
    if (!normalizeQuery.exec(
            "UPDATE PIECE SET "
            "STATUT = NVL(STATUT, 'PUBLIE'), "
            "SOURCE_GENERATION = NVL(SOURCE_GENERATION, 'FORMULAIRE'), "
            "UPDATED_AT = NVL(UPDATED_AT, NVL(DATE_CREATION, SYSDATE))")) {
        if (errorMessage) {
            *errorMessage = normalizeQuery.lastError().text();
        }
        return false;
    }

    QString constraintError;
    if (!dropLegacyPieceCodeConstraint(&constraintError) && errorMessage && errorMessage->isEmpty()) {
        *errorMessage = constraintError;
    }

    if (!constraintExists("UQ_PIECE_CTX_STATUS_CODE")) {
        QSqlQuery constraintQuery;
        constraintQuery.exec(
            "ALTER TABLE PIECE ADD CONSTRAINT UQ_PIECE_CTX_STATUS_CODE "
            "UNIQUE (IDMODELE, IDBOIS, STATUT, CODEPIECE)");
    }

    if (!ensureIndex("IDX_PIECE_CTX_STATUS",
                     "CREATE INDEX IDX_PIECE_CTX_STATUS ON PIECE (IDMODELE, IDBOIS, STATUT, UPDATED_AT)",
                     errorMessage)) {
        return false;
    }
    if (!ensureIndex("IDX_PIECE_CTX_CODE",
                     "CREATE INDEX IDX_PIECE_CTX_CODE ON PIECE (IDMODELE, IDBOIS, CODEPIECE)",
                     errorMessage)) {
        return false;
    }

    return true;
}

QVector<QString> normalizeStatuses(const QVector<PieceStatus> &statuses)
{
    QVector<QString> values;
    values.reserve(statuses.size());
    for (const PieceStatus status : statuses) {
        values.append(Piece::statusToString(status));
    }
    return values;
}

int sheetCountFromJson(const QJsonObject &root)
{
    const QJsonObject nesting = root.value("nesting").toObject();
    int usedSheets = nesting.value("usedSheets").toInt();
    if (usedSheets > 0) {
        return usedSheets;
    }

    const QJsonArray placements = nesting.value("placements").toArray();
    for (const QJsonValue &value : placements) {
        usedSheets = qMax(usedSheets, value.toObject().value("feuille").toInt());
    }
    return usedSheets;
}

int unplacedCountFromJson(const QJsonObject &root)
{
    return root.value("nesting").toObject().value("unplacedCount").toInt();
}

} // namespace

bool PieceWorkflowState::isValidContext() const
{
    return Piece::validerForeignKey(modeleId) && Piece::validerForeignKey(boisId);
}

Piece::Piece()
    : idPiece(0),
      quantite(0),
      longueurMm(0.0),
      largeurMm(0.0),
      epaisseurMm(0.0),
      idModele(0),
      idBois(0),
      status(PieceStatus::Published),
      sourceGeneration("FORMULAIRE")
{}

Piece::Piece(int idPiece, const QString &nomPiece, const QString &codePiece,
             int quantite, const QString &matiere, double longueurMm,
             double largeurMm, double epaisseurMm, const QDate &dateCreation,
             int idModele, int idBois, PieceStatus status,
             const QString &sourceGeneration, const QDateTime &updatedAt)
    : idPiece(idPiece),
      nomPiece(nomPiece),
      codePiece(codePiece),
      quantite(quantite),
      matiere(matiere),
      longueurMm(longueurMm),
      largeurMm(largeurMm),
      epaisseurMm(epaisseurMm),
      dateCreation(dateCreation),
      idModele(idModele),
      idBois(idBois),
      status(status),
      sourceGeneration(sourceGeneration),
      updatedAt(updatedAt)
{}

int Piece::getIdPiece() const { return idPiece; }
QString Piece::getNomPiece() const { return nomPiece; }
QString Piece::getCodePiece() const { return codePiece; }
int Piece::getQuantite() const { return quantite; }
QString Piece::getMatiere() const { return matiere; }
double Piece::getLongueurMm() const { return longueurMm; }
double Piece::getLargeurMm() const { return largeurMm; }
double Piece::getEpaisseurMm() const { return epaisseurMm; }
QDate Piece::getDateCreation() const { return dateCreation; }
int Piece::getIdModele() const { return idModele; }
int Piece::getIdBois() const { return idBois; }
PieceStatus Piece::getStatus() const { return status; }
QString Piece::getSourceGeneration() const { return sourceGeneration; }
QDateTime Piece::getUpdatedAt() const { return updatedAt; }

void Piece::setIdPiece(int id) { idPiece = id; }
void Piece::setNomPiece(const QString &nom) { nomPiece = nom; }
void Piece::setCodePiece(const QString &code) { codePiece = code; }
void Piece::setQuantite(int qty) { quantite = qty; }
void Piece::setMatiere(const QString &mat) { matiere = mat; }
void Piece::setLongueurMm(double valeur) { longueurMm = valeur; }
void Piece::setLargeurMm(double valeur) { largeurMm = valeur; }
void Piece::setEpaisseurMm(double valeur) { epaisseurMm = valeur; }
void Piece::setDateCreation(const QDate &date) { dateCreation = date; }
void Piece::setIdModele(int id) { idModele = id; }
void Piece::setIdBois(int id) { idBois = id; }
void Piece::setStatus(PieceStatus value) { status = value; }
void Piece::setSourceGeneration(const QString &value) { sourceGeneration = value; }
void Piece::setUpdatedAt(const QDateTime &value) { updatedAt = value; }

QString Piece::getLastError() const { return lastErrMsg; }

bool Piece::ensureStorage(QString *errorMessage)
{
    return ensurePieceStatusSchema(errorMessage);
}

QString Piece::statusToString(PieceStatus status)
{
    return status == PieceStatus::Draft ? QString("BROUILLON") : QString("PUBLIE");
}

PieceStatus Piece::statusFromString(const QString &status)
{
    return status.trimmed().compare("BROUILLON", Qt::CaseInsensitive) == 0
        ? PieceStatus::Draft
        : PieceStatus::Published;
}

bool Piece::validerNomPiece(const QString &nom)
{
    const QString value = nom.trimmed();
    return !value.isEmpty() && value.length() <= 100;
}

bool Piece::validerCodePiece(const QString &code)
{
    const QString value = code.trimmed();
    return !value.isEmpty() && value.length() <= 50;
}

bool Piece::validerQuantite(int qty)
{
    return qty > 0;
}

bool Piece::validerMatiere(const QString &matiere)
{
    const QString value = matiere.trimmed();
    return !value.isEmpty() && value.length() <= 50;
}

bool Piece::validerDimension(double valeur)
{
    return valeur > 0.0;
}

bool Piece::validerForeignKey(int id)
{
    return id > 0;
}

bool Piece::ajouter()
{
    lastErrMsg.clear();
    if (!ensureStorage(&lastErrMsg)) {
        return false;
    }

    if (!validerNomPiece(nomPiece)) {
        lastErrMsg = "Nom de piece invalide (1-100 caracteres).";
        return false;
    }
    if (!validerCodePiece(codePiece)) {
        lastErrMsg = "Code de piece invalide (1-50 caracteres).";
        return false;
    }
    if (!validerQuantite(quantite)) {
        lastErrMsg = "Quantite invalide (doit etre > 0).";
        return false;
    }
    if (!validerMatiere(matiere)) {
        lastErrMsg = "Matiere invalide (1-50 caracteres).";
        return false;
    }
    if (!validerDimension(longueurMm) || !validerDimension(largeurMm) || !validerDimension(epaisseurMm)) {
        lastErrMsg = "Les dimensions doivent etre superieures a 0.";
        return false;
    }
    if (!validerForeignKey(idModele) || !validerForeignKey(idBois)) {
        lastErrMsg = "ID Modele et ID Bois doivent etre superieurs a 0.";
        return false;
    }

    QSqlQuery queryId;
    queryId.prepare("SELECT NVL(MAX(IDPIECE), 0) + 1 FROM PIECE");
    if (!queryId.exec() || !queryId.next()) {
        lastErrMsg = queryId.lastError().text().trimmed().isEmpty()
            ? QString("Impossible de generer l'ID automatiquement.")
            : queryId.lastError().text();
        return false;
    }
    const int autoId = queryId.value(0).toInt();

    QSqlQuery query;
    query.prepare(
        "INSERT INTO PIECE "
        "(IDPIECE, NOMPIECE, CODEPIECE, QUANTITE, MATIERE, LONGUEUR_MM, "
        "LARGEUR_MM, EPAISSEUR_MM, STATUT, SOURCE_GENERATION, UPDATED_AT, DATE_CREATION, IDMODELE, IDBOIS) "
        "VALUES "
        "(:idPiece, :nomPiece, :codePiece, :quantite, :matiere, :longueurMm, "
        ":largeurMm, :epaisseurMm, :statut, :sourceGeneration, :updatedAt, :dateCreation, :idModele, :idBois)");

    query.bindValue(":idPiece", autoId);
    query.bindValue(":nomPiece", nomPiece.trimmed());
    query.bindValue(":codePiece", normalizeCodeToken(codePiece));
    query.bindValue(":quantite", quantite);
    query.bindValue(":matiere", matiere.trimmed());
    query.bindValue(":longueurMm", longueurMm);
    query.bindValue(":largeurMm", largeurMm);
    query.bindValue(":epaisseurMm", epaisseurMm);
    query.bindValue(":statut", statusToString(status));
    query.bindValue(":sourceGeneration", sanitizeSourceGeneration(sourceGeneration));
    query.bindValue(":updatedAt", updatedAt.isValid() ? updatedAt : QDateTime::currentDateTime());
    query.bindValue(":dateCreation", dateCreation.isValid() ? dateCreation : QDate::currentDate());
    query.bindValue(":idModele", idModele);
    query.bindValue(":idBois", idBois);

    if (!query.exec()) {
        lastErrMsg = query.lastError().text();
        return false;
    }

    idPiece = autoId;
    updatedAt = QDateTime::currentDateTime();
    return true;
}

QSqlQueryModel *Piece::afficher()
{
    ensureStorage(nullptr);

    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(
        "SELECT IDPIECE, NOMPIECE, CODEPIECE, QUANTITE, MATIERE, "
        "LONGUEUR_MM, LARGEUR_MM, EPAISSEUR_MM, NVL(STATUT, 'PUBLIE') AS STATUT, "
        "NVL(SOURCE_GENERATION, 'FORMULAIRE') AS SOURCE_GENERATION, "
        "TO_CHAR(NVL(UPDATED_AT, DATE_CREATION), 'DD/MM/YYYY HH24:MI') AS UPDATED_AT, "
        "TO_CHAR(DATE_CREATION, 'DD/MM/YYYY') AS DATE_CREATION, "
        "IDMODELE, IDBOIS "
        "FROM PIECE "
        "ORDER BY CASE NVL(STATUT, 'PUBLIE') WHEN 'BROUILLON' THEN 0 ELSE 1 END, "
        "NVL(UPDATED_AT, DATE_CREATION) DESC, IDPIECE DESC");

    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Code");
    model->setHeaderData(3, Qt::Horizontal, "Quantite");
    model->setHeaderData(4, Qt::Horizontal, "Matiere");
    model->setHeaderData(5, Qt::Horizontal, "Longueur");
    model->setHeaderData(6, Qt::Horizontal, "Largeur");
    model->setHeaderData(7, Qt::Horizontal, "Epaisseur");
    model->setHeaderData(8, Qt::Horizontal, "Statut");
    model->setHeaderData(9, Qt::Horizontal, "Source");
    model->setHeaderData(10, Qt::Horizontal, "Maj");
    model->setHeaderData(11, Qt::Horizontal, "Date Creation");
    model->setHeaderData(12, Qt::Horizontal, "ID Modele");
    model->setHeaderData(13, Qt::Horizontal, "ID Bois");
    return model;
}

bool Piece::supprimer(int idPiece)
{
    lastErrMsg.clear();
    if (!ensureStorage(&lastErrMsg)) {
        return false;
    }
    if (idPiece <= 0) {
        lastErrMsg = "ID Piece invalide.";
        return false;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM PIECE WHERE IDPIECE = :id");
    query.bindValue(":id", idPiece);
    if (!query.exec()) {
        lastErrMsg = query.lastError().text();
        return false;
    }
    if (query.numRowsAffected() == 0) {
        lastErrMsg = "Aucune piece trouvee avec cet ID.";
        return false;
    }
    return true;
}

bool Piece::modifier(int idPiece)
{
    lastErrMsg.clear();
    if (!ensureStorage(&lastErrMsg)) {
        return false;
    }

    if (idPiece <= 0) {
        lastErrMsg = "ID Piece invalide.";
        return false;
    }
    if (!validerNomPiece(nomPiece)) {
        lastErrMsg = "Nom de piece invalide (1-100 caracteres).";
        return false;
    }
    if (!validerCodePiece(codePiece)) {
        lastErrMsg = "Code de piece invalide (1-50 caracteres).";
        return false;
    }
    if (!validerQuantite(quantite)) {
        lastErrMsg = "Quantite invalide (doit etre > 0).";
        return false;
    }
    if (!validerMatiere(matiere)) {
        lastErrMsg = "Matiere invalide (1-50 caracteres).";
        return false;
    }
    if (!validerDimension(longueurMm) || !validerDimension(largeurMm) || !validerDimension(epaisseurMm)) {
        lastErrMsg = "Les dimensions doivent etre superieures a 0.";
        return false;
    }
    if (!validerForeignKey(idModele) || !validerForeignKey(idBois)) {
        lastErrMsg = "ID Modele et ID Bois doivent etre superieurs a 0.";
        return false;
    }

    QSqlQuery query;
    query.prepare(
        "UPDATE PIECE SET "
        "NOMPIECE = :nomPiece, "
        "CODEPIECE = :codePiece, "
        "QUANTITE = :quantite, "
        "MATIERE = :matiere, "
        "LONGUEUR_MM = :longueurMm, "
        "LARGEUR_MM = :largeurMm, "
        "EPAISSEUR_MM = :epaisseurMm, "
        "STATUT = :statut, "
        "SOURCE_GENERATION = :sourceGeneration, "
        "UPDATED_AT = :updatedAt, "
        "IDMODELE = :idModele, "
        "IDBOIS = :idBois "
        "WHERE IDPIECE = :idPiece");

    query.bindValue(":nomPiece", nomPiece.trimmed());
    query.bindValue(":codePiece", normalizeCodeToken(codePiece));
    query.bindValue(":quantite", quantite);
    query.bindValue(":matiere", matiere.trimmed());
    query.bindValue(":longueurMm", longueurMm);
    query.bindValue(":largeurMm", largeurMm);
    query.bindValue(":epaisseurMm", epaisseurMm);
    query.bindValue(":statut", statusToString(status));
    query.bindValue(":sourceGeneration", sanitizeSourceGeneration(sourceGeneration));
    query.bindValue(":updatedAt", updatedAt.isValid() ? updatedAt : QDateTime::currentDateTime());
    query.bindValue(":idModele", idModele);
    query.bindValue(":idBois", idBois);
    query.bindValue(":idPiece", idPiece);

    if (!query.exec()) {
        lastErrMsg = query.lastError().text();
        return false;
    }
    if (query.numRowsAffected() == 0) {
        lastErrMsg = "Aucune piece trouvee avec cet ID.";
        return false;
    }
    return true;
}

bool PieceRepository::ensureStorage(QString *errorMessage)
{
    return Piece::ensureStorage(errorMessage);
}

QVector<PieceDraft> PieceRepository::loadPieces(int modeleId,
                                                int boisId,
                                                const QVector<PieceStatus> &statuses,
                                                QString *errorMessage)
{
    QVector<PieceDraft> drafts;
    if (!ensureStorage(errorMessage)) {
        return drafts;
    }
    if (!Piece::validerForeignKey(modeleId) || !Piece::validerForeignKey(boisId)) {
        return drafts;
    }

    QString sql =
        "SELECT NOMPIECE, CODEPIECE, QUANTITE, MATIERE, LONGUEUR_MM, LARGEUR_MM, "
        "EPAISSEUR_MM, NVL(SOURCE_GENERATION, 'FORMULAIRE') AS SOURCE_GENERATION "
        "FROM PIECE WHERE IDMODELE = :idModele AND IDBOIS = :idBois";

    const QVector<QString> normalizedStatuses = normalizeStatuses(statuses);
    for (int i = 0; i < normalizedStatuses.size(); ++i) {
        sql += (i == 0) ? " AND NVL(STATUT, 'PUBLIE') IN (" : ", ";
        sql += QString(":status%1").arg(i);
    }
    if (!normalizedStatuses.isEmpty()) {
        sql += ")";
    }
    sql += " ORDER BY NVL(UPDATED_AT, DATE_CREATION) DESC, IDPIECE DESC";

    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(":idModele", modeleId);
    query.bindValue(":idBois", boisId);
    for (int i = 0; i < normalizedStatuses.size(); ++i) {
        query.bindValue(QString(":status%1").arg(i), normalizedStatuses.at(i));
    }

    if (!query.exec()) {
        if (errorMessage) {
            *errorMessage = query.lastError().text();
        }
        return {};
    }

    int ordre = 1;
    while (query.next()) {
        PieceDraft draft;
        draft.nom = query.value(0).toString().trimmed();
        draft.code = query.value(1).toString().trimmed();
        draft.quantite = query.value(2).toInt();
        draft.matiere = query.value(3).toString().trimmed();
        draft.longueurMm = query.value(4).toDouble();
        draft.largeurMm = query.value(5).toDouble();
        draft.epaisseurMm = query.value(6).toDouble();
        draft.sourceGeneration = query.value(7).toString().trimmed();
        draft.ordreAffichage = ordre++;
        drafts.append(draft);
    }

    return drafts;
}

bool PieceRepository::hasDraftPieces(int modeleId,
                                     int boisId,
                                     QString *errorMessage)
{
    if (!ensureStorage(errorMessage)) {
        return false;
    }
    if (!Piece::validerForeignKey(modeleId) || !Piece::validerForeignKey(boisId)) {
        return false;
    }

    QSqlQuery query;
    query.prepare(
        "SELECT COUNT(*) FROM PIECE "
        "WHERE IDMODELE = :idModele AND IDBOIS = :idBois AND NVL(STATUT, 'PUBLIE') = 'BROUILLON'");
    query.bindValue(":idModele", modeleId);
    query.bindValue(":idBois", boisId);
    if (!query.exec() || !query.next()) {
        if (errorMessage) {
            *errorMessage = query.lastError().text();
        }
        return false;
    }
    return query.value(0).toInt() > 0;
}

int PieceRepository::countPublishedPieces(int modeleId,
                                          int boisId,
                                          QString *errorMessage)
{
    if (!ensureStorage(errorMessage)) {
        return 0;
    }

    QSqlQuery query;
    query.prepare(
        "SELECT COUNT(*) FROM PIECE "
        "WHERE IDMODELE = :idModele AND IDBOIS = :idBois AND NVL(STATUT, 'PUBLIE') = 'PUBLIE'");
    query.bindValue(":idModele", modeleId);
    query.bindValue(":idBois", boisId);
    if (!query.exec() || !query.next()) {
        if (errorMessage) {
            *errorMessage = query.lastError().text();
        }
        return 0;
    }
    return query.value(0).toInt();
}

bool PieceRepository::loadById(int idPiece,
                               Piece &piece,
                               QString *errorMessage)
{
    if (!ensureStorage(errorMessage)) {
        return false;
    }

    QSqlQuery query;
    query.prepare(
        "SELECT IDPIECE, NOMPIECE, CODEPIECE, QUANTITE, MATIERE, LONGUEUR_MM, "
        "LARGEUR_MM, EPAISSEUR_MM, DATE_CREATION, IDMODELE, IDBOIS, "
        "NVL(STATUT, 'PUBLIE'), NVL(SOURCE_GENERATION, 'FORMULAIRE'), NVL(UPDATED_AT, DATE_CREATION) "
        "FROM PIECE WHERE IDPIECE = :idPiece");
    query.bindValue(":idPiece", idPiece);

    if (!query.exec() || !query.next()) {
        if (errorMessage) {
            *errorMessage = query.lastError().isValid()
                ? query.lastError().text()
                : QString("Piece introuvable.");
        }
        return false;
    }

    piece.setIdPiece(query.value(0).toInt());
    piece.setNomPiece(query.value(1).toString());
    piece.setCodePiece(query.value(2).toString());
    piece.setQuantite(query.value(3).toInt());
    piece.setMatiere(query.value(4).toString());
    piece.setLongueurMm(query.value(5).toDouble());
    piece.setLargeurMm(query.value(6).toDouble());
    piece.setEpaisseurMm(query.value(7).toDouble());
    piece.setDateCreation(query.value(8).toDate());
    piece.setIdModele(query.value(9).toInt());
    piece.setIdBois(query.value(10).toInt());
    piece.setStatus(Piece::statusFromString(query.value(11).toString()));
    piece.setSourceGeneration(query.value(12).toString());
    piece.setUpdatedAt(query.value(13).toDateTime());
    return true;
}

bool PieceRepository::addPublishedPiece(const Piece &piece, QString &errorMessage)
{
    errorMessage.clear();
    Piece item = piece;
    item.setStatus(PieceStatus::Published);
    if (isWorkflowContextLocked(item.getIdModele(), item.getIdBois(), nullptr)) {
        errorMessage = workflowLockMessage();
        return false;
    }
    if (item.getSourceGeneration().trimmed().isEmpty()) {
        item.setSourceGeneration("FORMULAIRE");
    }
    if (!item.ajouter()) {
        errorMessage = item.getLastError();
        return false;
    }
    refreshWorkflowStateAfterMutation(item.getIdModele(),
                                      item.getIdBois(),
                                      "Le catalogue publie a change. Le nesting et le dossier atelier doivent etre recalcules.",
                                      nullptr);
    return true;
}

bool PieceRepository::deletePiece(int idPiece,
                                  QString &errorMessage)
{
    errorMessage.clear();
    Piece existingPiece;
    if (!loadById(idPiece, existingPiece, &errorMessage)) {
        return false;
    }

    if (existingPiece.getStatus() == PieceStatus::Published
        && isWorkflowContextLocked(existingPiece.getIdModele(), existingPiece.getIdBois(), nullptr)) {
        errorMessage = workflowLockMessage();
        return false;
    }

    Piece piece;
    if (!piece.supprimer(idPiece)) {
        errorMessage = piece.getLastError();
        return false;
    }

    refreshWorkflowStateAfterMutation(existingPiece.getIdModele(),
                                      existingPiece.getIdBois(),
                                      "Le jeu de pieces a change. Le nesting et le dossier atelier doivent etre recalcules.",
                                      nullptr);
    return true;
}

bool PieceRepository::updatePieceField(int idPiece,
                                       int column,
                                       const QVariant &value,
                                       QString &errorMessage)
{
    errorMessage.clear();
    Piece piece;
    if (!loadById(idPiece, piece, &errorMessage)) {
        return false;
    }
    if (piece.getStatus() == PieceStatus::Published
        && isWorkflowContextLocked(piece.getIdModele(), piece.getIdBois(), nullptr)) {
        errorMessage = workflowLockMessage();
        return false;
    }

    bool ok = true;
    switch (column) {
    case 1:
        piece.setNomPiece(value.toString().trimmed());
        break;
    case 2:
        piece.setCodePiece(value.toString().trimmed());
        break;
    case 3:
        piece.setQuantite(value.toInt(&ok));
        if (!ok) {
            errorMessage = "Quantite invalide.";
            return false;
        }
        break;
    case 5:
        piece.setLongueurMm(value.toDouble(&ok));
        if (!ok) {
            errorMessage = "Longueur invalide.";
            return false;
        }
        break;
    case 6:
        piece.setLargeurMm(value.toDouble(&ok));
        if (!ok) {
            errorMessage = "Largeur invalide.";
            return false;
        }
        break;
    case 7:
        piece.setEpaisseurMm(value.toDouble(&ok));
        if (!ok) {
            errorMessage = "Epaisseur invalide.";
            return false;
        }
        break;
    default:
        errorMessage = "Cette colonne n'est pas editable inline.";
        return false;
    }

    piece.setUpdatedAt(QDateTime::currentDateTime());
    if (!piece.modifier(idPiece)) {
        errorMessage = piece.getLastError();
        return false;
    }

    refreshWorkflowStateAfterMutation(piece.getIdModele(),
                                      piece.getIdBois(),
                                      "Le catalogue publie a ete modifie. Le nesting et le dossier atelier doivent etre recalcules.",
                                      nullptr);

    return true;
}

bool PieceRepository::updatePiece(const Piece &piece,
                                  QString &errorMessage)
{
    errorMessage.clear();
    Piece existingPiece;
    if (!loadById(piece.getIdPiece(), existingPiece, &errorMessage)) {
        return false;
    }
    if ((existingPiece.getStatus() == PieceStatus::Published
         && isWorkflowContextLocked(existingPiece.getIdModele(), existingPiece.getIdBois(), nullptr))
        || (piece.getStatus() == PieceStatus::Published
            && isWorkflowContextLocked(piece.getIdModele(), piece.getIdBois(), nullptr))) {
        errorMessage = workflowLockMessage();
        return false;
    }

    Piece item = piece;
    item.setUpdatedAt(QDateTime::currentDateTime());
    if (!item.modifier(item.getIdPiece())) {
        errorMessage = item.getLastError();
        return false;
    }

    refreshWorkflowStateAfterMutation(existingPiece.getIdModele(),
                                      existingPiece.getIdBois(),
                                      "Le jeu de pieces publie a change. Le nesting et le dossier atelier doivent etre recalcules.",
                                      nullptr);
    if (existingPiece.getIdModele() != item.getIdModele() || existingPiece.getIdBois() != item.getIdBois()) {
        refreshWorkflowStateAfterMutation(item.getIdModele(),
                                          item.getIdBois(),
                                          "Le jeu de pieces publie a change. Le nesting et le dossier atelier doivent etre recalcules.",
                                          nullptr);
    }
    return true;
}

bool PieceRepository::createOrReplaceDraftPieces(int modeleId,
                                                 int boisId,
                                                 const QVector<PieceDraft> &drafts,
                                                 const QString &sourceGeneration,
                                                 QString &errorMessage)
{
    errorMessage.clear();
    if (!ensureStorage(&errorMessage)) {
        return false;
    }
    if (!Piece::validerForeignKey(modeleId) || !Piece::validerForeignKey(boisId)) {
        errorMessage = "Contexte de brouillon invalide.";
        return false;
    }
    if (!recordExists("MODELE", "IDMODELE", modeleId, &errorMessage)) {
        if (errorMessage.isEmpty()) {
            errorMessage = "Modele introuvable.";
        }
        return false;
    }
    if (!recordExists("BOIS", "IDBOIS", boisId, &errorMessage)) {
        if (errorMessage.isEmpty()) {
            errorMessage = "Bois introuvable.";
        }
        return false;
    }
    if (!PieceWorkflowService::validateDrafts(drafts, errorMessage)) {
        return false;
    }

    QString matiereReference;
    if (!resolveMatiereForBois(boisId, matiereReference, &errorMessage)) {
        return false;
    }

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isValid()) {
        errorMessage = "Connexion base de donnees indisponible.";
        return false;
    }
    if (!db.transaction()) {
        errorMessage = "Impossible de demarrer la transaction de brouillon.";
        return false;
    }

    QSqlQuery deleteQuery(db);
    deleteQuery.prepare(
        "DELETE FROM PIECE "
        "WHERE IDMODELE = :idModele AND IDBOIS = :idBois AND NVL(STATUT, 'PUBLIE') = 'BROUILLON'");
    deleteQuery.bindValue(":idModele", modeleId);
    deleteQuery.bindValue(":idBois", boisId);
    if (!deleteQuery.exec()) {
        db.rollback();
        errorMessage = deleteQuery.lastError().text();
        return false;
    }

    QSqlQuery nextIdQuery(db);
    if (!nextIdQuery.exec("SELECT NVL(MAX(IDPIECE), 0) FROM PIECE") || !nextIdQuery.next()) {
        db.rollback();
        errorMessage = nextIdQuery.lastError().text();
        return false;
    }
    int nextId = nextIdQuery.value(0).toInt() + 1;

    QSet<QString> usedCodes;
    QSqlQuery insertQuery(db);
    insertQuery.prepare(
        "INSERT INTO PIECE "
        "(IDPIECE, NOMPIECE, CODEPIECE, QUANTITE, MATIERE, LONGUEUR_MM, LARGEUR_MM, "
        "EPAISSEUR_MM, STATUT, SOURCE_GENERATION, UPDATED_AT, DATE_CREATION, IDMODELE, IDBOIS) "
        "VALUES "
        "(:idPiece, :nomPiece, :codePiece, :quantite, :matiere, :longueur, :largeur, "
        ":epaisseur, 'BROUILLON', :sourceGeneration, :updatedAt, :dateCreation, :idModele, :idBois)");

    for (const PieceDraft &draft : drafts) {
        const QString codeBase = draft.code.isEmpty() ? draft.nom : draft.code;
        insertQuery.bindValue(":idPiece", nextId++);
        insertQuery.bindValue(":nomPiece", draft.nom.trimmed());
        insertQuery.bindValue(":codePiece", makeUniqueContextCode(codeBase, modeleId, boisId, usedCodes));
        insertQuery.bindValue(":quantite", draft.quantite);
        insertQuery.bindValue(":matiere", matiereReference);
        insertQuery.bindValue(":longueur", draft.longueurMm);
        insertQuery.bindValue(":largeur", draft.largeurMm);
        insertQuery.bindValue(":epaisseur", draft.epaisseurMm);
        insertQuery.bindValue(":sourceGeneration",
                              sanitizeSourceGeneration(draft.sourceGeneration.trimmed().isEmpty()
                                                           ? sourceGeneration
                                                           : draft.sourceGeneration));
        insertQuery.bindValue(":updatedAt", QDateTime::currentDateTime());
        insertQuery.bindValue(":dateCreation", QDate::currentDate());
        insertQuery.bindValue(":idModele", modeleId);
        insertQuery.bindValue(":idBois", boisId);
        if (!insertQuery.exec()) {
            db.rollback();
            errorMessage = insertQuery.lastError().text();
            return false;
        }
    }

    if (!db.commit()) {
        errorMessage = db.lastError().text();
        db.rollback();
        return false;
    }

    return true;
}

bool PieceRepository::discardDraftContext(int modeleId,
                                          int boisId,
                                          QString &errorMessage)
{
    errorMessage.clear();
    if (!ensureStorage(&errorMessage)) {
        return false;
    }

    QSqlQuery query;
    query.prepare(
        "DELETE FROM PIECE "
        "WHERE IDMODELE = :idModele AND IDBOIS = :idBois AND NVL(STATUT, 'PUBLIE') = 'BROUILLON'");
    query.bindValue(":idModele", modeleId);
    query.bindValue(":idBois", boisId);
    if (!query.exec()) {
        errorMessage = query.lastError().text();
        return false;
    }
    return true;
}

bool PieceRepository::publishContext(int modeleId,
                                     int boisId,
                                     bool replacePublished,
                                     QString &errorMessage)
{
    errorMessage.clear();
    if (!ensureStorage(&errorMessage)) {
        return false;
    }
    if (!Piece::validerForeignKey(modeleId) || !Piece::validerForeignKey(boisId)) {
        errorMessage = "Contexte de publication invalide.";
        return false;
    }
    if (isWorkflowContextLocked(modeleId, boisId, nullptr)) {
        errorMessage = workflowLockMessage();
        return false;
    }

    const QVector<PieceDraft> drafts = loadPieces(modeleId, boisId, {PieceStatus::Draft}, &errorMessage);
    if (!PieceWorkflowService::validateDrafts(drafts, errorMessage)) {
        return false;
    }

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isValid()) {
        errorMessage = "Connexion base de donnees indisponible.";
        return false;
    }
    if (!db.transaction()) {
        errorMessage = "Impossible de demarrer la transaction de publication.";
        return false;
    }

    if (replacePublished) {
        QSqlQuery deletePublished(db);
        deletePublished.prepare(
            "DELETE FROM PIECE "
            "WHERE IDMODELE = :idModele AND IDBOIS = :idBois AND NVL(STATUT, 'PUBLIE') = 'PUBLIE'");
        deletePublished.bindValue(":idModele", modeleId);
        deletePublished.bindValue(":idBois", boisId);
        if (!deletePublished.exec()) {
            db.rollback();
            errorMessage = deletePublished.lastError().text();
            return false;
        }
    }

    QSet<QString> usedCodes;
    QSqlQuery existingCodesQuery(db);
    existingCodesQuery.prepare(
        "SELECT CODEPIECE FROM PIECE "
        "WHERE IDMODELE = :idModele AND IDBOIS = :idBois AND NVL(STATUT, 'PUBLIE') = 'PUBLIE'");
    existingCodesQuery.bindValue(":idModele", modeleId);
    existingCodesQuery.bindValue(":idBois", boisId);
    if (!existingCodesQuery.exec()) {
        db.rollback();
        errorMessage = existingCodesQuery.lastError().text();
        return false;
    }
    while (existingCodesQuery.next()) {
        usedCodes.insert(existingCodesQuery.value(0).toString().trimmed());
    }

    QSqlQuery draftRowsQuery(db);
    draftRowsQuery.prepare(
        "SELECT IDPIECE, NOMPIECE, CODEPIECE "
        "FROM PIECE "
        "WHERE IDMODELE = :idModele AND IDBOIS = :idBois AND NVL(STATUT, 'PUBLIE') = 'BROUILLON' "
        "ORDER BY NVL(UPDATED_AT, DATE_CREATION) DESC, IDPIECE DESC");
    draftRowsQuery.bindValue(":idModele", modeleId);
    draftRowsQuery.bindValue(":idBois", boisId);
    if (!draftRowsQuery.exec()) {
        db.rollback();
        errorMessage = draftRowsQuery.lastError().text();
        return false;
    }

    QSqlQuery promoteQuery(db);
    promoteQuery.prepare(
        "UPDATE PIECE SET CODEPIECE = :codePiece, STATUT = 'PUBLIE', UPDATED_AT = SYSDATE "
        "WHERE IDPIECE = :idPiece");

    while (draftRowsQuery.next()) {
        const int idPiece = draftRowsQuery.value(0).toInt();
        const QString nomPiece = draftRowsQuery.value(1).toString().trimmed();
        const QString currentCode = draftRowsQuery.value(2).toString().trimmed();
        const QString uniqueCode = makeUniqueContextCode(currentCode.isEmpty() ? nomPiece : currentCode,
                                                         modeleId,
                                                         boisId,
                                                         usedCodes);
        promoteQuery.bindValue(":codePiece", uniqueCode);
        promoteQuery.bindValue(":idPiece", idPiece);
        if (!promoteQuery.exec()) {
            db.rollback();
            errorMessage = promoteQuery.lastError().text();
            return false;
        }
    }

    if (!db.commit()) {
        errorMessage = db.lastError().text();
        db.rollback();
        return false;
    }

    PieceWorkflowState state = PieceWorkflowStateRepository::loadWorkflowState(modeleId, boisId, nullptr);
    if (!state.isValidContext()) {
        state.modeleId = modeleId;
        state.boisId = boisId;
    }
    QJsonObject root = parseWorkflowStateObject(state.stateJson);
    root.insert("workflowWarning", "Le brouillon a ete publie. Le nesting doit etre recalcule sur la version publiee.");
    root.insert("atelierNeedsRevalidation", true);

    QJsonObject nesting = root.value("nesting").toObject();
    nesting.insert("summary", QString("Le brouillon a ete publie. Recalculez le nesting sur le catalogue publie."));
    nesting.insert("log", QString());
    nesting.insert("usedSheets", 0);
    nesting.insert("unplacedCount", 0);
    nesting.insert("placements", QJsonArray());
    root.insert("nesting", nesting);

    const QVector<PieceDraft> publishedPieces = loadPieces(modeleId,
                                                           boisId,
                                                           {PieceStatus::Published},
                                                           nullptr);
    setWorkflowSignatureValue(root, "draftSignature", QString());
    setWorkflowSignatureValue(root,
                              "publishedSignature",
                              PieceWorkflowService::buildWorkingSetSignature(publishedPieces));
    setWorkflowSignatureValue(root, "nestingSignature", QString());
    setWorkflowSignatureValue(root,
                              "manualPlanSignature",
                              buildTextSignature(root.value("manualPlan").toString()));

    state.currentStage = PieceWorkflowStage::Published;
    state.dirty = false;
    state.publishedAt = QDateTime::currentDateTime();
    state.nestingValidatedAt = QDateTime();
    state.atelierValidatedAt = QDateTime();
    state.stateJson = QString::fromUtf8(QJsonDocument(root).toJson(QJsonDocument::Compact));
    QString workflowError;
    if (!PieceWorkflowStateRepository::saveWorkflowState(state, workflowError) && errorMessage.isEmpty()) {
        errorMessage = workflowError;
    }

    return true;
}

bool PieceRepository::clonePublishedToDraftContext(int modeleId,
                                                   int boisId,
                                                   QString &errorMessage)
{
    errorMessage.clear();
    const QVector<PieceDraft> publishedPieces = loadPieces(modeleId, boisId, {PieceStatus::Published}, &errorMessage);
    if (publishedPieces.isEmpty()) {
        if (errorMessage.isEmpty()) {
            errorMessage = "Aucune piece publiee a reutiliser pour cette revision.";
        }
        return false;
    }
    return createOrReplaceDraftPieces(modeleId, boisId, publishedPieces, "FORMULAIRE", errorMessage);
}

bool PieceRepository::resolveMatiereForBois(int idBois,
                                            QString &matiere,
                                            QString *errorMessage)
{
    matiere.clear();
    QSqlQuery query;
    query.prepare("SELECT NOMBOIS FROM BOIS WHERE IDBOIS = :idBois");
    query.bindValue(":idBois", idBois);
    if (!query.exec() || !query.next()) {
        if (errorMessage) {
            *errorMessage = query.lastError().isValid()
                ? query.lastError().text()
                : QString("Bois introuvable.");
        }
        return false;
    }

    matiere = query.value(0).toString().trimmed();
    if (!Piece::validerMatiere(matiere)) {
        if (errorMessage) {
            *errorMessage = "Le bois selectionne n'a pas de matiere exploitable.";
        }
        return false;
    }

    return true;
}

bool PieceWorkflowStateRepository::ensureStorage(QString *errorMessage)
{
    if (!Piece::ensureStorage(errorMessage)) {
        return false;
    }

    const auto ensureWorkflowIndexes = [errorMessage]() -> bool {
        if (!ensureIndex("IDX_PWF_STAGE_DIRTY",
                         "CREATE INDEX IDX_PWF_STAGE_DIRTY ON PIECE_WORKFLOW_STATE (CURRENT_STAGE, DIRTY)",
                         errorMessage)) {
            return false;
        }
        if (!ensureIndex("IDX_PWF_UPDATED",
                         "CREATE INDEX IDX_PWF_UPDATED ON PIECE_WORKFLOW_STATE (UPDATED_AT, STOCK_CONSUMED_AT)",
                         errorMessage)) {
            return false;
        }
        return true;
    };

    QString existsError;
    const bool exists = tableExists("PIECE_WORKFLOW_STATE", &existsError);
    if (!existsError.isEmpty() && !exists) {
        if (errorMessage) {
            *errorMessage = existsError;
        }
        return false;
    }
    if (exists) {
        return ensureWorkflowIndexes();
    }

    QSqlQuery createQuery;
    const QString createSql =
        "CREATE TABLE PIECE_WORKFLOW_STATE ("
        "IDMODELE NUMBER NOT NULL, "
        "IDBOIS NUMBER NOT NULL, "
        "CURRENT_STAGE VARCHAR2(30) DEFAULT 'VIDE' NOT NULL, "
        "DIRTY NUMBER(1) DEFAULT 0 NOT NULL, "
        "STATE_JSON CLOB, "
        "PUBLISHED_AT DATE, "
        "NESTING_VALIDATED_AT DATE, "
        "ATELIER_VALIDATED_AT DATE, "
        "STOCK_CONSUMED_AT DATE, "
        "CONSUMED_SHEET_QTY NUMBER DEFAULT 0 NOT NULL, "
        "CREATED_AT DATE DEFAULT SYSDATE, "
        "UPDATED_AT DATE DEFAULT SYSDATE, "
        "CONSTRAINT PK_PIECE_WORKFLOW_STATE PRIMARY KEY (IDMODELE, IDBOIS), "
        "CONSTRAINT FK_PIECE_WORKFLOW_MODELE FOREIGN KEY (IDMODELE) REFERENCES MODELE(IDMODELE) ON DELETE CASCADE, "
        "CONSTRAINT FK_PIECE_WORKFLOW_BOIS FOREIGN KEY (IDBOIS) REFERENCES BOIS(IDBOIS) ON DELETE CASCADE)";

    if (!createQuery.exec(createSql)) {
        if (errorMessage) {
            *errorMessage = createQuery.lastError().text();
        }
        return false;
    }
    return ensureWorkflowIndexes();
}

PieceWorkflowState PieceWorkflowStateRepository::loadWorkflowState(int modeleId,
                                                                  int boisId,
                                                                  QString *errorMessage)
{
    PieceWorkflowState state;
    state.modeleId = modeleId;
    state.boisId = boisId;

    if (!ensureStorage(errorMessage)) {
        return state;
    }
    if (!Piece::validerForeignKey(modeleId) || !Piece::validerForeignKey(boisId)) {
        return state;
    }

    QSqlQuery query;
    query.prepare(
        "SELECT CURRENT_STAGE, DIRTY, STATE_JSON, PUBLISHED_AT, NESTING_VALIDATED_AT, "
        "ATELIER_VALIDATED_AT, STOCK_CONSUMED_AT, CONSUMED_SHEET_QTY "
        "FROM PIECE_WORKFLOW_STATE "
        "WHERE IDMODELE = :idModele AND IDBOIS = :idBois");
    query.bindValue(":idModele", modeleId);
    query.bindValue(":idBois", boisId);

    if (!query.exec()) {
        if (errorMessage) {
            *errorMessage = query.lastError().text();
        }
        return state;
    }
    if (!query.next()) {
        return state;
    }

    state.currentStage = workflowStageFromString(query.value(0).toString());
    state.dirty = query.value(1).toInt() > 0;
    state.stateJson = query.value(2).toString();
    state.publishedAt = query.value(3).toDateTime();
    state.nestingValidatedAt = query.value(4).toDateTime();
    state.atelierValidatedAt = query.value(5).toDateTime();
    state.stockConsumedAt = query.value(6).toDateTime();
    state.consumedSheetQty = query.value(7).toInt();
    return state;
}

bool PieceWorkflowStateRepository::saveWorkflowState(const PieceWorkflowState &state,
                                                     QString &errorMessage)
{
    errorMessage.clear();
    if (!ensureStorage(&errorMessage)) {
        return false;
    }
    if (!state.isValidContext()) {
        errorMessage = "Contexte workflow piece invalide.";
        return false;
    }

    const bool exists = workflowStateExists(state.modeleId, state.boisId, &errorMessage);
    if (!errorMessage.isEmpty()) {
        return false;
    }

    QSqlQuery query;
    if (exists) {
        query.prepare(
            "UPDATE PIECE_WORKFLOW_STATE SET "
            "CURRENT_STAGE = :currentStage, "
            "DIRTY = :dirty, "
            "STATE_JSON = :stateJson, "
            "PUBLISHED_AT = TO_DATE(:publishedAt, 'YYYY-MM-DD HH24:MI:SS'), "
            "NESTING_VALIDATED_AT = TO_DATE(:nestingValidatedAt, 'YYYY-MM-DD HH24:MI:SS'), "
            "ATELIER_VALIDATED_AT = TO_DATE(:atelierValidatedAt, 'YYYY-MM-DD HH24:MI:SS'), "
            "STOCK_CONSUMED_AT = TO_DATE(:stockConsumedAt, 'YYYY-MM-DD HH24:MI:SS'), "
            "CONSUMED_SHEET_QTY = :consumedSheetQty, "
            "UPDATED_AT = SYSDATE "
            "WHERE IDMODELE = :idModele AND IDBOIS = :idBois");
    } else {
        query.prepare(
            "INSERT INTO PIECE_WORKFLOW_STATE "
            "(IDMODELE, IDBOIS, CURRENT_STAGE, DIRTY, STATE_JSON, PUBLISHED_AT, "
            "NESTING_VALIDATED_AT, ATELIER_VALIDATED_AT, STOCK_CONSUMED_AT, CONSUMED_SHEET_QTY, CREATED_AT, UPDATED_AT) "
            "VALUES "
            "(:idModele, :idBois, :currentStage, :dirty, :stateJson, :publishedAt, "
            ":nestingValidatedAt, :atelierValidatedAt, :stockConsumedAt, :consumedSheetQty, SYSDATE, SYSDATE)");
    }

    query.bindValue(":idModele", state.modeleId);
    query.bindValue(":idBois", state.boisId);
    query.bindValue(":currentStage", workflowStageToString(state.currentStage));
    query.bindValue(":dirty", state.dirty ? 1 : 0);
    query.bindValue(":stateJson", state.stateJson);
    query.bindValue(":publishedAt", toOracleDateTimeString(state.publishedAt));
    query.bindValue(":nestingValidatedAt", toOracleDateTimeString(state.nestingValidatedAt));
    query.bindValue(":atelierValidatedAt", toOracleDateTimeString(state.atelierValidatedAt));
    query.bindValue(":stockConsumedAt", toOracleDateTimeString(state.stockConsumedAt));
    query.bindValue(":consumedSheetQty", state.consumedSheetQty);

    if (!query.exec()) {
        errorMessage = query.lastError().text();
        return false;
    }

    return true;
}

bool PieceWorkflowStateRepository::clearWorkflowState(int modeleId,
                                                      int boisId,
                                                      QString &errorMessage)
{
    errorMessage.clear();
    if (!ensureStorage(&errorMessage)) {
        return false;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM PIECE_WORKFLOW_STATE WHERE IDMODELE = :idModele AND IDBOIS = :idBois");
    query.bindValue(":idModele", modeleId);
    query.bindValue(":idBois", boisId);
    if (!query.exec()) {
        errorMessage = query.lastError().text();
        return false;
    }
    return true;
}

bool PieceWorkflowStateRepository::validateAtelierAndConsumeStock(int modeleId,
                                                                  int boisId,
                                                                  QString &errorMessage)
{
    errorMessage.clear();
    if (!ensureStorage(&errorMessage)) {
        return false;
    }

    PieceWorkflowState state = loadWorkflowState(modeleId, boisId, &errorMessage);
    if (!state.isValidContext()) {
        errorMessage = "Contexte atelier invalide.";
        return false;
    }
    if (state.currentStage != PieceWorkflowStage::NestingValidated) {
        errorMessage = "Le dossier atelier n'est pas pret: le nesting courant doit etre regenere et valide avant consommation du stock.";
        return false;
    }
    if (state.stockConsumedAt.isValid()) {
        errorMessage = "Le stock a deja ete consomme pour ce contexte.";
        return false;
    }
    const QVector<PieceDraft> publishedPieces = PieceRepository::loadPieces(modeleId,
                                                                            boisId,
                                                                            {PieceStatus::Published},
                                                                            &errorMessage);
    if (!errorMessage.isEmpty()) {
        return false;
    }
    if (publishedPieces.isEmpty()) {
        if (errorMessage.isEmpty()) {
            errorMessage = "Aucune piece publiee pour ce contexte.";
        }
        return false;
    }

    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(state.stateJson.toUtf8(), &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        errorMessage = "Le workflow atelier ne contient pas de resultat de nesting exploitable.";
        return false;
    }

    const QJsonObject root = doc.object();
    const QString manualPlan = root.value("manualPlan").toString().trimmed();
    if (manualPlan.isEmpty()) {
        errorMessage = "Le plan manuel est vide. Finalisez d'abord le dossier atelier.";
        return false;
    }

    const QString publishedSignature = PieceWorkflowService::buildWorkingSetSignature(publishedPieces);
    const QString nestingSignature = workflowSignatureValue(root, "nestingSignature");
    if (nestingSignature.isEmpty()) {
        errorMessage = "Le resultat de nesting courant n'est pas signe. Recalculez le plan avant la validation atelier.";
        return false;
    }
    if (!publishedSignature.isEmpty() && nestingSignature != publishedSignature) {
        errorMessage = "Le nesting courant ne correspond plus au catalogue publie. Recalculez le plan avant la validation atelier.";
        return false;
    }

    const int usedSheets = sheetCountFromJson(root);
    const int unplacedCount = unplacedCountFromJson(root);
    if (usedSheets <= 0) {
        errorMessage = "Aucune plaque valide n'est disponible dans le resultat de nesting.";
        return false;
    }
    if (unplacedCount > 0) {
        errorMessage = QString("Le nesting reste incomplet: %1 piece(s) non placee(s).").arg(unplacedCount);
        return false;
    }

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isValid()) {
        errorMessage = "Connexion base de donnees indisponible.";
        return false;
    }
    if (!db.transaction()) {
        errorMessage = "Impossible de demarrer la validation atelier.";
        return false;
    }

    QSqlQuery stockQuery(db);
    stockQuery.prepare("SELECT NVL(QUANTITE, 0) FROM BOIS WHERE IDBOIS = :idBois");
    stockQuery.bindValue(":idBois", boisId);
    if (!stockQuery.exec() || !stockQuery.next()) {
        db.rollback();
        errorMessage = stockQuery.lastError().text().trimmed().isEmpty()
            ? QString("Bois introuvable.")
            : stockQuery.lastError().text();
        return false;
    }

    const int availableSheets = stockQuery.value(0).toInt();
    if (availableSheets < usedSheets) {
        db.rollback();
        errorMessage = QString("Stock insuffisant: %1 plaque(s) requise(s), %2 disponible(s).")
                           .arg(usedSheets)
                           .arg(availableSheets);
        return false;
    }

    QSqlQuery updateStockQuery(db);
    updateStockQuery.prepare(
        "UPDATE BOIS SET QUANTITE = NVL(QUANTITE, 0) - :usedSheets WHERE IDBOIS = :idBois");
    updateStockQuery.bindValue(":usedSheets", usedSheets);
    updateStockQuery.bindValue(":idBois", boisId);
    if (!updateStockQuery.exec()) {
        db.rollback();
        errorMessage = updateStockQuery.lastError().text();
        return false;
    }

    QSqlQuery updateStateQuery(db);
    updateStateQuery.prepare(
        "UPDATE PIECE_WORKFLOW_STATE SET "
        "CURRENT_STAGE = 'ATELIER_VALIDE', "
        "DIRTY = 0, "
        "NESTING_VALIDATED_AT = NVL(NESTING_VALIDATED_AT, SYSDATE), "
        "ATELIER_VALIDATED_AT = SYSDATE, "
        "STOCK_CONSUMED_AT = SYSDATE, "
        "CONSUMED_SHEET_QTY = :consumedSheetQty, "
        "UPDATED_AT = SYSDATE "
        "WHERE IDMODELE = :idModele AND IDBOIS = :idBois");
    updateStateQuery.bindValue(":consumedSheetQty", usedSheets);
    updateStateQuery.bindValue(":idModele", modeleId);
    updateStateQuery.bindValue(":idBois", boisId);
    if (!updateStateQuery.exec()) {
        db.rollback();
        errorMessage = updateStateQuery.lastError().text();
        return false;
    }

    if (!db.commit()) {
        errorMessage = db.lastError().text();
        db.rollback();
        return false;
    }

    return true;
}

QString PieceWorkflowService::buildWorkingSetSignature(const QVector<PieceDraft> &drafts)
{
    if (drafts.isEmpty()) {
        return QString();
    }

    QVector<PieceDraft> orderedDrafts = drafts;
    std::sort(orderedDrafts.begin(),
              orderedDrafts.end(),
              [](const PieceDraft &left, const PieceDraft &right) {
                  const QString leftKey = normalizeCodeToken(left.code) + "|" + normalizeWorkflowText(left.nom);
                  const QString rightKey = normalizeCodeToken(right.code) + "|" + normalizeWorkflowText(right.nom);
                  if (leftKey == rightKey) {
                      if (left.quantite == right.quantite) {
                          return left.ordreAffichage < right.ordreAffichage;
                      }
                      return left.quantite < right.quantite;
                  }
                  return leftKey < rightKey;
              });

    QStringList lines;
    lines.reserve(orderedDrafts.size());
    for (const PieceDraft &draft : orderedDrafts) {
        lines << QString("%1|%2|%3|%4|%5|%6|%7")
                     .arg(normalizeCodeToken(draft.code),
                          normalizeWorkflowText(draft.nom),
                          QString::number(draft.quantite),
                          normalizeWorkflowText(draft.matiere),
                          QString::number(draft.longueurMm, 'f', 3),
                          QString::number(draft.largeurMm, 'f', 3),
                          QString::number(draft.epaisseurMm, 'f', 3));
    }

    return QString::fromLatin1(QCryptographicHash::hash(lines.join('\n').toUtf8(),
                                                        QCryptographicHash::Sha256)
                                   .toHex());
}

PieceAtelierReadiness PieceWorkflowService::evaluateAtelierReadiness(const QVector<PieceDraft> &drafts,
                                                                     const QString &manualPlan,
                                                                     int passes,
                                                                     bool axisLocked,
                                                                     bool keepOffcuts)
{
    PieceAtelierReadiness readiness;
    const QString plan = manualPlan.trimmed();
    if (plan.isEmpty()) {
        readiness.verdict = "A revoir";
        readiness.alerts << "Le plan manuel est vide.";
        return readiness;
    }

    const QStringList steps = plan.split(QRegularExpression("[\\r\\n]+"), Qt::SkipEmptyParts);
    QSet<QString> mentionedCodes;
    bool mentionsLabeling = false;
    bool mentionsControl = false;
    bool mentionsZero = false;
    bool mentionsOffcuts = false;
    bool mentionsRough = false;
    bool mentionsFinal = false;
    const QString normalizedPlan = normalizeWorkflowText(plan);

    for (const QString &step : steps) {
        const QString normalizedStep = normalizeWorkflowText(step);
        if (normalizedStep.contains("ETIQUET") || normalizedStep.contains("MARQU") || normalizedStep.contains("CODE")) {
            mentionsLabeling = true;
        }
        if (normalizedStep.contains("CONTROLE") || normalizedStep.contains("VERIF")
            || normalizedStep.contains("MESURE") || normalizedStep.contains("EQUERR")) {
            mentionsControl = true;
        }
        if (normalizedStep.contains("POINT ZERO") || normalizedStep.contains("ORIGINE") || normalizedStep.contains("REPERE")) {
            mentionsZero = true;
        }
        if (normalizedStep.contains("CHUTE") || normalizedStep.contains("REEMPLOI") || normalizedStep.contains("STOCKER")) {
            mentionsOffcuts = true;
        }
        if (normalizedStep.contains("BRUT") || normalizedStep.contains("ROUGH") || normalizedStep.contains("PRE DEBIT")) {
            mentionsRough = true;
        }
        if (normalizedStep.contains("FINI") || normalizedStep.contains("FINAL") || normalizedStep.contains("MISE A COTE")) {
            mentionsFinal = true;
        }
    }

    for (const PieceDraft &draft : drafts) {
        const QString normalizedCode = normalizeCodeToken(draft.code);
        if (!normalizedCode.isEmpty() && normalizedPlan.contains(normalizedCode, Qt::CaseInsensitive)) {
            mentionedCodes.insert(normalizedCode);
        }
    }

    if (steps.size() >= qMax(1, passes)) {
        readiness.score += 22;
        readiness.strengths << "Le nombre d'etapes couvre les passes configurees.";
    } else {
        readiness.alerts << "Le nombre d'etapes est inferieur au nombre de passes prevu.";
    }

    if (drafts.isEmpty() || mentionedCodes.size() >= qMin(drafts.size(), qMax(1, drafts.size() / 2))) {
        readiness.score += 18;
        readiness.strengths << "Les references critiques sont citees dans le plan.";
    } else {
        readiness.alerts << "Trop peu de references ou de codes pieces sont cites dans le plan.";
    }

    if (mentionsLabeling) {
        readiness.score += 14;
        readiness.strengths << "Le plan mentionne le marquage ou l'etiquetage des pieces.";
    } else {
        readiness.alerts << "Ajouter une etape de marquage / etiquetage des pieces.";
    }

    if (mentionsControl) {
        readiness.score += 14;
        readiness.strengths << "Le plan inclut un controle qualite ou une verification dimensionnelle.";
    } else {
        readiness.alerts << "Ajouter un controle de cote finale, equerrage ou verification atelier.";
    }

    if (mentionsZero) {
        readiness.score += 10;
        readiness.strengths << "Le point zero ou l'origine de coupe est defini.";
    } else {
        readiness.alerts << "Preciser un point zero ou une origine stable avant decoupe.";
    }

    if (mentionsRough && mentionsFinal) {
        readiness.score += 10;
        readiness.strengths << "Le plan distingue les etapes BRUT et FINI.";
    } else {
        readiness.alerts << "Le plan devrait distinguer les etapes BRUT et FINI.";
    }

    if (!keepOffcuts || mentionsOffcuts) {
        readiness.score += 6;
        readiness.strengths << "La gestion des chutes est couverte ou non requise.";
    } else {
        readiness.alerts << "Les chutes reutilisables doivent etre mentionnees dans le plan.";
    }

    if (axisLocked) {
        readiness.score += 6;
        readiness.strengths << "Les axes de coupe sont verrouilles pour fiabiliser l'atelier.";
    } else {
        readiness.alerts << "Verifier si un verrouillage d'axes est necessaire pour l'atelier.";
    }

    if (readiness.score >= 85) {
        readiness.verdict = "Pret atelier";
    } else if (readiness.score >= 65) {
        readiness.verdict = "Exploitable avec reserve";
    } else {
        readiness.verdict = "A revoir";
    }
    readiness.canValidate = readiness.score >= 65;
    return readiness;
}

bool PieceWorkflowService::validateDraft(const PieceDraft &draft,
                                         QString &errorMessage)
{
    if (!Piece::validerNomPiece(draft.nom)) {
        errorMessage = "Nom de piece invalide dans le brouillon.";
        return false;
    }
    if (!Piece::validerCodePiece(draft.code)) {
        errorMessage = QString("Code de piece invalide pour '%1'.").arg(draft.nom.trimmed());
        return false;
    }
    if (!Piece::validerQuantite(draft.quantite)) {
        errorMessage = QString("Quantite invalide pour '%1' (doit etre > 0).").arg(draft.nom.trimmed());
        return false;
    }
    if (!Piece::validerMatiere(draft.matiere)) {
        errorMessage = QString("Matiere invalide pour '%1'.").arg(draft.nom.trimmed());
        return false;
    }
    if (!Piece::validerDimension(draft.longueurMm)
        || !Piece::validerDimension(draft.largeurMm)
        || !Piece::validerDimension(draft.epaisseurMm)) {
        errorMessage = QString("Dimensions invalides pour '%1'.").arg(draft.nom.trimmed());
        return false;
    }
    return true;
}

bool PieceWorkflowService::validateDrafts(const QVector<PieceDraft> &drafts,
                                          QString &errorMessage)
{
    errorMessage.clear();
    if (drafts.isEmpty()) {
        errorMessage = "Aucune piece brouillon disponible pour ce contexte.";
        return false;
    }

    QSet<QString> seenCodes;
    for (const PieceDraft &draft : drafts) {
        if (!validateDraft(draft, errorMessage)) {
            return false;
        }
        const QString normalizedCode = normalizeCodeToken(draft.code);
        if (seenCodes.contains(normalizedCode)) {
            errorMessage = QString("Le code '%1' est duplique dans le contexte courant.").arg(normalizedCode);
            return false;
        }
        seenCodes.insert(normalizedCode);
    }
    return true;
}
