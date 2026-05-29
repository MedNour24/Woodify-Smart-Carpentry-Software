#include "employee.h"

#include <QDebug>
#include <QCryptographicHash>
#include <QFileInfo>
#include <QFile>
#include <QRandomGenerator>
#include <QRegularExpression>
#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSet>
#include <QStringList>
#include <QTextStream>

#include <algorithm>
#include <cmath>

namespace {
QString g_employeeAuditActor;

QString canonicalRoleValue(const QString &role)
{
    QString r = role.trimmed().toUpper();
    if (r == "INGENIEUR" || r == "INGÉNIEUR") {
        return "ENGINEER";
    }
    if (r == "CHEF ATELIER") {
        return "CHEF_ATELIER";
    }
    if (r == "MENUISIER") {
        return "MENUISIER";
    }
    if (r == "DESIGNER") {
        return "DESIGNER";
    }
    if (r == "ADMIN") {
        return "ADMIN";
    }
    if (r == "RH") {
        return "RH";
    }
    return r;
}

int resolveAuditActorId()
{
    const QString cleanActor = g_employeeAuditActor.trimmed();
    if (!cleanActor.isEmpty()) {
        bool ok = false;
        const int numericId = cleanActor.toInt(&ok);
        if (ok && numericId > 0) {
            return numericId;
        }

        QSqlQuery actorQuery;
        actorQuery.prepare(
            "SELECT IDEMPLOYEE "
            "FROM EMPLOYEE "
            "WHERE LOWER(MAIL) = LOWER(:actor) OR LOWER(NOM || ' ' || PRENOM) = LOWER(:actor) "
            "ORDER BY IDEMPLOYEE");
        actorQuery.bindValue(":actor", cleanActor);
        if (actorQuery.exec() && actorQuery.next()) {
            return actorQuery.value(0).toInt();
        }
    }
    return 1;
}

void ensureAuditLogTable()
{
    QSqlQuery query;
    query.exec(
        "BEGIN "
        "EXECUTE IMMEDIATE 'CREATE TABLE AUDIT_LOG ("
        "ID_LOG NUMBER PRIMARY KEY, "
        "TABLE_NAME VARCHAR2(50) NOT NULL, "
        "RECORD_ID NUMBER NOT NULL, "
        "ACTION_TYPE VARCHAR2(64) NOT NULL, "
        "CHAMP_MODIFIE VARCHAR2(100), "
        "OLD_VALUE VARCHAR2(1000), "
        "NEW_VALUE VARCHAR2(1000), "
        "ACTION_BY NUMBER NOT NULL, "
        "ACTION_AT TIMESTAMP DEFAULT CURRENT_TIMESTAMP, "
        "DETAILS VARCHAR2(1000), "
        "CONSTRAINT FK_AUDIT_EMPLOYEE FOREIGN KEY (ACTION_BY) REFERENCES EMPLOYEE(IDEMPLOYEE) ON DELETE CASCADE)'; "
        "EXCEPTION WHEN OTHERS THEN IF SQLCODE != -955 THEN RAISE; END IF; "
        "END;");
}

void logEmployeeHistory(int idEmployee, const QString &action, const QString &details)
{
    ensureAuditLogTable();

    const int actorId = resolveAuditActorId();
    const int recordId = (idEmployee > 0) ? idEmployee : 0;

    QSqlQuery query;
    query.prepare(
        "INSERT INTO AUDIT_LOG (ID_LOG, TABLE_NAME, RECORD_ID, ACTION_TYPE, ACTION_BY, ACTION_AT, DETAILS) "
        "SELECT NVL(MAX(ID_LOG), 0) + 1, 'EMPLOYEE', :recordId, :action, :actionBy, SYSTIMESTAMP, :details "
        "FROM AUDIT_LOG");
    query.bindValue(":recordId", recordId);
    query.bindValue(":action", action);
    query.bindValue(":actionBy", actorId);
    query.bindValue(":details", details.left(1000));

    if (query.exec()) {
        return;
    }
    qWarning() << "Erreur logEmployeeHistory:" << query.lastError().text();
}

bool isPhotoPathValid(const QString &photoPath)
{
    const QString path = photoPath.trimmed();
    if (path.isEmpty()) {
        return true;
    }
    if (path.startsWith(":/")) {
        return true;
    }
    return QFileInfo::exists(path);
}

QString normalizeFaceIdValue(const QString &value)
{
    const QString trimmed = value.trimmed();
    if (trimmed.isEmpty()) {
        return QString();
    }

    // FACE_ID is a short identifier; image file paths are stored in PHOTO_PATH.
    if (trimmed.startsWith(":/") || trimmed.contains('/') || trimmed.contains('\\')) {
        return QString();
    }

    if (trimmed.length() > 100) {
        return QString();
    }

    return trimmed;
}

QChar detectCsvSeparator(const QString &line)
{
    int semicolonCount = 0;
    int commaCount = 0;
    bool inQuotes = false;

    for (int i = 0; i < line.size(); ++i) {
        const QChar c = line.at(i);
        if (c == '"') {
            if (inQuotes && (i + 1) < line.size() && line.at(i + 1) == '"') {
                ++i;
                continue;
            }
            inQuotes = !inQuotes;
            continue;
        }

        if (inQuotes) {
            continue;
        }

        if (c == ';') {
            ++semicolonCount;
        } else if (c == ',') {
            ++commaCount;
        }
    }

    return (semicolonCount >= commaCount) ? ';' : ',';
}

QStringList parseCsvLineQuoted(const QString &line, QChar separator, bool *hasUnbalancedQuotes = nullptr)
{
    QStringList fields;
    QString current;
    bool inQuotes = false;

    for (int i = 0; i < line.size(); ++i) {
        const QChar c = line.at(i);

        if (c == '"') {
            if (inQuotes && (i + 1) < line.size() && line.at(i + 1) == '"') {
                current += '"';
                ++i;
            } else {
                inQuotes = !inQuotes;
            }
            continue;
        }

        if (!inQuotes && c == separator) {
            fields << current.trimmed();
            current.clear();
            continue;
        }

        current += c;
    }

    fields << current.trimmed();
    if (hasUnbalancedQuotes) {
        *hasUnbalancedQuotes = inQuotes;
    }
    return fields;
}

QDate parseCsvDateFlexible(const QString &rawValue)
{
    const QString value = rawValue.trimmed();
    if (value.isEmpty()) {
        return QDate();
    }

    static const QStringList formats = {
        "dd/MM/yyyy",
        "yyyy-MM-dd",
        "dd-MM-yyyy",
        "d/M/yyyy"
    };

    for (const QString &fmt : formats) {
        const QDate parsed = QDate::fromString(value, fmt);
        if (parsed.isValid()) {
            return parsed;
        }
    }

    return QDate();
}

bool isStoredPasswordHash(const QString &value)
{
    return value.startsWith("sha256$");
}

QString hashPasswordForStorage(const QString &plainPassword)
{
    if (isStoredPasswordHash(plainPassword)) {
        return plainPassword;
    }

    QByteArray salt(16, '\0');
    for (int i = 0; i < salt.size(); ++i) {
        salt[i] = static_cast<char>(QRandomGenerator::global()->bounded(256));
    }

    const QByteArray digest = QCryptographicHash::hash(salt + plainPassword.toUtf8(),
                                                       QCryptographicHash::Sha256);

    return QString("sha256$%1$%2")
        .arg(QString::fromLatin1(salt.toHex()), QString::fromLatin1(digest.toHex()));
}

QString generateTemporaryImportPassword()
{
    static const QString charset = QStringLiteral("ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz23456789!@#$%");
    QString temp;
    temp.reserve(12);
    for (int i = 0; i < 12; ++i) {
        const int idx = QRandomGenerator::global()->bounded(charset.size());
        temp.append(charset.at(idx));
    }
    return temp;
}
}

// ============================================================
//  Constructors
// ============================================================

Employee::Employee()
    : idEmployee(0),
      CIN(),
      mail(),
      password(),
      nom(),
      prenom(),
      date_naissance(),
    telephone(),
    photo_path(),
    date_embauche(),
    type_contrat(),
    salaire_base(0.0),
    statut(),
    niveau(),
      face_id(),
      face_ts(),
      role(),
      classification_x(0.0),
      classification_y(0.0),
      lastErrMsg()
{}

Employee::Employee(int idEmployee, const QString &CIN, const QString &mail,
                   const QString &password, const QString &nom, const QString &prenom,
                   const QDate &date_naissance, const QString &face_id,
                   const QDateTime &face_ts, const QString &role,
                   double classification_x, double classification_y)
    : idEmployee(idEmployee),
      CIN(CIN),
      mail(mail),
      password(password),
      nom(nom),
      prenom(prenom),
      date_naissance(date_naissance),
            telephone(),
            photo_path(),
            date_embauche(),
            type_contrat(),
            salaire_base(0.0),
            statut(),
            niveau(),
      face_id(face_id),
      face_ts(face_ts),
      role(role),
      classification_x(classification_x),
      classification_y(classification_y)
{}

// ============================================================
//  Getters
// ============================================================

