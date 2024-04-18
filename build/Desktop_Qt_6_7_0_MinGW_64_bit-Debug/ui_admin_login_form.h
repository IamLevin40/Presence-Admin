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
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Admin_Login_Form
{
public:
    QWidget *centralwidget;
    QGroupBox *appGroup;
    QLabel *presenceIcon;
    QLabel *appTitleLabel;
    QLabel *appSubtitleLabel;
    QGroupBox *formGroup;
    QLabel *background;
    QGroupBox *adminIdGroup;
    QLabel *adminIdHud;
    QLineEdit *adminIdTextbox;
    QGroupBox *adminPinGroup;
    QLabel *adminPinHud;
    QLineEdit *adminPinTextbox;
    QPushButton *loginButton;
    QLabel *errorLabel;

    void setupUi(QMainWindow *Admin_Login_Form)
    {
        if (Admin_Login_Form->objectName().isEmpty())
            Admin_Login_Form->setObjectName("Admin_Login_Form");
        Admin_Login_Form->resize(360, 640);
        Admin_Login_Form->setStyleSheet(QString::fromUtf8(""));
        centralwidget = new QWidget(Admin_Login_Form);
        centralwidget->setObjectName("centralwidget");
        centralwidget->setStyleSheet(QString::fromUtf8("QWidget #centralwidget {\n"
"	background-color: #000F2A;\n"
"}"));
        appGroup = new QGroupBox(centralwidget);
        appGroup->setObjectName("appGroup");
        appGroup->setGeometry(QRect(30, 20, 300, 200));
        appGroup->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"	border: 0px;\n"
"	border-radius: 0px;\n"
"}"));
        appGroup->setFlat(true);
        appGroup->setCheckable(false);
        presenceIcon = new QLabel(appGroup);
        presenceIcon->setObjectName("presenceIcon");
        presenceIcon->setGeometry(QRect(0, 0, 200, 200));
        presenceIcon->setStyleSheet(QString::fromUtf8(""));
        presenceIcon->setPixmap(QPixmap(QString::fromUtf8(":/res/assets/presence_logo.png")));
        presenceIcon->setScaledContents(false);
        appTitleLabel = new QLabel(appGroup);
        appTitleLabel->setObjectName("appTitleLabel");
        appTitleLabel->setGeometry(QRect(140, 80, 151, 31));
        QFont font;
        font.setFamilies({QString::fromUtf8("Poppins")});
        font.setWeight(QFont::ExtraBold);
        font.setItalic(false);
        appTitleLabel->setFont(font);
        appTitleLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	color: #FFFFFF;\n"
"	font-family: Poppins;\n"
"	font-size: 35px;\n"
"	font-style: normal;\n"
"	font-weight: 800;\n"
"	line-height: normal;\n"
"}\n"
""));
        appTitleLabel->setText(QString::fromUtf8("resence"));
        appSubtitleLabel = new QLabel(appGroup);
        appSubtitleLabel->setObjectName("appSubtitleLabel");
        appSubtitleLabel->setGeometry(QRect(160, 110, 131, 20));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Montserrat Medium")});
        font1.setBold(false);
        font1.setItalic(false);
        appSubtitleLabel->setFont(font1);
        appSubtitleLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	color: #15CAE3;\n"
"	font-size: 9px;\n"
"	font-style: normal;\n"
"	font-weight: 400;\n"
"	line-height: normal;\n"
"}"));
        appSubtitleLabel->setText(QString::fromUtf8("KEEP IN TRACK EVERYTIME"));
        formGroup = new QGroupBox(centralwidget);
        formGroup->setObjectName("formGroup");
        formGroup->setGeometry(QRect(0, 210, 360, 430));
        formGroup->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"	border: 0px;\n"
"	border-radius: 0px;\n"
"}"));
        background = new QLabel(formGroup);
        background->setObjectName("background");
        background->setGeometry(QRect(0, 0, 360, 430));
        background->setPixmap(QPixmap(QString::fromUtf8(":/res/assets/form_background.png")));
        background->setScaledContents(true);
        adminIdGroup = new QGroupBox(formGroup);
        adminIdGroup->setObjectName("adminIdGroup");
        adminIdGroup->setEnabled(true);
        adminIdGroup->setGeometry(QRect(55, 110, 250, 40));
        adminIdGroup->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"	border: 0px;\n"
