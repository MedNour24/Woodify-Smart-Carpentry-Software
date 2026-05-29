#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QRegularExpression>
#include <QKeyEvent>
#include <QDate>
#include "connsql.h"

namespace Ui {
class login;
}

class login : public QWidget
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();

private slots:
    void on_pushButton_r_clicked();   // Retour vers login depuis la 2e page
    void on_pushButton_2_clicked();   // Se connecter
    void on_label_m_linkActivated(const QString &link);     // Mot de passe oublié
    void on_btnRetour_clicked();      // Retour vers login
    void on_pushButton_createAccount_clicked(); // Aller vers page création compte
    void on_pushButton_createNew_clicked();     // Créer nouveau compte
    void on_btnRetour_createAccount_clicked();  // Retour de création vers login

protected:
    void keyPressEvent(QKeyEvent *event) override; // Handle Enter key

private:
    Ui::login *ui;

    QString authenticatedNom;
    QString authenticatedPrenom;
    QString authenticatedRole;
    QString authenticatedFaceId;
    QString authenticatedProfileImagePath;
    QString authenticatedEmail;
    
    // Authentication methods
    bool authenticateUser(const QString &email, const QString &password);
    QString hashPassword(const QString &password);
    void createDefaultUsers(); // Create sample users if not exist
    void showLoginError(const QString &message);
    void debugDatabaseUsers(); // Debug method to list all users
    
    // Account creation methods
    bool createNewAccount(const QString &nom, const QString &prenom, const QString &email,
                         const QString &password, const QString &role,
                         const QString &faceIdToken,
                         const QString &cin,
                         const QDate &dateNaissance,
                         const QString &photoPath);
    bool isEmailAlreadyExists(const QString &email);
    void showAccountCreationError(const QString &message);
    void showAccountCreationSuccess(const QString &message);
    int getNextEmployeeId();
};

#endif // LOGIN_H
