/********************************************************************************
** Form generated from reading UI file 'login.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_login
{
public:
    QStackedWidget *stackedWidgetlogin;
    QWidget *page;
    QLabel *backgroundLabel;
    QLabel *label_m;
    QLineEdit *id1;
    QLabel *label_3;
    QLabel *label_2;
    QLineEdit *mot1;
    QPushButton *pushButton_2;
    QPushButton *pushButton_createAccount;
    QWidget *page_createAccount;
    QLabel *backgroundLabel_3;
    QLabel *label_createAccount_title;
    QLabel *label_nom;
    QLineEdit *lineEdit_nom;
    QLabel *label_prenom;
    QLineEdit *lineEdit_prenom;
    QLabel *label_email_new;
    QLineEdit *lineEdit_email_new;
    QLabel *label_password_new;
    QLineEdit *lineEdit_password_new;
    QLabel *label_password_confirm;
    QLineEdit *lineEdit_password_confirm;
    QLabel *label_role;
    QComboBox *comboBox_role;
    QPushButton *pushButton_createNew;
    QPushButton *btnRetour_createAccount;
    QWidget *page_2;
    QStackedWidget *stackedWidget_2;
    QWidget *page_3;
    QLabel *backgroundLabel_2;
    QPushButton *pushButton_r;
    QLabel *label;
    QLineEdit *lineEditr;
    QLabel *label_5;
    QPushButton *pushButton_v;
    QWidget *page_4;

    void setupUi(QWidget *login)
    {
        if (login->objectName().isEmpty())
            login->setObjectName("login");
        login->resize(1550, 820);
        login->setStyleSheet(QString::fromUtf8(""));
        stackedWidgetlogin = new QStackedWidget(login);
        stackedWidgetlogin->setObjectName("stackedWidgetlogin");
        stackedWidgetlogin->setGeometry(QRect(0, 0, 1550, 820));
        page = new QWidget();
        page->setObjectName("page");
        backgroundLabel = new QLabel(page);
        backgroundLabel->setObjectName("backgroundLabel");
        backgroundLabel->setGeometry(QRect(0, 0, 1550, 820));
        backgroundLabel->setStyleSheet(QString::fromUtf8("background-image: url(:/Atelier.png);"));
        label_m = new QLabel(page);
        label_m->setObjectName("label_m");
        label_m->setGeometry(QRect(630, 550, 231, 20));
        label_m->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #8b5a2b;      /* bois fonc\303\251 */\n"
"    color: white;\n"
"    border: 2px solid #5c3a1e;\n"
"    border-radius: 8px;\n"
"    padding: 8px 20px;\n"
"    font-size: 14px;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #a06a3b;      /* bois plus clair */\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #6f4422;\n"
"}\n"
""));
        id1 = new QLineEdit(page);
        id1->setObjectName("id1");
        id1->setGeometry(QRect(630, 380, 291, 41));
        id1->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    color: white !important;\n"
"    background-color: rgba(255,255,255,20);\n"
"    border: 2px solid #8b5a2b;\n"
"    border-radius: 8px;\n"
"    padding: 8px;\n"
"    font-size: 12pt;\n"
"}\n"
"QLineEdit:focus {\n"
"    border-color: #a06a3b;\n"
"}"));
        label_3 = new QLabel(page);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(630, 440, 121, 20));
        label_3->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #8b5a2b;      /* bois fonc\303\251 */\n"
"    color: white;\n"
"    border: 2px solid #5c3a1e;\n"
"    border-radius: 8px;\n"
"    padding: 8px 20px;\n"
"    font-size: 14px;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #a06a3b;      /* bois plus clair */\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #6f4422;\n"
"}\n"
""));
        label_2 = new QLabel(page);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(630, 340, 81, 20));
        label_2->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #8b5a2b;      /* bois fonc\303\251 */\n"
