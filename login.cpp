#include "login.h"
#include "ui_login.h"
#include "smartcarpentry.h"
#include "faceid.h"

#include <QRandomGenerator>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>
#include <QDialog>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QFileDialog>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QImageReader>
#include <QProcessEnvironment>

namespace {
bool isStoredPasswordHash(const QString &value)
{
    return value.startsWith("sha256$");
}

bool isLegacySha256Hex(const QString &value)
{
    return value.size() == 64 && value.contains(QRegularExpression(QStringLiteral("^[0-9a-fA-F]{64}$")));
}

bool verifyPasswordAgainstStored(const QString &plainPassword, const QString &storedPassword)
{
    if (isStoredPasswordHash(storedPassword)) {
        const QStringList parts = storedPassword.split('$');
        if (parts.size() != 3 || parts.at(0) != "sha256") {
            return false;
        }

        const QByteArray salt = QByteArray::fromHex(parts.at(1).toLatin1());
        const QByteArray expected = parts.at(2).toLatin1();
        if (salt.isEmpty() || expected.isEmpty()) {
            return false;
        }

        const QByteArray computed = QCryptographicHash::hash(salt + plainPassword.toUtf8(),
                                                             QCryptographicHash::Sha256).toHex();
        return computed == expected;
    }

    if (isLegacySha256Hex(storedPassword)) {
        if (storedPassword.trimmed() == plainPassword.trimmed()) {
            return true;
        }
        const QByteArray computed = QCryptographicHash::hash(plainPassword.toUtf8(),
                                                             QCryptographicHash::Sha256).toHex();
        return QString::fromLatin1(computed) == storedPassword.trimmed();
    }

    // Backward compatibility for legacy plaintext passwords.
    return storedPassword.trimmed() == plainPassword.trimmed();
}

bool isLikelyFaceToken(const QString &value)
{
    const QString token = value.trimmed();
    if (token.isEmpty()) {
        return false;
    }
    if (token.startsWith(":/") || token.contains('/') || token.contains('\\')) {
        return false;
    }
    return token.size() <= 100;
}

bool isFaceIdTemporarilyBypassed()
{
    const QString value = QString::fromLatin1(qgetenv("SC_SKIP_FACE_ID")).trimmed().toLower();
    return value == "1" || value == "true" || value == "yes" || value == "on";
}

QString resolveFaceppApiKey()
{
    return QProcessEnvironment::systemEnvironment().value(QStringLiteral("FACEPP_API_KEY")).trimmed();
}

QString resolveFaceppApiSecret()
{
    return QProcessEnvironment::systemEnvironment().value(QStringLiteral("FACEPP_API_SECRET")).trimmed();
}

QString resolveProjectRootPathForLogin()
{
    const QStringList startPaths = {
        QDir::currentPath(),
        QCoreApplication::applicationDirPath()
    };

    for (const QString &startPath : startPaths) {
        QDir dir(startPath);
        for (int depth = 0; depth < 12; ++depth) {
            if (dir.exists("Smartcarpentry.pro") || dir.exists("CMakeLists.txt")) {
                return QDir::cleanPath(dir.absolutePath());
            }
            if (!dir.cdUp()) {
                break;
            }
        }
    }

    return QDir::cleanPath(QCoreApplication::applicationDirPath());
}

QString persistProfilePhotoUploadForLogin(const QString &sourcePath, QString *errorMessage = nullptr)
{
    if (errorMessage) {
        errorMessage->clear();
    }

    const QString trimmedSource = sourcePath.trimmed();
    if (trimmedSource.isEmpty()) {
        if (errorMessage) *errorMessage = "Aucune image sélectionnée.";
        return QString();
    }

    QFileInfo sourceInfo(trimmedSource);
    if (!sourceInfo.exists() || !sourceInfo.isFile()) {
        if (errorMessage) *errorMessage = "Image source introuvable.";
        return QString();
    }

    QImageReader reader(trimmedSource);
    if (!reader.canRead()) {
        if (errorMessage) *errorMessage = "Fichier image invalide ou format non pris en charge.";
        return QString();
    }

    const QString projectRoot = resolveProjectRootPathForLogin();
    QDir photoDir(projectRoot + "/employee_photos");
    if (!photoDir.exists() && !photoDir.mkpath(".")) {
        if (errorMessage) *errorMessage = "Impossible de créer le dossier des photos.";
        return QString();
    }

    const QString extension = sourceInfo.suffix().isEmpty() ? QString("png") : sourceInfo.suffix().toLower();
    const QString fileName = QString("profile_%1_%2.%3")
            .arg(QDateTime::currentDateTimeUtc().toString("yyyyMMddHHmmsszzz"))
            .arg(QRandomGenerator::global()->bounded(1000, 9999))
            .arg(extension);
    const QString targetPath = photoDir.filePath(fileName);

    if (!QFile::copy(trimmedSource, targetPath)) {
        if (errorMessage) *errorMessage = "Échec de copie de la photo dans le stockage local.";
        return QString();
    }

    return QDir::cleanPath(targetPath);
}

double compareFaceTokens(const QString &storedToken, const QString &capturedToken)
{
    const QString apiKey = resolveFaceppApiKey();
    const QString apiSecret = resolveFaceppApiSecret();
    if (apiKey.isEmpty() || apiSecret.isEmpty()) {
        return -1.0;
    }

    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl(QStringLiteral("https://api-us.faceplusplus.com/facepp/v3/compare")));
    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      QStringLiteral("application/x-www-form-urlencoded"));

    QUrlQuery query;
    query.addQueryItem(QStringLiteral("api_key"), apiKey);
    query.addQueryItem(QStringLiteral("api_secret"), apiSecret);
    query.addQueryItem(QStringLiteral("face_token1"), storedToken.trimmed());
    query.addQueryItem(QStringLiteral("face_token2"), capturedToken.trimmed());

    QNetworkReply *reply = manager.post(request, query.query(QUrl::FullyEncoded).toUtf8());
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    const QByteArray payload = reply->readAll();
    const QJsonDocument doc = QJsonDocument::fromJson(payload);
    reply->deleteLater();

    if (!doc.isObject()) {
        return -1.0;
    }

    return doc.object().value(QStringLiteral("confidence")).toDouble(-1.0);
}
}