int      Employee::getIdEmployee()     const { return idEmployee; }
QString  Employee::getCIN()            const { return CIN; }
QString  Employee::getMail()           const { return mail; }
QString  Employee::getPassword()       const { return password; }
QString  Employee::getNom()            const { return nom; }
QString  Employee::getPrenom()         const { return prenom; }
QDate    Employee::getDateNaissance()  const { return date_naissance; }
QString  Employee::getTelephone()      const { return telephone; }
QString  Employee::getPhotoPath()      const { return photo_path; }
QDate    Employee::getDateEmbauche()   const { return date_embauche; }
QString  Employee::getTypeContrat()    const { return type_contrat; }
double   Employee::getSalaireBase()    const { return salaire_base; }
QString  Employee::getStatut()         const { return statut; }
QString  Employee::getNiveau()         const { return niveau; }
QString  Employee::getFaceId()         const { return face_id; }
QDateTime Employee::getFaceTs()        const { return face_ts; }
QString  Employee::getRole()           const { return role; }
double   Employee::getClassificationX()const { return classification_x; }
double   Employee::getClassificationY()const { return classification_y; }
QString  Employee::getLastError()      const { return lastErrMsg; }

// ============================================================
//  Setters
// ============================================================

void Employee::setIdEmployee(int id)                  { idEmployee = id; }
void Employee::setCIN(const QString &cin)             { CIN = cin; }
void Employee::setMail(const QString &m)              { mail = m; }
void Employee::setPassword(const QString &p)          { password = p; }
void Employee::setNom(const QString &n)               { nom = n; }
void Employee::setPrenom(const QString &p)             { prenom = p; }
void Employee::setDateNaissance(const QDate &d)        { date_naissance = d; }
void Employee::setTelephone(const QString &t)          { telephone = t; }
void Employee::setPhotoPath(const QString &p)          { photo_path = p; }
void Employee::setDateEmbauche(const QDate &d)         { date_embauche = d; }
void Employee::setTypeContrat(const QString &c)        { type_contrat = c; }
void Employee::setSalaireBase(double s)                { salaire_base = s; }
void Employee::setStatut(const QString &s)             { statut = s; }
void Employee::setNiveau(const QString &n)             { niveau = n; }
void Employee::setFaceId(const QString &fid)           { face_id = fid; }
void Employee::setFaceTs(const QDateTime &fts)         { face_ts = fts; }
void Employee::setRole(const QString &r)               { role = canonicalRoleValue(r); }
void Employee::setClassificationX(double x)            { classification_x = x; }
void Employee::setClassificationY(double y)            { classification_y = y; }

// ============================================================
//  Allowed roles (matches CHECK constraint)
// ============================================================

QStringList Employee::rolesAutorises()
{
    return {"ADMIN", "DESIGNER", "CHEF_ATELIER", "ENGINEER", "RH", "MENUISIER"};
}

void Employee::setAuditActor(const QString &actor)
{
    g_employeeAuditActor = actor.trimmed();
}

// ============================================================
//  Input Validation
// ============================================================

bool Employee::validerCIN(const QString &cin)
{
    static QRegularExpression rx("^[0-9]{8}$");
    return !cin.trimmed().isEmpty() && rx.match(cin.trimmed()).hasMatch();
}

bool Employee::validerMail(const QString &mail)
{
    static QRegularExpression rx(R"(^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$)");
    return !mail.trimmed().isEmpty()
           && mail.trimmed().length() <= 100
           && rx.match(mail.trimmed()).hasMatch();
}

bool Employee::validerPassword(const QString &password)
{
    QString p = password.trimmed();
    return p.length() >= 4 && p.length() <= 255;
}

bool Employee::validerNom(const QString &nom)
{
    static QRegularExpression rx(R"(^[A-Za-zÀ-ÿ\s\-']{1,50}$)");
    return !nom.trimmed().isEmpty() && rx.match(nom.trimmed()).hasMatch();
}

bool Employee::validerPrenom(const QString &prenom)
{
    static QRegularExpression rx(R"(^[A-Za-zÀ-ÿ\s\-']{1,50}$)");
    return !prenom.trimmed().isEmpty() && rx.match(prenom.trimmed()).hasMatch();
}

bool Employee::validerRole(const QString &role)
{
    return rolesAutorises().contains(canonicalRoleValue(role));
}

bool Employee::validerDateNaissance(const QDate &date)
{
    if (!date.isValid()) return false;
    QDate today = QDate::currentDate();
    int age = today.year() - date.year();
    if (today.month() < date.month()
        || (today.month() == date.month() && today.day() < date.day())) {
        age--;
    }
    return age >= 18 && age <= 100;
}



// ============================================================
//  CRUD — Ajouter (CREATE)
// ============================================================

bool Employee::ajouter()
{
    lastErrMsg.clear();

    const QString normalizedRole = canonicalRoleValue(role);
    const QString normalizedContract = type_contrat.trimmed().toUpper();
    const QDate effectiveHireDate = date_embauche;
    const QString effectivePhotoPath = photo_path.trimmed();
    const QString normalizedFaceId = normalizeFaceIdValue(face_id);

    const QStringList allowedContracts = {"CDI", "CDD", "STAGE", "INTERIM"};

    // ------ Contrôle de saisie ------
    if (!validerCIN(CIN)) {
        lastErrMsg = "CIN invalide (8 chiffres attendus).";
        return false;
    }
    if (!validerMail(mail)) {
        lastErrMsg = "Adresse mail invalide.";
        return false;
    }
    if (!validerPassword(password)) {
        lastErrMsg = "Mot de passe invalide (4-255 caractères).";
        return false;
    }
    const QString passwordToStore = hashPasswordForStorage(password);
    if (!validerNom(nom)) {
        lastErrMsg = "Nom invalide.";
        return false;
    }
    if (!validerPrenom(prenom)) {
        lastErrMsg = "Prénom invalide.";
        return false;
    }
    if (!validerRole(normalizedRole)) {
        lastErrMsg = "Rôle invalide.";
        return false;
    }
    if (!normalizedContract.isEmpty() && !allowedContracts.contains(normalizedContract)) {
        lastErrMsg = "Type de contrat invalide.";
        return false;
    }
    if (salaire_base < 0.0) {
        lastErrMsg = "Salaire de base invalide.";
        return false;
    }
    if (telephone.trimmed().length() > 20) {
        lastErrMsg = "Téléphone invalide (20 caractères max).";
        return false;
    }
    if (!validerDateNaissance(date_naissance)) {
        lastErrMsg = "Date de naissance invalide (�ge entre 18 et 100 ans).";
        return false;
    }
    if (!isPhotoPathValid(effectivePhotoPath)) {
        lastErrMsg = "Photo invalide: le fichier sélectionné n'existe pas.";
        return false;
    }

    // ------ Vérifier l'unicité CIN et Email ------
    QSqlQuery checkQuery;
    
    // Vérifier si le CIN existe déjà
    checkQuery.prepare("SELECT COUNT(*) FROM EMPLOYEE WHERE CIN = :cin");
    checkQuery.bindValue(":cin", CIN);
    if (!checkQuery.exec() || !checkQuery.next()) {
        lastErrMsg = "Erreur lors de la vérification du CIN.";
        return false;
    }
    if (checkQuery.value(0).toInt() > 0) {
        lastErrMsg = "Ce CIN existe déjà dans la base de données.";
        return false;
    }
    
    // Vérifier si l'email existe déjà
    checkQuery.prepare("SELECT COUNT(*) FROM EMPLOYEE WHERE MAIL = :mail");
    checkQuery.bindValue(":mail", mail);
    if (!checkQuery.exec() || !checkQuery.next()) {
        lastErrMsg = "Erreur lors de la vérification de l'email.";
        return false;
    }
    if (checkQuery.value(0).toInt() > 0) {
        lastErrMsg = "Cette adresse email existe déjà dans la base de données.";
        return false;
    }

    // ------ Auto-generate ID from Oracle sequence ------
    QSqlQuery queryId;
    queryId.prepare("SELECT EMP_SEQ.NEXTVAL FROM DUAL");
    if (!queryId.exec() || !queryId.next()) {
        lastErrMsg = "Impossible de générer l'ID via la sequence EMP_SEQ.";
        return false;
    }
    int autoId = queryId.value(0).toInt();

    // ------ Requête SQL ------
    QSqlQuery query;

    query.prepare(
        "INSERT INTO EMPLOYEE "
        "(IDEMPLOYEE, CIN, MAIL, PASSWORD, NOM, PRENOM, TELEPHONE, PHOTO_PATH, DATE_NAISSANCE, DATE_EMBAUCHE, TYPE_CONTRAT, SALAIRE_BASE, FACE_ID, FACE_TS, ROLE) "
        "VALUES "
        "(:idEmployee, :CIN, :mail, :password, :nom, :prenom, :telephone, :photo_path, :date_naissance, :date_embauche, :type_contrat, :salaire_base, :face_id, :face_ts, :role)"
    );

    query.bindValue(":idEmployee",     autoId);
    query.bindValue(":CIN",            CIN);
    query.bindValue(":mail",           mail);
    query.bindValue(":password",       passwordToStore);
    query.bindValue(":nom",            nom);
    query.bindValue(":prenom",         prenom);
    query.bindValue(":telephone",      telephone.trimmed());
    query.bindValue(":photo_path",     effectivePhotoPath);
    query.bindValue(":date_naissance", date_naissance);
    query.bindValue(":date_embauche",  effectiveHireDate.isValid()
                                            ? QVariant(effectiveHireDate)
                                            : QVariant(QMetaType::fromType<QDate>()));
    query.bindValue(":type_contrat",   normalizedContract);
    query.bindValue(":salaire_base",   salaire_base);
    query.bindValue(":face_id",        normalizedFaceId);
    query.bindValue(":face_ts",        normalizedFaceId.isEmpty() ? QVariant(QMetaType::fromType<QDateTime>())
                                                                     : QVariant(QDateTime::currentDateTime()));
    query.bindValue(":role",           normalizedRole);

    if (!query.exec()) {
        QSqlError error = query.lastError();
        lastErrMsg = error.text();
        
        // Analyse plus précise de l'erreur pour des messages plus clairs
        QString errorText = error.text().toUpper();
        if (errorText.contains("UNIQUE CONSTRAINT") && errorText.contains("CIN")) {
            lastErrMsg = "Ce CIN existe déjà dans la base de données.";
        } else if (errorText.contains("UNIQUE CONSTRAINT") && errorText.contains("MAIL")) {
            lastErrMsg = "Cette adresse email existe déjà dans la base de données.";
        } else if (errorText.contains("UNIQUE CONSTRAINT")) {
            lastErrMsg = "Une valeur dupliquée a été détectée. Veuillez vérifier le CIN et l'email.";
        }
        
        qDebug() << "Erreur Ajouter Employee:" << lastErrMsg;
        return false;
    }

    // Mettre à jour l'ID de l'objet avec l'ID généré
    this->idEmployee = autoId;
    recomputeClassification2D(autoId);
    logEmployeeHistory(autoId,
                       "CREATE",
                       QString("Création employé %1 %2, rôle %3").arg(prenom, nom, role.toUpper()));
    qDebug() << "Employee ajouté avec succès (id:" << autoId << ")";
    return true;
}