"	border-radius: 0px;\n"
"	background: none\n"
"}"));
        adminIdGroup->setAlignment(Qt::AlignCenter);
        adminIdGroup->setFlat(true);
        adminIdHud = new QLabel(adminIdGroup);
        adminIdHud->setObjectName("adminIdHud");
        adminIdHud->setGeometry(QRect(0, 0, 250, 40));
        adminIdHud->setStyleSheet(QString::fromUtf8("QLabel {	\n"
"	background: none;\n"
"}"));
        adminIdHud->setPixmap(QPixmap(QString::fromUtf8(":/res/assets/login_field.png")));
        adminIdHud->setScaledContents(false);
        adminIdHud->setAlignment(Qt::AlignCenter);
        adminIdTextbox = new QLineEdit(adminIdGroup);
        adminIdTextbox->setObjectName("adminIdTextbox");
        adminIdTextbox->setGeometry(QRect(15, 0, 220, 30));
        QFont font2;
        font2.setFamilies({QString::fromUtf8("Poppins")});
        font2.setWeight(QFont::Medium);
        font2.setItalic(false);
        adminIdTextbox->setFont(font2);
        adminIdTextbox->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"	background-color: transparent;\n"
"	border: 0px;\n"
"	color: #FFFFFF;\n"
"	font-family: Poppins;\n"
"	font-size: 12px;\n"
"	font-style: normal;\n"
"	font-weight: 500;\n"
"	line-height: normal;\n"
"}"));
        adminIdTextbox->setMaxLength(9);
        adminPinGroup = new QGroupBox(formGroup);
        adminPinGroup->setObjectName("adminPinGroup");
        adminPinGroup->setEnabled(true);
        adminPinGroup->setGeometry(QRect(55, 160, 250, 40));
        adminPinGroup->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"	border: 0px;\n"
"	border-radius: 0px;\n"
"	background: none\n"
"}"));
        adminPinGroup->setAlignment(Qt::AlignCenter);
        adminPinGroup->setFlat(true);
        adminPinHud = new QLabel(adminPinGroup);
        adminPinHud->setObjectName("adminPinHud");
        adminPinHud->setGeometry(QRect(0, 0, 250, 40));
        adminPinHud->setStyleSheet(QString::fromUtf8("QLabel {	\n"
"	background: none;\n"
"}"));
        adminPinHud->setPixmap(QPixmap(QString::fromUtf8(":/res/assets/login_field.png")));
        adminPinHud->setScaledContents(false);
        adminPinHud->setAlignment(Qt::AlignCenter);
        adminPinTextbox = new QLineEdit(adminPinGroup);
        adminPinTextbox->setObjectName("adminPinTextbox");
        adminPinTextbox->setGeometry(QRect(15, 0, 220, 30));
        adminPinTextbox->setFont(font2);
        adminPinTextbox->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"	background-color: transparent;\n"
"	border: 0px;\n"
"	color: #FFFFFF;\n"
"	font-family: Poppins;\n"
"	font-size: 12px;\n"
"	font-style: normal;\n"
"	font-weight: 500;\n"
"	line-height: normal;\n"
"}"));
        adminPinTextbox->setMaxLength(6);
        adminPinTextbox->setEchoMode(QLineEdit::Password);
        loginButton = new QPushButton(formGroup);
        loginButton->setObjectName("loginButton");
        loginButton->setGeometry(QRect(75, 330, 210, 40));
        loginButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	border: 0px;\n"
"	border-radius: 0px;\n"
"	background: none;\n"
"}"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/res/assets/login_button.png"), QSize(), QIcon::Normal, QIcon::Off);
        loginButton->setIcon(icon);
        loginButton->setIconSize(QSize(210, 40));
        loginButton->setFlat(true);
        errorLabel = new QLabel(formGroup);
        errorLabel->setObjectName("errorLabel");
        errorLabel->setEnabled(true);
        errorLabel->setGeometry(QRect(25, 310, 310, 16));
        errorLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	background: none;\n"
"	color: #FF003D;\n"
"	font-family: Poppins;\n"
"	font-size: 12px;\n"
"	font-style: normal;\n"
"	font-weight: 600;\n"
"	line-height: normal;\n"
"}"));
        errorLabel->setAlignment(Qt::AlignCenter);
        Admin_Login_Form->setCentralWidget(centralwidget);

        retranslateUi(Admin_Login_Form);

        QMetaObject::connectSlotsByName(Admin_Login_Form);
    } // setupUi

    void retranslateUi(QMainWindow *Admin_Login_Form)
    {
        Admin_Login_Form->setWindowTitle(QCoreApplication::translate("Admin_Login_Form", "Admin Login Form", nullptr));
        appGroup->setTitle(QString());
        presenceIcon->setText(QString());
        formGroup->setTitle(QString());
        background->setText(QString());
        adminIdGroup->setTitle(QString());
        adminIdHud->setText(QString());
        adminIdTextbox->setPlaceholderText(QCoreApplication::translate("Admin_Login_Form", "Admin No.", nullptr));
        adminPinGroup->setTitle(QString());
        adminPinHud->setText(QString());
        adminPinTextbox->setPlaceholderText(QCoreApplication::translate("Admin_Login_Form", "Pin", nullptr));
        loginButton->setText(QString());
        errorLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Admin_Login_Form: public Ui_Admin_Login_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADMIN_LOGIN_FORM_H