login::login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);

    // Toujours commencer par la page login
    ui->stackedWidgetlogin->setCurrentIndex(0);

    // Set window properties
    setWindowTitle("SmartCarpentry - Connexion");
    setMinimumSize(1024, 768);

    // Rendre le QLabel cliquable (si pas fait dans Designer)
    ui->label_m->setText("<a href=\"#\">Mot de passe oublié ?</a>");
    ui->label_m->setTextFormat(Qt::RichText);
    ui->label_m->setTextInteractionFlags(Qt::LinksAccessibleByMouse);
    ui->label_m->setOpenExternalLinks(false);
    
    // Set initial focus on email field
    ui->id1->setFocus();
    
    // Add some keyboard shortcuts for better UX
    ui->id1->setTabOrder(ui->id1, ui->mot1);
    ui->mot1->setTabOrder(ui->mot1, ui->pushButton_2);
    
    // Create default users for testing if they don't exist
    createDefaultUsers();
    
    // Debug: List all users in database
    debugDatabaseUsers();
}

login::~login()
{
    delete ui;
}

void login::on_pushButton_r_clicked()
{
    ui->stackedWidgetlogin->setCurrentIndex(0); // Return to main login page
}


// ==============================
// BOUTON SE CONNECTER
// ==============================
void login::on_pushButton_2_clicked()
{
    // Get email and password from the UI
    QString email = ui->id1->text().trimmed();
    QString password = ui->mot1->text();
    
    qDebug() << "Login attempt - Email:" << email << "Password length:" << password.length();
    
    // Validate input
    if (email.isEmpty()) {
        showLoginError("Veuillez saisir votre adresse e-mail.");
        return;
    }
    
    if (password.isEmpty()) {
        showLoginError("Veuillez saisir votre mot de passe.");
        return;
    }
    
    // Test database connection first
    ConnSQL& db = ConnSQL::createInstance();
    if (!db.getDB()->isOpen()) {
        showLoginError("Erreur: Base de données non connectée.");
        return;
    }
    
    qDebug() << "Database connection is open, attempting authentication...";
    
    // Authenticate user against database
    if (authenticateUser(email, password)) {
        if (false) {
            if (!isLikelyFaceToken(authenticatedFaceId)) {
                showLoginError("Aucun Face ID valide n'est enregistré pour ce compte.");
                ui->mot1->clear();
                ui->mot1->setFocus();
                return;
            }

            faceid faceDialog(this);
            const int faceResult = faceDialog.exec();
            if (faceResult != QDialog::Accepted) {
                showLoginError("Authentification faciale annulée ou échouée.");
                ui->mot1->clear();
                ui->mot1->setFocus();
                return;
            }

            const QString capturedFaceToken = faceDialog.getResult().trimmed();
            if (!isLikelyFaceToken(capturedFaceToken)) {
                showLoginError("Impossible de lire un Face ID valide depuis la caméra.");
                ui->mot1->clear();
                ui->mot1->setFocus();
                return;
            }

            const double confidence = compareFaceTokens(authenticatedFaceId, capturedFaceToken);
            constexpr double kMinConfidence = 70.0;
            if (confidence < kMinConfidence) {
                showLoginError("Visage non reconnu pour cet utilisateur.");
                ui->mot1->clear();
                ui->mot1->setFocus();
                return;
            }
        }

        QString displayName = (authenticatedPrenom + " " + authenticatedNom).trimmed();
        if (displayName.isEmpty()) {
            displayName = email;
        }

        // Open main application window
        smartcarpentry *w = new smartcarpentry(displayName, authenticatedProfileImagePath, authenticatedEmail);
        w->showMaximized();
        this->close();
    } else {
        // Authentication failed
        showLoginError("Adresse e-mail ou mot de passe incorrect.");
        ui->mot1->clear(); // Clear password field
        ui->id1->setFocus(); // Focus on email field

    }
}