// ============================================================
//  CRUD — Afficher (READ)
// ============================================================

QSqlQueryModel* Employee::afficher(bool includeArchived)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    const QString archivedFilter = includeArchived
        ? QStringLiteral("WHERE NVL(UPPER(STATUT), 'ACTIF') = 'ARCHIVE' ")
        : QStringLiteral("WHERE NVL(UPPER(STATUT), 'ACTIF') <> 'ARCHIVE' ");
    model->setQuery(
        "SELECT IDEMPLOYEE, CIN, MAIL, NOM, PRENOM, NVL(TELEPHONE, ''), "
        "TO_CHAR(DATE_NAISSANCE, 'DD/MM/YYYY') AS DATE_NAISSANCE, "
        "TO_CHAR(DATE_EMBAUCHE, 'DD/MM/YYYY') AS DATE_EMBAUCHE, "
        "NVL(TYPE_CONTRAT, ''), NVL(TO_CHAR(SALAIRE_BASE), '0'), ROLE "
        "FROM EMPLOYEE "
        + archivedFilter +
        "ORDER BY IDEMPLOYEE"
    );

    if (model->lastError().isValid()) {
        qDebug() << "Erreur Afficher Employee:" << model->lastError().text();
    }

    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "CIN");
    model->setHeaderData(2, Qt::Horizontal, "Mail");
    model->setHeaderData(3, Qt::Horizontal, "Nom");
    model->setHeaderData(4, Qt::Horizontal, "Prénom");
    model->setHeaderData(5, Qt::Horizontal, "Telephone");
    model->setHeaderData(6, Qt::Horizontal, "Date Naissance");
    model->setHeaderData(7, Qt::Horizontal, "Date Embauche");
    model->setHeaderData(8, Qt::Horizontal, "Type contrat");
    model->setHeaderData(9, Qt::Horizontal, "Salaire base");
    model->setHeaderData(10, Qt::Horizontal, "Role");

    return model;
}

// ============================================================
//  CRUD — Archiver (soft delete)
// ============================================================

bool Employee::supprimer(int idEmployee)
{
    lastErrMsg.clear();

    if (idEmployee <= 0) {
        lastErrMsg = "ID Employee invalide.";
        return false;
    }

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isValid() || !db.isOpen()) {
        lastErrMsg = "Erreur de connexion à la base de données.";
        return false;
    }

    QSqlQuery currentQuery(db);
    currentQuery.prepare(
        "SELECT NVL(UPPER(STATUT), 'ACTIF'), NVL(IDFABRICATION, 0) "
        "FROM EMPLOYEE WHERE IDEMPLOYEE = :id");
    currentQuery.bindValue(":id", idEmployee);
    if (!currentQuery.exec() || !currentQuery.next()) {
        lastErrMsg = "Aucun employee trouvé avec cet ID.";
        return false;
    }

    const QString currentStatus = currentQuery.value(0).toString().trimmed().toUpper();
    if (currentStatus == "ARCHIVE") {
        lastErrMsg = "Cet employé est déjà archivé.";
        return false;
    }

    const int currentFabricationId = currentQuery.value(1).toInt();

    const bool transactionStarted = db.transaction();

    QSqlQuery query(db);
    query.prepare(
        "UPDATE EMPLOYEE SET "
        "STATUT = 'ARCHIVE', "
        "IDFABRICATION = NULL, "
        "FACE_ID = NULL, "
        "FACE_TS = NULL "
        "WHERE IDEMPLOYEE = :id AND NVL(UPPER(STATUT), 'ACTIF') <> 'ARCHIVE'");
    query.bindValue(":id", idEmployee);

    if (!query.exec()) {
        if (transactionStarted) {
            db.rollback();
        }
        lastErrMsg = query.lastError().text();
        qDebug() << "Erreur Archiver Employee:" << lastErrMsg;
        return false;
    }

    if (query.numRowsAffected() == 0) {
        if (transactionStarted) {
            db.rollback();
        }
        lastErrMsg = "Cet employé est déjà archivé.";
        return false;
    }

    if (transactionStarted && !db.commit()) {
        db.rollback();
        lastErrMsg = "Impossible de valider l'archivage.";
        qDebug() << "Erreur commit archivage employee:" << lastErrMsg;
        return false;
    }

    logEmployeeHistory(idEmployee,
                       "ARCHIVE",
                       QString("Archivage employé (ID=%1, ancien statut=%2, fabrication=%3)")
                           .arg(idEmployee)
                           .arg(currentStatus)
                           .arg(currentFabricationId));

    qDebug() << "Employee archivé avec succès (id:" << idEmployee << ")";
    return true;
}

// ============================================================
//  CRUD — Suppression définitive (PURGE)
// ============================================================

bool Employee::supprimerDefinitivement(int idEmployee)
{
    lastErrMsg.clear();

    if (idEmployee <= 0) {
        lastErrMsg = "ID Employee invalide.";
        return false;
    }

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isValid() || !db.isOpen()) {
        lastErrMsg = "Erreur de connexion à la base de données.";
        return false;
    }

    QSqlQuery currentQuery(db);
    currentQuery.prepare(
        "SELECT NVL(UPPER(STATUT), 'ACTIF') "
        "FROM EMPLOYEE WHERE IDEMPLOYEE = :id");
    currentQuery.bindValue(":id", idEmployee);
    if (!currentQuery.exec() || !currentQuery.next()) {
        lastErrMsg = "Aucun employee trouvé avec cet ID.";
        return false;
    }

    const QString currentStatus = currentQuery.value(0).toString().trimmed().toUpper();
    if (currentStatus != "ARCHIVE") {
        lastErrMsg = "La suppression définitive est réservée aux employés archivés.";
        return false;
    }

    const bool transactionStarted = db.transaction();

    QSqlQuery query(db);
    query.prepare("DELETE FROM EMPLOYEE WHERE IDEMPLOYEE = :id AND NVL(UPPER(STATUT), 'ACTIF') = 'ARCHIVE'");
    query.bindValue(":id", idEmployee);

    if (!query.exec()) {
        if (transactionStarted) {
            db.rollback();
        }
        lastErrMsg = query.lastError().text();
        qDebug() << "Erreur suppression définitive employee:" << lastErrMsg;
        return false;
    }

    if (query.numRowsAffected() == 0) {
        if (transactionStarted) {
            db.rollback();
        }
        lastErrMsg = "Aucun employé archivé trouvé avec cet ID.";
        return false;
    }

    if (transactionStarted && !db.commit()) {
        db.rollback();
        lastErrMsg = "Impossible de valider la suppression définitive.";
        qDebug() << "Erreur commit suppression définitive employee:" << lastErrMsg;
        return false;
    }

    logEmployeeHistory(idEmployee,
                       "PURGE",
                       QString("Suppression définitive employé archivé (ID=%1)").arg(idEmployee));

    qDebug() << "Employee supprimé définitivement (id:" << idEmployee << ")";
    return true;
}

