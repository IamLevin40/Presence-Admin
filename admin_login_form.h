#ifndef ADMIN_LOGIN_FORM_H
#define ADMIN_LOGIN_FORM_H

#include <QMainWindow>
#include <QMessageBox>

#include "admin_students_list.h"

namespace Ui {
class Admin_Login_Form;
}

class Admin_Login_Form : public QMainWindow
{
    Q_OBJECT

public:
    explicit Admin_Login_Form(QWidget *parent = nullptr);
    ~Admin_Login_Form();

private slots:
    void on_loginButton_clicked();

private:
    Ui::Admin_Login_Form *ui;

    // Reference the class file here
    Admin_Students_List *admin_students_list;

    // All custom functions() should be recognized upon here
    QString authenticateAdmin(const QString &adminId, const QString &adminPin);
    void switchWindow_AdminStudentsList();
};

#endif // ADMIN_LOGIN_FORM_H