"    color: white;\n"
"    border: 2px solid #5c3a1e;\n"
"    border-radius: 8px;\n"
"    padding: 8px 20px;\n"
"    font-size: 14px;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #a06a3b;      /* bois plus clair */\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #6f4422;\n"
"}\n"
""));
        mot1 = new QLineEdit(page);
        mot1->setObjectName("mot1");
        mot1->setGeometry(QRect(630, 480, 291, 41));
        mot1->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    color: white !important;\n"
"    background-color: rgba(255,255,255,20);\n"
"    border: 2px solid #8b5a2b;\n"
"    border-radius: 8px;\n"
"    padding: 8px;\n"
"    font-size: 12pt;\n"
"}\n"
"QLineEdit:focus {\n"
"    border-color: #a06a3b;\n"
"}"));
        mot1->setEchoMode(QLineEdit::EchoMode::Password);
        pushButton_2 = new QPushButton(page);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(630, 640, 291, 61));
        pushButton_2->setStyleSheet(QString::fromUtf8("#pushButton_2 {\n"
"    font-family: Arial, Helvetica, sans-serif;\n"
"    font-weight: bold;\n"
"    color: white;\n"
"    background-color: #171717;\n"
"    padding: 10px 20px;\n"
"    border: none;\n"
"    border-radius: 10px;\n"
"}\n"
"\n"
"#pushButton_2:hover {\n"
"    background-color: #0c66ed; /* hover effect */\n"
"}\n"
"\n"
"#pushButton_2:pressed {\n"
"    background-color: #094bb0; /* pressed effect */\n"
"}"));
        pushButton_createAccount = new QPushButton(page);
        pushButton_createAccount->setObjectName("pushButton_createAccount");
        pushButton_createAccount->setGeometry(QRect(630, 580, 291, 41));
        pushButton_createAccount->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    font-family: Arial, Helvetica, sans-serif;\n"