// ============================================================
//  CRUD — Modifier (UPDATE)
// ============================================================

bool Employee::modifier(int idEmployee)
{
    lastErrMsg.clear();

    const QString normalizedRole = canonicalRoleValue(role);
    const QString normalizedContract = type_contrat.trimmed().toUpper();
    const QDate effectiveHireDate = date_embauche;
    const QString effectivePhotoPath = photo_path.trimmed();
    const QString normalizedFaceId = normalizeFaceIdValue(face_id);

    const QStringList allowedContracts = {"CDI", "CDD", "STAGE", "INTERIM"};

    if (idEmployee <= 0) {
        lastErrMsg = "ID Employee invalide.";
        return false;
    }

    // ------ Contrôle de saisie ------
    if (!validerCIN(CIN)) {
        lastErrMsg = "CIN invalide (8 chiffres attendus).";
        return false;
    }
    if (!validerMail(mail)) {
        lastErrMsg = "Adresse mail invalide.";
        return false;
    }
    QString passwordToStore;
    if (password.trimmed().isEmpty()) {
        QSqlQuery currentPwdQuery;
        currentPwdQuery.prepare("SELECT PASSWORD FROM EMPLOYEE WHERE IDEMPLOYEE = :id");
        currentPwdQuery.bindValue(":id", idEmployee);
        if (!currentPwdQuery.exec() || !currentPwdQuery.next()) {
            lastErrMsg = "Impossible de récupérer le mot de passe actuel.";
            return false;
        }
        passwordToStore = currentPwdQuery.value(0).toString();
        if (passwordToStore.trimmed().isEmpty()) {
            lastErrMsg = "Mot de passe actuel introuvable.";
            return false;
        }
    } else {
        if (!validerPassword(password)) {
            lastErrMsg = "Mot de passe invalide (4-255 caractères).";
            return false;
        }
        passwordToStore = hashPasswordForStorage(password);
    }
    if (!validerNom(nom)) {
        lastErrMsg = "Nom invalide.";
        return false;
    }
    if (!validerPrenom(prenom)) {
        lastErrMsg = "Prénom invalide.";
        return false;
    }
    if (!validerRole(normalizedRole)) {
        lastErrMsg = "Rôle invalide.";
        return false;
    }
    if (!normalizedContract.isEmpty() && !allowedContracts.contains(normalizedContract)) {
        lastErrMsg = "Type de contrat invalide.";
        return false;
    }
    if (salaire_base < 0.0) {
        lastErrMsg = "Salaire de base invalide.";
        return false;
    }
    if (telephone.trimmed().length() > 20) {
        lastErrMsg = "Téléphone invalide (20 caractères max).";
        return false;
    }
    if (!validerDateNaissance(date_naissance)) {
        lastErrMsg = "Date de naissance invalide (�ge entre 18 et 100 ans).";
        return false;
    }
    if (!isPhotoPathValid(effectivePhotoPath)) {
        lastErrMsg = "Photo invalide: le fichier sélectionné n'existe pas.";
        return false;
    }

    // ------ Check for CIN uniqueness (excluding current employee) ------
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM EMPLOYEE WHERE CIN = :cin AND IDEMPLOYEE != :id");
    checkQuery.bindValue(":cin", CIN);
    checkQuery.bindValue(":id", idEmployee);
    if (!checkQuery.exec() || !checkQuery.next()) {
        lastErrMsg = "Erreur lors de la vérification du CIN.";
        return false;
    }
    if (checkQuery.value(0).toInt() > 0) {
        lastErrMsg = "Ce CIN existe déjà dans la base de données.";
        return false;
    }

    // ------ Check for MAIL uniqueness (excluding current employee) ------
    checkQuery.prepare("SELECT COUNT(*) FROM EMPLOYEE WHERE MAIL = :mail AND IDEMPLOYEE != :id");
    checkQuery.bindValue(":mail", mail);
    checkQuery.bindValue(":id", idEmployee);
    if (!checkQuery.exec() || !checkQuery.next()) {
        lastErrMsg = "Erreur lors de la vérification de l'email.";
        return false;
    }
    if (checkQuery.value(0).toInt() > 0) {
        lastErrMsg = "Cette adresse email existe déjà dans la base de données.";
        return false;
    }

    // ------ Requête SQL ------
    QSqlQuery currentFaceQuery;
    currentFaceQuery.prepare("SELECT NVL(FACE_ID, '') FROM EMPLOYEE WHERE IDEMPLOYEE = :id");
    currentFaceQuery.bindValue(":id", idEmployee);
    if (!currentFaceQuery.exec() || !currentFaceQuery.next()) {
        lastErrMsg = "Impossible de récupérer le face_id actuel.";
        return false;
    }
    const QString previousFaceId = currentFaceQuery.value(0).toString().trimmed();
    const bool faceIdChanged = previousFaceId != normalizedFaceId;

    QSqlQuery query;
    query.prepare(
        "UPDATE EMPLOYEE SET "
        "CIN = :CIN, "
        "MAIL = :mail, "
        "PASSWORD = :password, "
        "NOM = :nom, "
        "PRENOM = :prenom, "
        "TELEPHONE = :telephone, "
        "PHOTO_PATH = :photo_path, "
        "DATE_NAISSANCE = :date_naissance, "
        "DATE_EMBAUCHE = NVL(:date_embauche, DATE_EMBAUCHE), "
        "TYPE_CONTRAT = :type_contrat, "
        "SALAIRE_BASE = :salaire_base, "
        "FACE_ID = :face_id, "
        "FACE_TS = NVL(:face_ts, FACE_TS), "
        "ROLE = :role "
        "WHERE IDEMPLOYEE = :idEmployee"
    );

    query.bindValue(":CIN",            CIN);
    query.bindValue(":mail",           mail);
    query.bindValue(":password",       passwordToStore);
    query.bindValue(":nom",            nom);
    query.bindValue(":prenom",         prenom);
    query.bindValue(":telephone",      telephone.trimmed());
    query.bindValue(":photo_path",     effectivePhotoPath);
    query.bindValue(":date_naissance", date_naissance);
    query.bindValue(":date_embauche",  effectiveHireDate.isValid()
                                            ? QVariant(effectiveHireDate)
                                            : QVariant(QMetaType::fromType<QDate>()));
    query.bindValue(":type_contrat",   normalizedContract);
    query.bindValue(":salaire_base",   salaire_base);
    query.bindValue(":face_id",        normalizedFaceId);
    query.bindValue(":face_ts",        (faceIdChanged && !normalizedFaceId.isEmpty())
                                            ? QVariant(QDateTime::currentDateTime())
                                            : QVariant(QMetaType::fromType<QDateTime>()));
    query.bindValue(":role",           normalizedRole);
    query.bindValue(":idEmployee",     idEmployee);

    if (!query.exec()) {
        lastErrMsg = query.lastError().text();
        qDebug() << "Erreur Modifier Employee:" << lastErrMsg;
        return false;
    }

    if (query.numRowsAffected() == 0) {
        lastErrMsg = "Aucun employee trouvé avec cet ID.";
        return false;
    }

    recomputeClassification2D(idEmployee);
    logEmployeeHistory(idEmployee,
                       "UPDATE",
                       QString("Modification employé %1 %2, rôle %3").arg(prenom, nom, role.toUpper()));

    qDebug() << "Employee modifié avec succès (id:" << idEmployee << ")";
    return true;
}

// ============================================================
//  KNN — Chargement des profils employes
// ============================================================

