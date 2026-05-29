/********************************************************************************
** Form generated from reading UI file 'faceid.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FACEID_H
#define UI_FACEID_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_faceid
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QPushButton *pushButton;

    void setupUi(QDialog *faceid)
    {
        if (faceid->objectName().isEmpty())
            faceid->setObjectName("faceid");
        faceid->resize(640, 420);
        verticalLayout = new QVBoxLayout(faceid);
        verticalLayout->setObjectName("verticalLayout");
        label = new QLabel(faceid);
        label->setObjectName("label");
        label->setMinimumSize(QSize(0, 320));
        label->setStyleSheet(QString::fromUtf8("background-color: #111827; color: white; border-radius: 12px; padding: 16px;"));
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);

        pushButton = new QPushButton(faceid);
        pushButton->setObjectName("pushButton");
        pushButton->setMinimumHeight(42);

        verticalLayout->addWidget(pushButton);


        retranslateUi(faceid);

        QMetaObject::connectSlotsByName(faceid);
    } // setupUi

    void retranslateUi(QDialog *faceid)
    {
        faceid->setWindowTitle(QCoreApplication::translate("faceid", "Face ID", nullptr));
        label->setText(QCoreApplication::translate("faceid", "Starting camera...", nullptr));
        pushButton->setText(QCoreApplication::translate("faceid", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class faceid: public Ui_faceid {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FACEID_H
