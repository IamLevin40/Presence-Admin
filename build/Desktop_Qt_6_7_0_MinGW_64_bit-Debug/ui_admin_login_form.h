/********************************************************************************
** Form generated from reading UI file 'admin_login_form.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADMIN_LOGIN_FORM_H
#define UI_ADMIN_LOGIN_FORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Admin_Login_Form
{
public:
    QWidget *centralwidget;

    void setupUi(QMainWindow *Admin_Login_Form)
    {
        if (Admin_Login_Form->objectName().isEmpty())
            Admin_Login_Form->setObjectName("Admin_Login_Form");
        Admin_Login_Form->resize(360, 640);
        centralwidget = new QWidget(Admin_Login_Form);
        centralwidget->setObjectName("centralwidget");
        Admin_Login_Form->setCentralWidget(centralwidget);

        retranslateUi(Admin_Login_Form);

        QMetaObject::connectSlotsByName(Admin_Login_Form);
    } // setupUi

    void retranslateUi(QMainWindow *Admin_Login_Form)
    {
        Admin_Login_Form->setWindowTitle(QCoreApplication::translate("Admin_Login_Form", "MainWindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Admin_Login_Form: public Ui_Admin_Login_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADMIN_LOGIN_FORM_H