namespace {
double clampScore(double value)
{
    return std::clamp(value, 0.0, 100.0);
}

double clampUnit(double value)
{
    return std::clamp(value, 0.0, 1.0);
}

double deterministicAxisJitter(int idEmployee,
                               int idFabrication,
                               const QString &niveau,
                               const QString &contrat,
                               int axisSeed)
{
    const QByteArray seed = QStringLiteral("%1|%2|%3|%4|%5")
                                .arg(idEmployee)
                                .arg(idFabrication)
                                .arg(niveau.trimmed().toUpper())
                                .arg(contrat.trimmed().toUpper())
                                .arg(axisSeed)
                                .toUtf8();
    const QByteArray digest = QCryptographicHash::hash(seed, QCryptographicHash::Sha256);
    const quint8 high = static_cast<quint8>(digest.at(0));
    const quint8 low = static_cast<quint8>(digest.at(1));
    const double normalized = (static_cast<double>((high << 8) | low) / 65535.0) - 0.5;
    return normalized * 0.04;
}

double niveauWeight(const QString &niveau)
{
    const QString n = niveau.trimmed().toUpper();
    if (n == "SENIOR") return 0.95;
    if (n == "MID") return 0.80;
    if (n == "JUNIOR") return 0.65;
    if (n == "APPRENTI") return 0.50;
    return 0.60;
}

double contratWeight(const QString &contrat)
{
    const QString c = contrat.trimmed().toUpper();
    if (c == "CDI") return 1.00;
    if (c == "CDD") return 0.92;
    if (c == "INTERIM") return 0.85;
    if (c == "STAGE") return 0.75;
    return 0.85;
}

double scoreCompetenceFromProfile(double classificationX, double classificationY)
{
    return clampScore((0.55 * clampUnit(classificationX) + 0.45 * clampUnit(classificationY)) * 100.0);
}

double scoreProximityFromDistance(double distance)
{
    const double normalizedDistance = std::clamp(distance, 0.0, 1.5);
    return clampScore((1.0 - (normalizedDistance / 1.5)) * 100.0);
}

QString qualityClassFromScore(double score)
{
    const double s = clampScore(score);
    if (s >= 85.0) return "A";
    if (s >= 70.0) return "B";
    if (s >= 55.0) return "C";
    return "D";
}

QString qualityClassFromRank(int rankIndex, int totalCount)
{
    if (totalCount <= 1) {
        return QStringLiteral("A");
    }

    const double normalizedRank = static_cast<double>(rankIndex) / static_cast<double>(totalCount - 1);
    if (normalizedRank < 0.20) return QStringLiteral("A");
    if (normalizedRank < 0.45) return QStringLiteral("B");
    if (normalizedRank < 0.75) return QStringLiteral("C");
    return QStringLiteral("D");
}

bool isArchivedStatus(const QString &status)
{
    return status.trimmed().toUpper() == "ARCHIVE";
}

bool applyFeedbackDelta(int idEmployee, double taskX, double taskY, bool confirme, bool writeAudit)
{
    if (idEmployee <= 0) {
        return false;
    }

    QSqlQuery readQuery;
    readQuery.prepare(
        "SELECT NVL(UPPER(STATUT), 'ACTIF'), "
        "NVL(CLASSIFICATION_X, :taskX), NVL(CLASSIFICATION_Y, :taskY) "
        "FROM EMPLOYEE "
        "WHERE IDEMPLOYEE = :idEmployee");
    readQuery.bindValue(":taskX", clampUnit(taskX));
    readQuery.bindValue(":taskY", clampUnit(taskY));
    readQuery.bindValue(":idEmployee", idEmployee);

    if (!readQuery.exec() || !readQuery.next()) {
        return false;
    }

    if (isArchivedStatus(readQuery.value(0).toString())) {
        return false;
    }

    double currentX = readQuery.value(1).isNull() ? taskX : clampUnit(readQuery.value(1).toDouble());
    double currentY = readQuery.value(2).isNull() ? taskY : clampUnit(readQuery.value(2).toDouble());

    const double alphaConfirm = 0.20;
    const double alphaReject = 0.08;

    double updatedX = currentX;
    double updatedY = currentY;

    if (confirme) {
        updatedX = ((1.0 - alphaConfirm) * currentX) + (alphaConfirm * taskX);
        updatedY = ((1.0 - alphaConfirm) * currentY) + (alphaConfirm * taskY);
    } else {
        updatedX = currentX + (alphaReject * (currentX - taskX));
        updatedY = currentY + (alphaReject * (currentY - taskY));
    }

    updatedX = clampUnit(updatedX);
    updatedY = clampUnit(updatedY);

    QSqlQuery updateQuery;
    updateQuery.prepare(
        "UPDATE EMPLOYEE SET CLASSIFICATION_X = :x, CLASSIFICATION_Y = :y WHERE IDEMPLOYEE = :id");
    updateQuery.bindValue(":x", updatedX);
    updateQuery.bindValue(":y", updatedY);
    updateQuery.bindValue(":id", idEmployee);
    if (!updateQuery.exec()) {
        return false;
    }

    if (writeAudit) {
        logEmployeeHistory(
            idEmployee,
            confirme ? "KNN_ACCEPT" : "KNN_REJECT",
            QString("Feedback KNN: task=(%1,%2) géré via EMPLOYEE_SKILL").arg(taskX).arg(taskY));
    }

    return true;
}

bool calculerClassificationDepuisFabrication(int idFabrication, double &x, double &y)
{
    if (idFabrication <= 0) {
        return false;
    }

    QSqlQuery fabQuery;
    fabQuery.prepare(
        "SELECT DATE_FIN_PREVUE, DATE_FIN_REELLE, NVL(UPPER(ETAT_FABRICATION), 'INCONNU') "
        "FROM FABRICATION "
        "WHERE IDFABRICATION = :idFabrication");
    fabQuery.bindValue(":idFabrication", idFabrication);

    if (!fabQuery.exec() || !fabQuery.next()) {
        return false;
    }

    const QDate plannedDate = fabQuery.value(0).toDate();
    const QDate actualDate = fabQuery.value(1).toDate();
    const QString fabricationEtat = fabQuery.value(2).toString();

    double scoreDelai = 0.50;
    if (plannedDate.isValid() && actualDate.isValid()) {
        const int delayDays = plannedDate.daysTo(actualDate);
        scoreDelai = clampUnit(1.0 - (delayDays / 30.0));
    } else if (plannedDate.isValid()) {
        const int delayDaysSoFar = plannedDate.daysTo(QDate::currentDate());
        scoreDelai = clampUnit(1.0 - (std::max(0, delayDaysSoFar) / 30.0));
    }

    QSqlQuery phaseQuery;
    phaseQuery.prepare(
        "SELECT COUNT(*) AS CNT, "
        "NVL(SUM(CASE WHEN UPPER(ETAT) = 'TERMINE_BON' THEN 1 ELSE 0 END), 0) AS CNT_BON, "
        "NVL(SUM(CASE WHEN UPPER(ETAT) = 'TERMINE_ACCEPTABLE' THEN 1 ELSE 0 END), 0) AS CNT_OK, "
        "NVL(SUM(CASE WHEN UPPER(ETAT) = 'TERMINE_MAUVAIS' THEN 1 ELSE 0 END), 0) AS CNT_BAD "
        "FROM PHASE "
        "WHERE IDFABRICATION = :idFabrication");
    phaseQuery.bindValue(":idFabrication", idFabrication);

    if (!phaseQuery.exec() || !phaseQuery.next()) {
        return false;
    }

    const int totalPhases = phaseQuery.value(0).toInt();
    const int doneBon = phaseQuery.value(1).toInt();
    const int doneOk = phaseQuery.value(2).toInt();
    const int doneBad = phaseQuery.value(3).toInt();

    double progressionScore = 0.50;
    if (totalPhases > 0) {
        const double quality = (doneBon + (0.6 * doneOk)) / static_cast<double>(totalPhases);
        const double anomalies = doneBad / static_cast<double>(totalPhases);
        progressionScore = clampUnit(quality - (0.35 * anomalies));
    }

    if (fabricationEtat.contains("RETARD")) {
        progressionScore = clampUnit(progressionScore - 0.20);
    } else if (fabricationEtat.contains("TERMINE")) {
        progressionScore = std::max(progressionScore, 0.90);
    } else if (fabricationEtat.contains("COURS")) {
        progressionScore = std::max(progressionScore, 0.60);
    } else if (fabricationEtat.contains("ANNULE")) {
        progressionScore = std::min(progressionScore, 0.20);
    } else if (fabricationEtat.contains("SUSPENDU")) {
        progressionScore = std::min(progressionScore, 0.35);
    }

    x = scoreDelai;
    y = progressionScore;
    return true;
}
}

bool Employee::recomputeClassification2D(int idEmployee)
{
    if (idEmployee <= 0) {
        return false;
    }

    QSqlQuery empQuery;
    empQuery.prepare(
        "SELECT NVL(UPPER(STATUT), 'ACTIF'), NVL(NIVEAU, 'JUNIOR'), NVL(TYPE_CONTRAT, 'CDD'), IDFABRICATION "
        "FROM EMPLOYEE WHERE IDEMPLOYEE = :id");
    empQuery.bindValue(":id", idEmployee);
    if (!empQuery.exec() || !empQuery.next()) {
        return false;
    }

    if (isArchivedStatus(empQuery.value(0).toString())) {
        return false;
    }

    const QString niveau = empQuery.value(1).toString();
    const QString contrat = empQuery.value(2).toString();
    const int idFab = empQuery.value(3).isNull() ? 0 : empQuery.value(3).toInt();

    QSqlQuery skillQuery;
    skillQuery.prepare(
        "SELECT "
        "NVL(AVG(CASE UPPER(NIVEAU_MAITRISE) "
        "         WHEN 'CERTIFIE' THEN 1.0 "
        "         WHEN 'EN_FORMATION' THEN 0.6 "
        "         ELSE 0.2 END), 0), "
        "NVL(AVG(CASE WHEN UPPER(NIVEAU_MAITRISE) = 'CERTIFIE' THEN 1 ELSE 0 END), 0) "
        "FROM EMPLOYEE_SKILL WHERE IDEMPLOYEE = :id");
    skillQuery.bindValue(":id", idEmployee);

    double avgMastery = 0.0;
    double certRatio = 0.0;
    if (skillQuery.exec() && skillQuery.next()) {
        avgMastery = skillQuery.value(0).toDouble();
        certRatio = skillQuery.value(1).toDouble();
    }

    QSqlQuery trainingQuery;
    trainingQuery.prepare(
        "SELECT NVL(AVG(CASE WHEN UPPER(NIVEAU_MAITRISE) = 'EN_FORMATION' THEN 1 ELSE 0 END), 0) "
        "FROM EMPLOYEE_SKILL WHERE IDEMPLOYEE = :id");
    trainingQuery.bindValue(":id", idEmployee);

    double trainRatio = 0.0;
    if (trainingQuery.exec() && trainingQuery.next()) {
        trainRatio = trainingQuery.value(0).toDouble();
    }

    double classificationX = clampUnit(
        (0.55 * avgMastery)
        + (0.20 * certRatio)
        + (0.15 * niveauWeight(niveau))
        + (0.10 * contratWeight(contrat)));

    double classificationY = clampUnit((0.65 * avgMastery) + (0.25 * certRatio) + (0.10 * trainRatio));

    double scheduleX = 0.50;
    double scheduleY = 0.50;
    if (idFab > 0) {
        calculerClassificationDepuisFabrication(idFab, scheduleX, scheduleY);
        classificationX = clampUnit((0.75 * classificationX) + (0.25 * scheduleX));
        classificationY = clampUnit((0.40 * classificationY) + (0.60 * scheduleY));
    }

    QSqlQuery updateQuery;
    updateQuery.prepare(
        "UPDATE EMPLOYEE SET CLASSIFICATION_X = :x, CLASSIFICATION_Y = :y WHERE IDEMPLOYEE = :id");
    updateQuery.bindValue(":x", classificationX);
        updateQuery.bindValue(":y", classificationY);
    updateQuery.bindValue(":id", idEmployee);
    return updateQuery.exec();
}