"    font-weight: bold;\n"
"    color: #8b5a2b;\n"
"    background-color: transparent;\n"
"    padding: 10px 20px;\n"
"    border: 2px solid #8b5a2b;\n"
"    border-radius: 10px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #8b5a2b;\n"
"    color: white;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #6f4422;\n"
"}"));
        stackedWidgetlogin->addWidget(page);
        page_createAccount = new QWidget();
        page_createAccount->setObjectName("page_createAccount");
        backgroundLabel_3 = new QLabel(page_createAccount);
        backgroundLabel_3->setObjectName("backgroundLabel_3");
        backgroundLabel_3->setGeometry(QRect(0, 0, 1550, 820));
        backgroundLabel_3->setStyleSheet(QString::fromUtf8("background-image: url(:/Atelier.png);"));
        label_createAccount_title = new QLabel(page_createAccount);
        label_createAccount_title->setObjectName("label_createAccount_title");
        label_createAccount_title->setGeometry(QRect(580, 190, 391, 51));
        label_createAccount_title->setStyleSheet(QString::fromUtf8("color: white; font-size: 20pt; font-weight: bold; text-align: center;"));
        label_createAccount_title->setAlignment(Qt::AlignCenter);
        label_nom = new QLabel(page_createAccount);
        label_nom->setObjectName("label_nom");
        label_nom->setGeometry(QRect(630, 260, 121, 20));
        label_nom->setStyleSheet(QString::fromUtf8("color: white;"));
        lineEdit_nom = new QLineEdit(page_createAccount);
        lineEdit_nom->setObjectName("lineEdit_nom");
        lineEdit_nom->setGeometry(QRect(630, 280, 141, 31));
        lineEdit_nom->setStyleSheet(QString::fromUtf8("QLineEdit { color: white !important; background-color: rgba(255,255,255,20); border: 1px solid #8b5a2b; border-radius: 5px; padding: 5px; }"));
        label_prenom = new QLabel(page_createAccount);
        label_prenom->setObjectName("label_prenom");
        label_prenom->setGeometry(QRect(780, 260, 121, 20));
        label_prenom->setStyleSheet(QString::fromUtf8("color: white;"));
        lineEdit_prenom = new QLineEdit(page_createAccount);
        lineEdit_prenom->setObjectName("lineEdit_prenom");
        lineEdit_prenom->setGeometry(QRect(780, 280, 141, 31));
        lineEdit_prenom->setStyleSheet(QString::fromUtf8("QLineEdit { color: white !important; background-color: rgba(255,255,255,20); border: 1px solid #8b5a2b; border-radius: 5px; padding: 5px; }"));
        label_email_new = new QLabel(page_createAccount);
        label_email_new->setObjectName("label_email_new");
        label_email_new->setGeometry(QRect(630, 320, 121, 20));
        label_email_new->setStyleSheet(QString::fromUtf8("color: white;"));
        lineEdit_email_new = new QLineEdit(page_createAccount);
        lineEdit_email_new->setObjectName("lineEdit_email_new");
        lineEdit_email_new->setGeometry(QRect(630, 340, 291, 31));
        lineEdit_email_new->setStyleSheet(QString::fromUtf8("QLineEdit { color: white !important; background-color: rgba(255,255,255,20); border: 1px solid #8b5a2b; border-radius: 5px; padding: 5px; }"));
        label_password_new = new QLabel(page_createAccount);
        label_password_new->setObjectName("label_password_new");
        label_password_new->setGeometry(QRect(630, 380, 121, 20));
        label_password_new->setStyleSheet(QString::fromUtf8("color: white;"));
        lineEdit_password_new = new QLineEdit(page_createAccount);
        lineEdit_password_new->setObjectName("lineEdit_password_new");
        lineEdit_password_new->setGeometry(QRect(630, 400, 291, 31));
        lineEdit_password_new->setStyleSheet(QString::fromUtf8("QLineEdit { color: white !important; background-color: rgba(255,255,255,20); border: 1px solid #8b5a2b; border-radius: 5px; padding: 5px; }"));
        lineEdit_password_new->setEchoMode(QLineEdit::EchoMode::Password);
        label_password_confirm = new QLabel(page_createAccount);
        label_password_confirm->setObjectName("label_password_confirm");
        label_password_confirm->setGeometry(QRect(630, 440, 141, 20));
        label_password_confirm->setStyleSheet(QString::fromUtf8("color: white;"));
        lineEdit_password_confirm = new QLineEdit(page_createAccount);
        lineEdit_password_confirm->setObjectName("lineEdit_password_confirm");
        lineEdit_password_confirm->setGeometry(QRect(630, 460, 291, 31));
        lineEdit_password_confirm->setStyleSheet(QString::fromUtf8("QLineEdit { color: white !important; background-color: rgba(255,255,255,20); border: 1px solid #8b5a2b; border-radius: 5px; padding: 5px; }"));
        lineEdit_password_confirm->setEchoMode(QLineEdit::EchoMode::Password);
        label_role = new QLabel(page_createAccount);
        label_role->setObjectName("label_role");
        label_role->setGeometry(QRect(630, 500, 121, 20));
        label_role->setStyleSheet(QString::fromUtf8("color: white;"));
        comboBox_role = new QComboBox(page_createAccount);
        comboBox_role->addItem(QString());
        comboBox_role->addItem(QString());
        comboBox_role->addItem(QString());
        comboBox_role->addItem(QString());
        comboBox_role->addItem(QString());
        comboBox_role->setObjectName("comboBox_role");
        comboBox_role->setGeometry(QRect(630, 520, 291, 31));
        comboBox_role->setStyleSheet(QString::fromUtf8("QComboBox { color: white; background-color: rgba(255,255,255,20); border: 1px solid #8b5a2b; border-radius: 5px; padding: 5px; }\n"
"QComboBox::drop-down { border: none; }\n"
"QComboBox::down-arrow { image: url(down_arrow.png); }"));
        pushButton_createNew = new QPushButton(page_createAccount);
        pushButton_createNew->setObjectName("pushButton_createNew");
        pushButton_createNew->setGeometry(QRect(630, 570, 141, 41));
        pushButton_createNew->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    font-family: Arial, Helvetica, sans-serif;\n"