// ==============================
// LABEL MOT DE PASSE OUBLIE
// ==============================
void login::on_label_m_linkActivated(const QString &link)
{
    Q_UNUSED(link);
    ui->stackedWidgetlogin->setCurrentIndex(2); // Go to password recovery page
}


// ==============================
// BOUTON RETOUR
// ==============================
void login::on_btnRetour_clicked()
{
    ui->stackedWidgetlogin->setCurrentIndex(0);
}

// ==============================
// CREATE ACCOUNT DIALOG (Modal Popup)
// ==============================

void login::on_pushButton_createAccount_clicked()
{
    bool accountCreated = false;
    QString createdAccountEmail;

    // Create modal dialog for account creation
    QDialog dialog(this);
    dialog.setWindowTitle("Créer un nouveau compte");
    dialog.setMinimumWidth(500);
    dialog.setStyleSheet(
        "QDialog { background-color: #0f172a; color: #f8fafc; }"
        "QLabel { color: #f8fafc; }"
        "QLineEdit { color: #f8fafc; background-color: rgba(255,255,255,10); border: 1px solid #8b5a2b; border-radius: 5px; padding: 5px; }"
        "QComboBox { color: #f8fafc; background-color: rgba(255,255,255,10); border: 1px solid #8b5a2b; border-radius: 5px; padding: 5px; }"
    );

    // Create form layout
    QFormLayout form(&dialog);
    form.setVerticalSpacing(12);

    QLineEdit lineEdit_nom;
    lineEdit_nom.setPlaceholderText("Entrez votre nom");
    form.addRow("Nom:", &lineEdit_nom);

    QLineEdit lineEdit_prenom;
    lineEdit_prenom.setPlaceholderText("Entrez votre prénom");
    form.addRow("Prénom:", &lineEdit_prenom);

    QLineEdit lineEdit_cin;
    lineEdit_cin.setMaxLength(20);
    lineEdit_cin.setPlaceholderText("CIN (8 chiffres)");
    form.addRow("CIN:", &lineEdit_cin);

    QDateEdit dateEdit_naissance(QDate::currentDate().addYears(-20));
    dateEdit_naissance.setCalendarPopup(true);
    dateEdit_naissance.setDisplayFormat("dd/MM/yyyy");
    form.addRow("Date de naissance:", &dateEdit_naissance);

    QLineEdit lineEdit_email;
    lineEdit_email.setPlaceholderText("exemple@domaine.com");
    form.addRow("E-mail:", &lineEdit_email);

    QLineEdit lineEdit_password;
    lineEdit_password.setEchoMode(QLineEdit::Password);
    lineEdit_password.setPlaceholderText("Minimum 6 caractères");
    form.addRow("Mot de passe:", &lineEdit_password);

    QLineEdit lineEdit_password_confirm;
    lineEdit_password_confirm.setEchoMode(QLineEdit::Password);
    lineEdit_password_confirm.setPlaceholderText("Confirmez le mot de passe");
    form.addRow("Confirmer mot de passe:", &lineEdit_password_confirm);

    QComboBox comboBox_role;
    comboBox_role.addItems({"DESIGNER", "MENUISIER", "ENGINEER", "CHEF_ATELIER"});
    form.addRow("Rôle:", &comboBox_role);

    QLineEdit lineEdit_photoPath;
    lineEdit_photoPath.setPlaceholderText("Chemin photo profil (optionnel)");
    QPushButton btnSelectPhoto("Sélectionner photo");
    QWidget photoWidget;
    QHBoxLayout photoLayout(&photoWidget);
    photoLayout.setContentsMargins(0, 0, 0, 0);
    photoLayout.addWidget(&lineEdit_photoPath);
    photoLayout.addWidget(&btnSelectPhoto);
    form.addRow("Photo profil:", &photoWidget);

    QLineEdit lineEdit_faceToken;
    lineEdit_faceToken.setReadOnly(true);
    lineEdit_faceToken.setPlaceholderText("Token Face ID (requis si bypass désactivé)");
    QPushButton btnEnrollFace("Enroll Face ID");
    QWidget faceWidget;
    QHBoxLayout faceLayout(&faceWidget);
    faceLayout.setContentsMargins(0, 0, 0, 0);
    faceLayout.addWidget(&lineEdit_faceToken);
    faceLayout.addWidget(&btnEnrollFace);
    form.addRow("Face ID:", &faceWidget);

    QLabel faceStatusLabel(isFaceIdTemporarilyBypassed()
                               ? "Face ID bypass activé (SC_SKIP_FACE_ID)"
                               : "Face ID requis: cliquez sur Enroll Face ID");
    faceStatusLabel.setStyleSheet("color:#cbd5e1;");
    form.addRow(QString(), &faceStatusLabel);

    QObject::connect(&btnSelectPhoto, &QPushButton::clicked, &dialog, [&]() {
        const QString filePath = QFileDialog::getOpenFileName(
            this,
            "Choisir une photo de profil",
            QString(),
            "Images (*.png *.jpg *.jpeg *.bmp *.webp)");
        if (!filePath.isEmpty()) {
            QString photoError;
            const QString storedPhotoPath = persistProfilePhotoUploadForLogin(filePath, &photoError);
            if (storedPhotoPath.isEmpty()) {
                QMessageBox::warning(&dialog, "Photo", photoError);
                return;
            }
            lineEdit_photoPath.setText(storedPhotoPath);
        }
    });

    QObject::connect(&btnEnrollFace, &QPushButton::clicked, &dialog, [&]() {
        faceid enrollmentDialog(this);
        if (enrollmentDialog.exec() != QDialog::Accepted) {
            faceStatusLabel.setText("Enroll Face ID annulé.");
            return;
        }

        const QString token = enrollmentDialog.getResult().trimmed();
        if (!isLikelyFaceToken(token)) {
            QMessageBox::warning(&dialog, "Face ID", "Face ID invalide. Veuillez réessayer.");
            faceStatusLabel.setText("Token Face ID invalide.");
            return;
        }

        lineEdit_faceToken.setText(token);
        faceStatusLabel.setText("Face ID enrollé avec succès.");
    });

    // Dialog buttons
    QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    buttons.button(QDialogButtonBox::Ok)->setText("Créer");
    buttons.button(QDialogButtonBox::Cancel)->setText("Annuler");
    form.addRow(&buttons);

    // Lambda for OK button click
    QObject::connect(&buttons, &QDialogButtonBox::accepted, &dialog, [&]() {
        // Get form data
        QString nom = lineEdit_nom.text().trimmed();
        QString prenom = lineEdit_prenom.text().trimmed();
        QString cin = lineEdit_cin.text().trimmed();
        QDate dateNaissance = dateEdit_naissance.date();
        QString email = lineEdit_email.text().trimmed().toLower();
        QString password = lineEdit_password.text();
        QString confirmPassword = lineEdit_password_confirm.text();
        QString role = comboBox_role.currentText();
        QString photoPath = lineEdit_photoPath.text().trimmed();
        QString enrolledFaceToken = lineEdit_faceToken.text().trimmed();

        qDebug() << "Creating account for:" << nom << prenom << email << "Role:" << role;

        // Comprehensive validation
        if (nom.isEmpty()) {
            QMessageBox::warning(&dialog, "Validation", "Le nom est requis.");
            lineEdit_nom.setFocus();
            return;
        }

        if (prenom.isEmpty()) {
            QMessageBox::warning(&dialog, "Validation", "Le prénom est requis.");
            lineEdit_prenom.setFocus();
            return;
        }

        if (cin.isEmpty()) {
            QMessageBox::warning(&dialog, "Validation", "Le CIN est requis.");
            lineEdit_cin.setFocus();
            return;
        }

        if (email.isEmpty()) {
            QMessageBox::warning(&dialog, "Validation", "L'adresse e-mail est requise.");
            lineEdit_email.setFocus();
            return;
        }

        if (password.isEmpty()) {
            QMessageBox::warning(&dialog, "Validation", "Le mot de passe est requis.");
            lineEdit_password.setFocus();
            return;
        }

        // Enhanced validation
        if (nom.length() < 2) {
            QMessageBox::warning(&dialog, "Validation", "Le nom doit contenir au moins 2 caractères.");
            lineEdit_nom.setFocus();
            return;
        }

        if (prenom.length() < 2) {
            QMessageBox::warning(&dialog, "Validation", "Le prénom doit contenir au moins 2 caractères.");
            lineEdit_prenom.setFocus();
            return;
        }

        QRegularExpression cinRegex(QStringLiteral("^[0-9]{8}$"));
        if (!cinRegex.match(cin).hasMatch()) {
            QMessageBox::warning(&dialog, "Validation", "CIN invalide (8 chiffres attendus).");
            lineEdit_cin.setFocus();
            return;
        }

        const int age = dateNaissance.daysTo(QDate::currentDate()) / 365;
        if (age < 18 || age > 100) {
            QMessageBox::warning(&dialog, "Validation", "Date de naissance invalide ( ge entre 18 et 100 ans).");
            dateEdit_naissance.setFocus();
            return;
        }

        if (password.length() < 6) {
            QMessageBox::warning(&dialog, "Validation", "Le mot de passe doit contenir au moins 6 caractères.");
            lineEdit_password.setFocus();
            return;
        }

        if (password != confirmPassword) {
            QMessageBox::warning(&dialog, "Validation", "Les mots de passe ne correspondent pas.");
            lineEdit_password_confirm.clear();
            lineEdit_password_confirm.setFocus();
            return;
        }

        // Enhanced email validation
        QRegularExpression emailRegex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
        if (!emailRegex.match(email).hasMatch()) {
            QMessageBox::warning(&dialog, "Validation", "Adresse e-mail non valide.");
            lineEdit_email.setFocus();
            return;
        }

        // Check if email already exists
        if (isEmailAlreadyExists(email)) {
            QMessageBox::warning(&dialog, "Validation", "Cette adresse e-mail est déjà utilisée.");
            lineEdit_email.setFocus();
            return;
        }

        // Enforce Face ID when bypass is disabled.
        if (!isFaceIdTemporarilyBypassed()) {
            if (!isLikelyFaceToken(enrolledFaceToken)) {
                QMessageBox::warning(&dialog, "Face ID", "Veuillez effectuer l'enrôlement Face ID avant de créer le compte.");
                return;
            }
        }

        // Create the account
        if (createNewAccount(nom,
                             prenom,
                             email,
                             password,
                             role,
                             enrolledFaceToken,
                             cin,
                             dateNaissance,
                             photoPath)) {
            accountCreated = true;
            createdAccountEmail = email;
            dialog.accept();
        } else {
            QMessageBox::warning(&dialog, "Erreur", "Erreur lors de la création du compte. Veuillez réessayer.");
        }
    });

    QObject::connect(&buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    // Show modal dialog
    dialog.exec();

    if (accountCreated) {
        ui->stackedWidgetlogin->setCurrentIndex(0);
        ui->id1->setText(createdAccountEmail);
        ui->mot1->clear();
        ui->mot1->setFocus();
        QMessageBox::information(this,
                                 "Compte créé",
                                 "Compte créé avec succès. Connectez-vous maintenant.");
    }
}

void login::on_pushButton_createNew_clicked()
{
    // This function is now only called from the old UI path (if accessed via stacked widget)
    // For the new modal dialog approach, this is essentially deprecated but kept for compatibility
    qDebug() << "on_pushButton_createNew_clicked called (legacy UI path)";
}

void login::on_btnRetour_createAccount_clicked()
{
    // Legacy function - kept for compatibility with old UI layout
    ui->stackedWidgetlogin->setCurrentIndex(0);
}

// ==============================
// AUTHENTICATION METHODS
// ==============================

bool login::authenticateUser(const QString &email, const QString &password)
{
    ConnSQL& db = ConnSQL::createInstance();

    authenticatedNom.clear();
    authenticatedPrenom.clear();
    authenticatedRole.clear();
    authenticatedFaceId.clear();
    authenticatedProfileImagePath.clear();
    authenticatedEmail.clear();
    
    // Check if database connection is available
    if (!db.getDB()->isOpen()) {
        showLoginError("Erreur de connexion à la base de données.");
        qDebug() << "Database connection is not open";
        return false;
    }
    
    qDebug() << "Attempting to authenticate user:" << email;
    
    // Prepare query to check user credentials (no case conversion for now)
    QSqlQuery query(*db.getDB());
    query.prepare("SELECT idEmployee, nom, prenom, role, face_id, photo_path, password, statut FROM EMPLOYEE WHERE LOWER(TRIM(mail)) = LOWER(TRIM(?))");
    query.addBindValue(email.trimmed());
    
    if (!query.exec()) {
        qDebug() << "Database query error:" << query.lastError().text();
        showLoginError("Erreur lors de la vérification des identifiants.");
        return false;
    }
    
    qDebug() << "Query executed successfully";
    
    if (query.next()) {
        const QString storedStatus = query.value("statut").toString().trimmed().toUpper();
        if (storedStatus == "ARCHIVE") {
            showLoginError("Ce compte est archivé. Contactez l'administrateur pour le réactiver.");
            return false;
        }

        const QString storedPassword = query.value("password").toString();
        qDebug() << "User found in database:" << query.value("nom").toString() << query.value("prenom").toString();

        if (verifyPasswordAgainstStored(password, storedPassword)) {
            authenticatedNom = query.value("nom").toString();
            authenticatedPrenom = query.value("prenom").toString();
            authenticatedRole = query.value("role").toString();
            authenticatedFaceId = query.value("face_id").toString();
            authenticatedProfileImagePath = query.value("photo_path").toString().trimmed();
            if (authenticatedProfileImagePath.isEmpty()) {
                authenticatedProfileImagePath = authenticatedFaceId.trimmed();
            }
            authenticatedEmail = email.trimmed();

            // Upgrade legacy plaintext password on successful login.
            if (!isStoredPasswordHash(storedPassword)) {
                QSqlQuery upgradeQuery(*db.getDB());
                upgradeQuery.prepare("UPDATE EMPLOYEE SET password = ? WHERE idEmployee = ?");
                upgradeQuery.addBindValue(hashPassword(password));
                upgradeQuery.addBindValue(query.value("idEmployee").toInt());
                if (!upgradeQuery.exec()) {
                    qDebug() << "Password hash upgrade failed for user" << email << ":" << upgradeQuery.lastError().text();
                }
            }

            qDebug() << "Authentication successful for user:" << query.value("nom").toString() 
                     << query.value("prenom").toString() 
                     << "Role:" << query.value("role").toString();
            return true;
        } else {
            qDebug() << "Password mismatch";
        }
    } else {
        qDebug() << "No user found with email:" << email;
    }
    
    return false; // Authentication failed
}

QString login::hashPassword(const QString &password)
{
    QByteArray salt(16, '\0');
    for (int i = 0; i < salt.size(); ++i) {
        salt[i] = static_cast<char>(QRandomGenerator::global()->bounded(256));
    }

    const QByteArray hash = QCryptographicHash::hash(salt + password.toUtf8(), QCryptographicHash::Sha256);
    return QString("sha256$%1$%2")
        .arg(QString::fromLatin1(salt.toHex()), QString::fromLatin1(hash.toHex()));
}

void login::createDefaultUsers()
{
    ConnSQL& db = ConnSQL::createInstance();
    
    // Check if database connection is available
    if (!db.getDB()->isOpen()) {
        qDebug() << "Database not connected - cannot create default users";
        return;
    }
    
    qDebug() << "Checking for existing users...";
    
    // Check if any users already exist
    QSqlQuery checkQuery(*db.getDB());
    checkQuery.prepare("SELECT COUNT(*) as user_count FROM EMPLOYEE");
    
    if (!checkQuery.exec() || !checkQuery.next()) {
        qDebug() << "Could not check existing users:" << checkQuery.lastError().text();
        return;
    }
    
    int userCount = checkQuery.value("user_count").toInt();
    qDebug() << "Found" << userCount << "users in database";
    
    // If no users exist, create default ones
    if (userCount == 0) {
        qDebug() << "No users found, creating default users...";
        
        QSqlQuery insertQuery(*db.getDB());
        
        // Create admin user
        insertQuery.prepare("INSERT INTO EMPLOYEE (idEmployee, mail, password, nom, prenom, role) "
                           "VALUES (1, ?, ?, ?, ?, ?)");
        insertQuery.addBindValue("admin@smartcarpentry.com");
        insertQuery.addBindValue(hashPassword("admin123"));
        insertQuery.addBindValue("Admin");
        insertQuery.addBindValue("User");
        insertQuery.addBindValue("ADMIN");
        
        if (insertQuery.exec()) {
            qDebug() << "Created default admin user";
        } else {
            qDebug() << "Failed to create admin user:" << insertQuery.lastError().text();
        }
        
        // Create designer user
        insertQuery.prepare("INSERT INTO EMPLOYEE (idEmployee, mail, password, nom, prenom, role) "
                           "VALUES (2, ?, ?, ?, ?, ?)");
        insertQuery.addBindValue("designer@smartcarpentry.com");
        insertQuery.addBindValue(hashPassword("design123"));
        insertQuery.addBindValue("Designer");
        insertQuery.addBindValue("User");
        insertQuery.addBindValue("DESIGNER");
        
        if (insertQuery.exec()) {
            qDebug() << "Created default designer user";
        } else {
            qDebug() << "Failed to create designer user:" << insertQuery.lastError().text();
        }
        
        // Create menuisier user
        insertQuery.prepare("INSERT INTO EMPLOYEE (idEmployee, mail, password, nom, prenom, role) "
                           "VALUES (3, ?, ?, ?, ?, ?)");
        insertQuery.addBindValue("menuisier@smartcarpentry.com");
        insertQuery.addBindValue(hashPassword("menu123"));
        insertQuery.addBindValue("Menuisier");
        insertQuery.addBindValue("User");
        insertQuery.addBindValue("MENUISIER");
        
        if (insertQuery.exec()) {
            qDebug() << "Created default menuisier user";
        } else {
            qDebug() << "Failed to create menuisier user:" << insertQuery.lastError().text();
        }
        
        qDebug() << "Finished creating default users";
    } else {
        qDebug() << "Users already exist in database, skipping creation";
    }
}

void login::showLoginError(const QString &message)
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle("Erreur de connexion");
    msgBox.setText(message);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

void login::debugDatabaseUsers()
{
    ConnSQL& db = ConnSQL::createInstance();
    
    if (!db.getDB()->isOpen()) {
        qDebug() << "Cannot debug users - database not connected";
        return;
    }
    
    QSqlQuery query(*db.getDB());
    query.prepare("SELECT idEmployee, mail, password, nom, prenom, role FROM EMPLOYEE");
    
    if (!query.exec()) {
        qDebug() << "Failed to query users:" << query.lastError().text();
        return;
    }
    
    qDebug() << "=== DEBUG: Users in database ===";
    int count = 0;
    while (query.next()) {
        count++;
        qDebug() << "User" << count << ":"
                 << "ID:" << query.value("idEmployee").toInt()
                 << "Email:" << query.value("mail").toString()
                 << "PasswordStored:" << (query.value("password").toString().startsWith("sha256$") ? "HASHED" : "PLAINTEXT")
                 << "Name:" << query.value("nom").toString() << query.value("prenom").toString()
                 << "Role:" << query.value("role").toString();
    }
    
    if (count == 0) {
        qDebug() << "No users found in database!";
    } else {
        qDebug() << "Total users found:" << count;
    }
    qDebug() << "=== End users debug ===";
}

// ==============================
// ACCOUNT CREATION METHODS
// ==============================

bool login::createNewAccount(const QString &nom, const QString &prenom, const QString &email,
                           const QString &password, const QString &role,
                           const QString &faceIdToken,
                           const QString &cin,
                           const QDate &dateNaissance,
                           const QString &photoPath)
{
    ConnSQL& db = ConnSQL::createInstance();
    
    if (!db.getDB()->isOpen()) {
        qDebug() << "Database not connected - cannot create account";
        return false;
    }
  
    // Get next available ID
    int nextId = getNextEmployeeId();
    if (nextId == -1) {
        qDebug() << "Failed to get next employee ID";
        return false;
    }
    
    // Insert new user
    QSqlQuery insertQuery(*db.getDB());
    insertQuery.prepare("INSERT INTO EMPLOYEE (idEmployee, CIN, mail, password, nom, prenom, date_naissance, photo_path, role, face_id) "
                       "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    insertQuery.addBindValue(nextId);
    insertQuery.addBindValue(cin.trimmed());
    insertQuery.addBindValue(email);
    insertQuery.addBindValue(hashPassword(password));
    insertQuery.addBindValue(nom);
    insertQuery.addBindValue(prenom);
    insertQuery.addBindValue(dateNaissance);
    insertQuery.addBindValue(photoPath);
    insertQuery.addBindValue(role);
    insertQuery.addBindValue(faceIdToken.trimmed());
    
    if (!insertQuery.exec()) {
        qDebug() << "Failed to create account:" << insertQuery.lastError().text();
        return false;
    }
    
    if (!faceIdToken.trimmed().isEmpty()) {
        QSqlQuery faceTimestampQuery(*db.getDB());
        faceTimestampQuery.prepare("UPDATE EMPLOYEE SET face_ts = CURRENT_TIMESTAMP WHERE idEmployee = ?");
        faceTimestampQuery.addBindValue(nextId);
        if (!faceTimestampQuery.exec()) {
            qDebug() << "Failed to update face_ts for" << email << ":" << faceTimestampQuery.lastError().text();
        }
    }

    qDebug() << "Successfully created account for:" << email << "with ID:" << nextId;
    return true;
}

bool login::isEmailAlreadyExists(const QString &email)
{
    ConnSQL& db = ConnSQL::createInstance();
    
    if (!db.getDB()->isOpen()) {
        qDebug() << "Database not connected - cannot check email";
        return true; // Assume it exists to be safe
    }
    
    QSqlQuery query(*db.getDB());
    query.prepare("SELECT COUNT(*) as email_count FROM EMPLOYEE WHERE mail = ?");
    query.addBindValue(email);
    
    if (!query.exec() || !query.next()) {
        qDebug() << "Failed to check email existence:" << query.lastError().text();
        return true; // Assume it exists to be safe
    }
    
    int count = query.value("email_count").toInt();
    qDebug() << "Email check for" << email << "- count:" << count;
    return count > 0;
}

int login::getNextEmployeeId()
{
    ConnSQL& db = ConnSQL::createInstance();
    
    if (!db.getDB()->isOpen()) {
        return -1;
    }
    
    QSqlQuery query(*db.getDB());
    query.prepare("SELECT MAX(idEmployee) as max_id FROM EMPLOYEE");
    
    if (!query.exec() || !query.next()) {
        qDebug() << "Failed to get max employee ID:" << query.lastError().text();
        return -1;
    }
    
    int maxId = query.value("max_id").toInt();
    int nextId = maxId + 1;
    
    qDebug() << "Next employee ID:" << nextId;
    return nextId;
}

void login::showAccountCreationError(const QString &message)
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setWindowTitle("Erreur de création de compte");
    msgBox.setText(message);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

void login::showAccountCreationSuccess(const QString &message)
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowTitle("Compte créé");
    msgBox.setText(message);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

// ==============================
// KEYBOARD EVENT HANDLING
// ==============================
void login::keyPressEvent(QKeyEvent *event)
{
    if (!this->isVisible()) {
        QWidget::keyPressEvent(event);
        return;
    }
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        int currentPage = ui->stackedWidgetlogin->currentIndex();
        
        if (currentPage == 0) { // Login page
            on_pushButton_2_clicked();
        } else if (currentPage == 1) { // Create account page
            on_pushButton_createNew_clicked();
        }
        return;
    }
    
    // Pass the event to the base class
    QWidget::keyPressEvent(event);
}