QVector<EmployeeKnnProfile> Employee::chargerProfilsKnn(const QString &roleFilter)
{
    QVector<EmployeeKnnProfile> employees;

    QSqlQuery query;
    QString sql =
        "SELECT E.IDEMPLOYEE, E.NOM, E.PRENOM, E.ROLE, E.IDFABRICATION, "
        "       E.CLASSIFICATION_X, E.CLASSIFICATION_Y, "
        "       NVL(E.NIVEAU, 'JUNIOR'), NVL(E.TYPE_CONTRAT, 'CDD'), "
        "       NVL(S.SKILL_AVG, 0), NVL(S.CERT_RATIO, 0), NVL(S.TRAIN_RATIO, 0) "
        "FROM EMPLOYEE E "
        "LEFT JOIN ("
        "    SELECT IDEMPLOYEE, "
        "           AVG(CASE UPPER(NIVEAU_MAITRISE) "
        "                   WHEN 'CERTIFIE' THEN 1.0 "
        "                   WHEN 'EN_FORMATION' THEN 0.6 "
        "                   ELSE 0.2 END) AS SKILL_AVG, "
        "           AVG(CASE WHEN UPPER(NIVEAU_MAITRISE) = 'CERTIFIE' THEN 1 ELSE 0 END) AS CERT_RATIO, "
        "           AVG(CASE WHEN UPPER(NIVEAU_MAITRISE) = 'EN_FORMATION' THEN 1 ELSE 0 END) AS TRAIN_RATIO "
        "    FROM EMPLOYEE_SKILL "
        "    GROUP BY IDEMPLOYEE"
        ") S ON S.IDEMPLOYEE = E.IDEMPLOYEE "
        "WHERE NVL(UPPER(E.STATUT), 'ACTIF') <> 'ARCHIVE'";

    const QString role = canonicalRoleValue(roleFilter);
    if (!role.isEmpty() && role != "TOUS") {
        sql += " AND UPPER(ROLE) = :role";
    }

    sql += " ORDER BY IDEMPLOYEE";

    query.prepare(sql);
    if (!role.isEmpty() && role != "TOUS") {
        query.bindValue(":role", role);
    }

    if (!query.exec()) {
        qDebug() << "Erreur chargerProfilsKnn:" << query.lastError().text();
        return employees;
    }

    while (query.next()) {
        EmployeeKnnProfile employee;
        employee.idEmployee = query.value(0).toInt();
        employee.nom = query.value(1).toString();
        employee.prenom = query.value(2).toString();
        employee.role = query.value(3).toString();

        employee.idFabrication = query.value(4).isNull() ? 0 : query.value(4).toInt();

        const bool hasStoredX = !query.value(5).isNull();
        const bool hasStoredY = !query.value(6).isNull();
        const double storedX = hasStoredX ? clampUnit(query.value(5).toDouble()) : 0.50;
        const double storedY = hasStoredY ? clampUnit(query.value(6).toDouble()) : 0.50;

        const QString niveau = query.value(7).toString();
        const QString contrat = query.value(8).toString();
        const double avgMastery = clampUnit(query.value(9).toDouble());
        const double certRatio = clampUnit(query.value(10).toDouble());
        const double trainRatio = clampUnit(query.value(11).toDouble());

        double classificationX = 0.0;
        double classificationY = 0.0;
        const bool hasAutoClassification = calculerClassificationDepuisFabrication(employee.idFabrication,
                                                                                    classificationX,
                                                                                    classificationY);

        if (!hasStoredX) {
            classificationX = clampUnit(
                (0.55 * avgMastery)
                + (0.20 * certRatio)
                + (0.15 * niveauWeight(niveau))
                + (0.10 * contratWeight(contrat)));
        } else {
            classificationX = storedX;
        }

        if (hasAutoClassification) {
            classificationX = clampUnit((0.40 * storedX) + (0.60 * classificationX));
            classificationY = clampUnit((0.40 * storedY) + (0.60 * classificationY));
        } else {
            if (!hasStoredY) {
                classificationY = clampUnit((0.65 * avgMastery) + (0.25 * certRatio) + (0.10 * trainRatio));
            } else {
                classificationY = storedY;
            }
        }

        classificationX = clampUnit(classificationX + deterministicAxisJitter(employee.idEmployee,
                                                                              employee.idFabrication,
                                                                              niveau,
                                                                              contrat,
                                                                              0));
        classificationY = clampUnit(classificationY + deterministicAxisJitter(employee.idEmployee,
                                                                              employee.idFabrication,
                                                                              niveau,
                                                                              contrat,
                                                                              1));

        employee.classificationX = classificationX;
        employee.classificationY = classificationY;

        employees.append(employee);
    }

    return employees;
}

double Employee::calculerDistanceEuclidienne(double x1, double y1, double x2, double y2)
{
    const double dx = x1 - x2;
    const double dy = y1 - y2;
    const double weightX = 0.45;
    const double weightY = 0.55;
    return std::sqrt((weightX * dx * dx) + (weightY * dy * dy));
}

bool Employee::pointTacheDepuisFabrication(int idFabrication, double &taskX, double &taskY)
{
    return calculerClassificationDepuisFabrication(idFabrication, taskX, taskY);
}

bool Employee::pointTacheDepuisPhase(int idPhase, double &taskX, double &taskY)
{
    if (idPhase <= 0) {
        return false;
    }

    QSqlQuery query;
    query.prepare(
        "SELECT CASE UPPER(P.ETAT) "
        "         WHEN 'TERMINE_BON' THEN 2 "
        "         WHEN 'TERMINE_ACCEPTABLE' THEN 1 "
        "         WHEN 'EN_COURS' THEN 1 "
        "         ELSE 0 END, "
        "       P.IDFABRICATION "
        "FROM PHASE P "
        "WHERE P.ID_PHASE = :idPhase");
    query.bindValue(":idPhase", idPhase);

    if (!query.exec() || !query.next()) {
        return false;
    }

    const int etat = query.value(0).toInt();
    const int idFabrication = query.value(1).toInt();

    if (!calculerClassificationDepuisFabrication(idFabrication, taskX, taskY)) {
        return false;
    }

    double phaseEtatScore = 0.30;
    switch (etat) {
    case 2:
        phaseEtatScore = 1.0;
        break;
    case 1:
        phaseEtatScore = 0.60;
        break;
    default:
        phaseEtatScore = 0.30;
        break;
    }
    taskY = clampUnit((0.7 * taskY) + (0.3 * phaseEtatScore));

    return true;
}

QVector<EmployeeKnnRecommendation> Employee::recommanderEmployesPourTache(double taskX,
                                                                           double taskY,
                                                                           int k,
                                                                           const QString &roleFilter)
{
    QVector<EmployeeKnnRecommendation> rankedEmployees;

    if (k <= 0) {
        return rankedEmployees;
    }

    const QVector<EmployeeKnnProfile> employees = chargerProfilsKnn(roleFilter);
    rankedEmployees.reserve(employees.size());

    for (const EmployeeKnnProfile &employee : employees) {
        EmployeeKnnRecommendation recommendation;
        recommendation.employee = employee;
        recommendation.distance = calculerDistanceEuclidienne(employee.classificationX,
                                                              employee.classificationY,
                                                              taskX,
                                                              taskY);

        const double distanceScore = scoreProximityFromDistance(recommendation.distance);
        const double profileScore = scoreCompetenceFromProfile(employee.classificationX,
                                                               employee.classificationY);
        recommendation.competenceScore = clampScore((0.82 * distanceScore) + (0.18 * profileScore));
        recommendation.qualityClass = qualityClassFromScore(recommendation.competenceScore);

        rankedEmployees.append(recommendation);
    }

    std::sort(rankedEmployees.begin(), rankedEmployees.end(),
              [](const EmployeeKnnRecommendation &a, const EmployeeKnnRecommendation &b) {
                  if (a.distance != b.distance) {
                      return a.distance < b.distance;
                  }
                  return a.competenceScore > b.competenceScore;
              });

    if (k < rankedEmployees.size()) {
        rankedEmployees.resize(k);
    }

    for (int i = 0; i < rankedEmployees.size(); ++i) {
        rankedEmployees[i].qualityClass = qualityClassFromRank(i, rankedEmployees.size());
    }

    return rankedEmployees;
}