"    font-weight: bold;\n"
"    color: white;\n"
"    background-color: #171717;\n"
"    padding: 10px 20px;\n"
"    border: none;\n"
"    border-radius: 10px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #0c66ed;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #094bb0;\n"
"}"));
        btnRetour_createAccount = new QPushButton(page_createAccount);
        btnRetour_createAccount->setObjectName("btnRetour_createAccount");
        btnRetour_createAccount->setGeometry(QRect(780, 570, 141, 41));
        btnRetour_createAccount->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    font-family: Arial, Helvetica, sans-serif;\n"
"    font-weight: bold;\n"
"    color: #8b5a2b;\n"
"    background-color: transparent;\n"
"    padding: 10px 20px;\n"
"    border: 2px solid #8b5a2b;\n"
"    border-radius: 10px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #8b5a2b;\n"
"    color: white;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #6f4422;\n"
"}"));
        stackedWidgetlogin->addWidget(page_createAccount);
        page_2 = new QWidget();
        page_2->setObjectName("page_2");
        stackedWidget_2 = new QStackedWidget(page_2);
        stackedWidget_2->setObjectName("stackedWidget_2");
        stackedWidget_2->setGeometry(QRect(0, 0, 1550, 820));
        page_3 = new QWidget();
        page_3->setObjectName("page_3");
        backgroundLabel_2 = new QLabel(page_3);
        backgroundLabel_2->setObjectName("backgroundLabel_2");
        backgroundLabel_2->setGeometry(QRect(0, 0, 1550, 820));
        backgroundLabel_2->setStyleSheet(QString::fromUtf8("background-image: url(:/Atelier.png);"));
        pushButton_r = new QPushButton(page_3);
        pushButton_r->setObjectName("pushButton_r");
        pushButton_r->setGeometry(QRect(630, 640, 291, 61));
        pushButton_r->setStyleSheet(QString::fromUtf8("#pushButton_2 {\n"
"    font-family: Arial, Helvetica, sans-serif;\n"
"    font-weight: bold;\n"
"    color: white;\n"
"    background-color: #171717;\n"
"    padding: 10px 20px;\n"
"    border: none;\n"
"    border-radius: 10px;\n"
"}\n"
"\n"
"#pushButton_2:hover {\n"
"    background-color: #0c66ed; /* hover effect */\n"
"}\n"
"\n"
"#pushButton_2:pressed {\n"
"    background-color: #094bb0; /* pressed effect */\n"
"}"));
        label = new QLabel(page_3);
        label->setObjectName("label");
        label->setGeometry(QRect(620, 290, 321, 81));
        label->setStyleSheet(QString::fromUtf8("  color: white;"));
        lineEditr = new QLineEdit(page_3);
        lineEditr->setObjectName("lineEditr");
        lineEditr->setGeometry(QRect(630, 440, 291, 41));
        lineEditr->setStyleSheet(QString::fromUtf8("QWidget {\n"
"    color: black;\n"
"}\n"
"QLineEdit {\n"
"    color: white !important;\n"
"}\n"
""));
        label_5 = new QLabel(page_3);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(630, 400, 121, 20));
        label_5->setStyleSheet(QString::fromUtf8("  color: white;"));
        pushButton_v = new QPushButton(page_3);
        pushButton_v->setObjectName("pushButton_v");
        pushButton_v->setGeometry(QRect(720, 510, 101, 41));
        pushButton_v->setStyleSheet(QString::fromUtf8("#pushButton_2 {\n"
"    font-family: Arial, Helvetica, sans-serif;\n"
"    font-weight: bold;\n"
"    color: white;\n"
"    background-color: #171717;\n"
"    padding: 10px 20px;\n"
"    border: none;\n"
"    border-radius: 10px;\n"
"}\n"
"\n"
"#pushButton_2:hover {\n"
"    background-color: #0c66ed; /* hover effect */\n"
"}\n"
"\n"
"#pushButton_2:pressed {\n"
"    background-color: #094bb0; /* pressed effect */\n"
"}"));
        stackedWidget_2->addWidget(page_3);
        page_4 = new QWidget();
        page_4->setObjectName("page_4");
        stackedWidget_2->addWidget(page_4);
        stackedWidgetlogin->addWidget(page_2);

        retranslateUi(login);

        stackedWidgetlogin->setCurrentIndex(0);
        stackedWidget_2->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(login);
    } // setupUi

    void retranslateUi(QWidget *login)
    {
        login->setWindowTitle(QCoreApplication::translate("login", "SmartCarpentry - Connexion", nullptr));
        backgroundLabel->setText(QString());
        label_m->setText(QCoreApplication::translate("login", "<a href=\"#\">Mot de passe oubli\303\251 ?</a>\n"
"", nullptr));
        id1->setPlaceholderText(QCoreApplication::translate("login", "Entrez votre e-mail", nullptr));
        label_3->setText(QCoreApplication::translate("login", "Mot de passe :", nullptr));
        label_2->setText(QCoreApplication::translate("login", "Email:", nullptr));
        mot1->setPlaceholderText(QCoreApplication::translate("login", "Entrez votre mot de passe", nullptr));
        pushButton_2->setText(QCoreApplication::translate("login", "Se connecter", nullptr));
        pushButton_createAccount->setText(QCoreApplication::translate("login", "Cr\303\251er un compte", nullptr));
        backgroundLabel_3->setText(QString());
        label_createAccount_title->setText(QCoreApplication::translate("login", "Cr\303\251er un nouveau compte", nullptr));
        label_nom->setText(QCoreApplication::translate("login", "Nom:", nullptr));
        label_prenom->setText(QCoreApplication::translate("login", "Pr\303\251nom:", nullptr));
        label_email_new->setText(QCoreApplication::translate("login", "E-mail:", nullptr));
        label_password_new->setText(QCoreApplication::translate("login", "Mot de passe:", nullptr));
        label_password_confirm->setText(QCoreApplication::translate("login", "Confirmer mot de passe:", nullptr));
        label_role->setText(QCoreApplication::translate("login", "R\303\264le:", nullptr));
        comboBox_role->setItemText(0, QCoreApplication::translate("login", "ADMIN", nullptr));
        comboBox_role->setItemText(1, QCoreApplication::translate("login", "DESIGNER", nullptr));
        comboBox_role->setItemText(2, QCoreApplication::translate("login", "MENUISIER", nullptr));
        comboBox_role->setItemText(3, QCoreApplication::translate("login", "ENGINEER", nullptr));
        comboBox_role->setItemText(4, QCoreApplication::translate("login", "CHEF_ATELIER", nullptr));

        pushButton_createNew->setText(QCoreApplication::translate("login", "Cr\303\251er", nullptr));
        btnRetour_createAccount->setText(QCoreApplication::translate("login", "Retour", nullptr));
        backgroundLabel_2->setText(QString());
        pushButton_r->setText(QCoreApplication::translate("login", "Retour", nullptr));
        label->setText(QCoreApplication::translate("login", "<html><head/><body><p><span style=\" font-size:16pt; font-weight:700;\">R\303\251cup\303\251ration de compte</span></p></body></html>", nullptr));
        lineEditr->setText(QString());
        label_5->setText(QCoreApplication::translate("login", "<html><head/><body><p><span style=\" font-size:10pt;\">Adresse e-mail</span></p></body></html>", nullptr));
        pushButton_v->setText(QCoreApplication::translate("login", "Valid\303\251e", nullptr));
    } // retranslateUi

};

namespace Ui {
    class login: public Ui_login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