QVector<double> Employee::calculerProbabilitesKnn(const QVector<EmployeeKnnRecommendation> &recommendations)
{
    QVector<double> probabilities;
    probabilities.reserve(recommendations.size());

    if (recommendations.isEmpty()) {
        return probabilities;
    }

    double totalDistance = 0.0;
    for (const auto &rec : recommendations) {
        totalDistance += std::max(0.0, rec.distance);
    }

    const double averageDistance = totalDistance / static_cast<double>(recommendations.size());
    const double temperature = std::max(0.08, averageDistance * 0.65 + 0.05);
    QVector<double> weights;
    weights.reserve(recommendations.size());

    double sumWeights = 0.0;
    for (const auto &rec : recommendations) {
        const double proximityWeight = std::exp(-(std::max(0.0, rec.distance) / temperature));
        const double competenceWeight = 0.55 + (0.45 * (clampScore(rec.competenceScore) / 100.0));
        const double w = proximityWeight * competenceWeight;
        weights.append(w);
        sumWeights += w;
    }

    if (sumWeights <= 0.0) {
        return probabilities;
    }

    for (double w : weights) {
        const double p = (w / sumWeights) * 100.0;
        probabilities.append(clampScore(p));
    }

    return probabilities;
}

bool Employee::appliquerFeedbackKnn(int idEmployee, double taskX, double taskY, bool confirme)
{
    return applyFeedbackDelta(idEmployee, taskX, taskY, confirme, true);
}

bool Employee::entrainerModeleKnn(int &updatedCountOut, QString &errorOut)
{
    updatedCountOut = 0;
    errorOut.clear();

    QSqlQuery employeeQuery;
    employeeQuery.prepare("SELECT IDEMPLOYEE FROM EMPLOYEE WHERE NVL(UPPER(STATUT), 'ACTIF') <> 'ARCHIVE' ORDER BY IDEMPLOYEE");
    if (!employeeQuery.exec()) {
        errorOut = employeeQuery.lastError().text();
        return false;
    }

    int baseRecomputed = 0;
    while (employeeQuery.next()) {
        const int idEmployee = employeeQuery.value(0).toInt();
        if (idEmployee > 0 && recomputeClassification2D(idEmployee)) {
            ++baseRecomputed;
        }
    }

    int feedbackApplied = 0;
    QSqlQuery feedbackQuery;
    feedbackQuery.prepare(
        "SELECT RECORD_ID, ACTION_TYPE, DETAILS "
        "FROM AUDIT_LOG "
        "WHERE TABLE_NAME = 'EMPLOYEE' "
        "  AND ACTION_TYPE IN ('KNN_ACCEPT', 'KNN_REJECT') "
        "ORDER BY ACTION_AT, ID_LOG");

    if (feedbackQuery.exec()) {
        const QRegularExpression re(R"(task=\(([+-]?\d*\.?\d+),([+-]?\d*\.?\d+)\))");
        while (feedbackQuery.next()) {
            const int idEmployee = feedbackQuery.value(0).toInt();
            const QString action = feedbackQuery.value(1).toString().trimmed().toUpper();
            const QString details = feedbackQuery.value(2).toString();
            const QRegularExpressionMatch m = re.match(details);
            if (!m.hasMatch()) {
                continue;
            }

            bool okX = false;
            bool okY = false;
            const double taskX = clampUnit(m.captured(1).toDouble(&okX));
            const double taskY = clampUnit(m.captured(2).toDouble(&okY));
            if (!okX || !okY) {
                continue;
            }

            const bool confirmed = (action == "KNN_ACCEPT");
            if (applyFeedbackDelta(idEmployee, taskX, taskY, confirmed, false)) {
                ++feedbackApplied;
            }
        }
    }

    updatedCountOut = baseRecomputed + feedbackApplied;
    return true;
}

bool Employee::sauvegarderClasseEmploye(int idEmployee, const QString &classe, double score)
{
    if (idEmployee <= 0) {
        return false;
    }

    const QString normalizedClass = classe.trimmed().toUpper();
    if (normalizedClass != "A" && normalizedClass != "B" && normalizedClass != "C" && normalizedClass != "D") {
        return false;
    }

    QSqlQuery query;
    query.prepare(
        "UPDATE EMPLOYEE "
        "SET CLASS_LABEL = :classLabel, "
        "    CLASS_SCORE = CASE WHEN :scoreValue >= 0 THEN :scoreValue ELSE CLASS_SCORE END, "
        "    CLASS_UPDATED_AT = SYSTIMESTAMP "
        "WHERE IDEMPLOYEE = :idEmployee");
    query.bindValue(":classLabel", normalizedClass);
    query.bindValue(":scoreValue", score);
    query.bindValue(":idEmployee", idEmployee);
    if (!query.exec()) {
        qWarning() << "Impossible de sauvegarder CLASS_LABEL pour l'employé" << idEmployee
                   << ":" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

// ============================================================
//  Fabrication Assignment / Removal
// ============================================================

bool Employee::assignerAFabrication(int idEmployee, int idFabrication)
{
    if (idEmployee <= 0 || idFabrication <= 0) {
        return false;
    }

    // Verify employee exists
    QSqlQuery checkEmp;
    checkEmp.prepare("SELECT COUNT(*) FROM EMPLOYEE WHERE IDEMPLOYEE = :empId");
    checkEmp.bindValue(":empId", idEmployee);
    if (!checkEmp.exec() || !checkEmp.next() || checkEmp.value(0).toInt() == 0) {
        return false;  // Employee doesn't exist
    }

    // Verify fabrication exists
    QSqlQuery checkFab;
    checkFab.prepare("SELECT COUNT(*) FROM FABRICATION WHERE IDFABRICATION = :fabId");
    checkFab.bindValue(":fabId", idFabrication);
    if (!checkFab.exec() || !checkFab.next() || checkFab.value(0).toInt() == 0) {
        return false;  // Fabrication doesn't exist
    }

    QSqlQuery query;
    query.prepare("UPDATE EMPLOYEE SET IDFABRICATION = :fabId WHERE IDEMPLOYEE = :empId");
    query.bindValue(":fabId", idFabrication);
    query.bindValue(":empId", idEmployee);

    if (!query.exec()) {
        return false;
    }

    QSqlQuery verifyQuery;
    verifyQuery.prepare(
        "SELECT COUNT(*) "
        "FROM EMPLOYEE "
        "WHERE IDEMPLOYEE = :empId AND IDFABRICATION = :fabId");
    verifyQuery.bindValue(":empId", idEmployee);
    verifyQuery.bindValue(":fabId", idFabrication);
    if (!verifyQuery.exec() || !verifyQuery.next() || verifyQuery.value(0).toInt() != 1) {
        return false;
    }

    logEmployeeHistory(idEmployee,
                       "ASSIGN_FABRICATION",
                       QString("Affecté à la fabrication #%1").arg(idFabrication));
    recomputeClassification2D(idEmployee);
    return true;
}

bool Employee::desaffecter(int idEmployee)
{
    if (idEmployee <= 0) {
        return false;
    }

    // Verify employee exists
    QSqlQuery checkEmp;
    checkEmp.prepare("SELECT COUNT(*) FROM EMPLOYEE WHERE IDEMPLOYEE = :empId");
    checkEmp.bindValue(":empId", idEmployee);
    if (!checkEmp.exec() || !checkEmp.next() || checkEmp.value(0).toInt() == 0) {
        return false;  // Employee doesn't exist
    }

    QSqlQuery query;
    query.prepare("UPDATE EMPLOYEE SET IDFABRICATION = NULL WHERE IDEMPLOYEE = :empId");
    query.bindValue(":empId", idEmployee);

    if (!query.exec()) {
        return false;
    }

    QSqlQuery verifyQuery;
    verifyQuery.prepare(
        "SELECT COUNT(*) "
        "FROM EMPLOYEE "
        "WHERE IDEMPLOYEE = :empId AND IDFABRICATION IS NULL");
    verifyQuery.bindValue(":empId", idEmployee);
    if (!verifyQuery.exec() || !verifyQuery.next() || verifyQuery.value(0).toInt() != 1) {
        return false;
    }

    logEmployeeHistory(idEmployee,
                       "UNASSIGN_FABRICATION",
                       "Désaffecté de sa fabrication");
    recomputeClassification2D(idEmployee);
    return true;
}

bool Employee::getFaceIdByEmployeeId(int idEmployee, QString &faceIdOut)
{
    faceIdOut.clear();
    if (idEmployee <= 0) {
        return false;
    }

    QSqlQuery query;
    query.prepare("SELECT FACE_ID FROM EMPLOYEE WHERE IDEMPLOYEE = :id");
    query.bindValue(":id", idEmployee);
    if (!query.exec() || !query.next()) {
        return false;
    }

    faceIdOut = query.value(0).toString();
    return true;
}

bool Employee::proposerTopEmployePourFabrication(int idFabrication,
                                                 const QString &roleFilter,
                                                 EmployeeKnnRecommendation &outRecommendation)
{
    double taskX = 0.0;
    double taskY = 0.0;
    if (!pointTacheDepuisFabrication(idFabrication, taskX, taskY)) {
        return false;
    }

    const QVector<EmployeeKnnRecommendation> suggestions =
        recommanderEmployesPourTache(taskX, taskY, 1, roleFilter);
    if (suggestions.isEmpty()) {
        return false;
    }

    outRecommendation = suggestions.first();
    return true;
}

bool Employee::exporterCsv(const QString &filePath, int &rowCountOut, QString &errorOut)
{
    rowCountOut = 0;
    errorOut.clear();

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        errorOut = "Impossible d'écrire le fichier CSV.";
        return false;
    }

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);

    QSqlQuery query;
    query.prepare(
        "SELECT CIN, MAIL, NOM, PRENOM, TO_CHAR(DATE_NAISSANCE, 'DD/MM/YYYY'), ROLE, STATUT, TYPE_CONTRAT, TO_CHAR(DATE_EMBAUCHE, 'DD/MM/YYYY'), "
        "NVL(PHOTO_PATH, ''), "
        "(SELECT COUNT(*) FROM EMPLOYEE_SKILL ES WHERE ES.IDEMPLOYEE = EMPLOYEE.IDEMPLOYEE), "
        "(SELECT COUNT(*) FROM EMPLOYEE_SKILL ES WHERE ES.IDEMPLOYEE = EMPLOYEE.IDEMPLOYEE AND UPPER(ES.NIVEAU_MAITRISE) = 'CERTIFIE') "
        "FROM EMPLOYEE "
        "ORDER BY IDEMPLOYEE");

    if (!query.exec()) {
        errorOut = query.lastError().text();
        return false;
    }

    out << "CIN;MAIL;NOM;PRENOM;DATE_NAISSANCE;ROLE;STATUT;TYPE_CONTRAT;DATE_EMBAUCHE;PHOTO_PATH;NB_COMPETENCES;COMPETENCES_CERTIFIEES\n";

    while (query.next()) {
        QStringList row;
        for (int c = 0; c < 12; ++c) {
            QString value = query.value(c).toString();
            value.replace('"', "\"\"");
            if (value.contains(';') || value.contains('"') || value.contains('\n')) {
                value = QString("\"%1\"").arg(value);
            }
            row << value;
        }

        out << row.join(';') << "\n";
        ++rowCountOut;
    }

    file.close();
    return true;
}

bool Employee::importerCsv(const QString &filePath,
                           int &successCountOut,
                           int &failCountOut,
                           QString &errorOut)
{
    successCountOut = 0;
    failCountOut = 0;
    errorOut.clear();

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        errorOut = "Impossible de lire le fichier CSV.";
        return false;
    }

    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);

    bool isHeader = true;
    QChar detectedSeparator = ';';
    int physicalLineNumber = 0;
    bool firstContentLine = true;

    QSet<QString> seenCin;
    QSet<QString> seenMail;
    QStringList errorSamples;

    const int kMinRequiredColumns = 6;
    const int kMaxSupportedColumns = 12;
    const int kMaxLineLength = 8192;

    auto registerFailure = [&](int lineNo, const QString &reason) {
        ++failCountOut;
        if (errorSamples.size() < 8) {
            errorSamples << QString("Ligne %1: %2").arg(lineNo).arg(reason);
        }
    };

    while (!in.atEnd()) {
        ++physicalLineNumber;
        const QString rawLine = in.readLine();
        QString line = rawLine.trimmed();

        if (firstContentLine && line.startsWith(QChar(0xFEFF))) {
            line.remove(0, 1);
        }

        if (line.isEmpty()) {
            continue;
        }

        firstContentLine = false;

        if (line.size() > kMaxLineLength) {
            registerFailure(physicalLineNumber, "Ligne trop longue (limite 8192 caractères).");
            isHeader = false;
            continue;
        }

        if (line.contains(QChar('\0'))) {
            registerFailure(physicalLineNumber, "Caractère NULL détecté.");
            isHeader = false;
            continue;
        }

        if (isHeader) {
            detectedSeparator = detectCsvSeparator(line);
        }

        bool hasUnbalancedQuotes = false;
        const QStringList cols = parseCsvLineQuoted(line, detectedSeparator, &hasUnbalancedQuotes);

        if (hasUnbalancedQuotes) {
            registerFailure(physicalLineNumber, "Guillemets non fermés dans la ligne CSV.");
            isHeader = false;
            continue;
        }

        if (isHeader) {
            isHeader = false;
            if (!cols.isEmpty() && cols.first().contains("CIN", Qt::CaseInsensitive)) {
                continue;
            }
        }

        if (cols.size() < kMinRequiredColumns) {
            registerFailure(physicalLineNumber,
                            QString("Colonnes insuffisantes (%1 trouvées, %2 minimum).")
                                .arg(cols.size())
                                .arg(kMinRequiredColumns));
            continue;
        }

        if (cols.size() > kMaxSupportedColumns) {
            registerFailure(physicalLineNumber,
                            QString("Trop de colonnes (%1, max %2).")
                                .arg(cols.size())
                                .arg(kMaxSupportedColumns));
            continue;
        }

        const QString cin = cols.value(0).trimmed();
        const QString mail = cols.value(1).trimmed();
        const QString nom = cols.value(2).trimmed();
        const QString prenom = cols.value(3).trimmed();
        const QString dobRaw = cols.value(4).trimmed();
        const QString role = cols.value(5).trimmed();

        if (cin.isEmpty() || mail.isEmpty() || nom.isEmpty() || prenom.isEmpty() || dobRaw.isEmpty() || role.isEmpty()) {
            registerFailure(physicalLineNumber, "Champs obligatoires manquants (CIN, MAIL, NOM, PRENOM, DATE_NAISSANCE, ROLE).");
            continue;
        }

        const QString mailKey = mail.toLower();
        if (seenCin.contains(cin)) {
            registerFailure(physicalLineNumber, QString("CIN dupliqué dans le fichier (%1).") .arg(cin));
            continue;
        }
        if (seenMail.contains(mailKey)) {
            registerFailure(physicalLineNumber, QString("Email dupliqué dans le fichier (%1).") .arg(mail));
            continue;
        }

        const QDate dob = parseCsvDateFlexible(dobRaw);
        if (!dob.isValid()) {
            registerFailure(physicalLineNumber,
                            QString("Date de naissance invalide (%1). Formats acceptés: dd/MM/yyyy, yyyy-MM-dd, dd-MM-yyyy.")
                                .arg(dobRaw));
            continue;
        }

        Employee e;
        e.setCIN(cin);
        e.setMail(mail);
        e.setPassword(generateTemporaryImportPassword());
        e.setNom(nom);
        e.setPrenom(prenom);
        e.setDateNaissance(dob);
        e.setRole(role);

        if (cols.size() > 7) {
            e.setTypeContrat(cols.value(7).trimmed());
        }
        if (cols.size() > 8) {
            const QString hireDateRaw = cols.value(8).trimmed();
            if (!hireDateRaw.isEmpty()) {
                const QDate hireDate = parseCsvDateFlexible(hireDateRaw);
                if (!hireDate.isValid()) {
                    registerFailure(physicalLineNumber,
                                    QString("Date d'embauche invalide (%1).")
                                        .arg(hireDateRaw));
                    continue;
                }
                e.setDateEmbauche(hireDate);
            }
        }
        if (cols.size() > 9) {
            const QString importedImageValue = cols.value(9).trimmed();
            e.setPhotoPath(importedImageValue);

            // Backward compatibility for legacy CSVs where column 10 was FACE_ID.
            if (!(importedImageValue.startsWith(":/")
                  || importedImageValue.contains('/')
                  || importedImageValue.contains('\\'))) {
                e.setFaceId(importedImageValue);
            }
        }

        if (e.ajouter()) {
            seenCin.insert(cin);
            seenMail.insert(mailKey);
            ++successCountOut;
        } else {
            registerFailure(physicalLineNumber,
                            QString("Insertion refusée: %1")
                                .arg(e.getLastError().isEmpty() ? "données invalides ou doublon en base" : e.getLastError()));
        }
    }

    file.close();

    if (!errorSamples.isEmpty()) {
        errorOut = QString("%1 ligne(s) importée(s), %2 échec(s). Détails: %3")
                       .arg(successCountOut)
                       .arg(failCountOut)
                       .arg(errorSamples.join(" | "));
    }

    return true;
}
