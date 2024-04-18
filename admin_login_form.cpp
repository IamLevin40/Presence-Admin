#include "global_variables.h"

#include "admin_login_form.h"
#include "ui_admin_login_form.h"

Admin_Login_Form::Admin_Login_Form(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Admin_Login_Form)
{
    ui->setupUi(this);
}

Admin_Login_Form::~Admin_Login_Form()
{
    delete ui;
}

void Admin_Login_Form::on_loginButton_clicked()
{
    // Fetch string input text() from textboxes
    QString adminId = ui->adminIdTextbox->text();
    QString adminPin = ui->adminPinTextbox->text();

    // Must return result in QString from authentication function
    QString result = authenticateAdmin(adminId, adminPin);

    // Indicate successful if the result is empty
    if (result.isEmpty())
    {
        switchWindow_AdminStudentsList();
    }
    // Otherwise, display error message
    else
    {
        ui->errorLabel->setText(result);
    }
}

QString Admin_Login_Form::authenticateAdmin(const QString &adminId, const QString &adminPin)
{
    // Return message if any conditions met true, otherwise return empty string
    if (adminId == "")
    {
        return Messages::emptyAdminId();
    }
    if (adminId != $adminId)
    {
        return Messages::invalidAdminId();
    }
    if (adminPin == "")
    {
        return Messages::emptyAdminPin();
    }
    if (adminPin != $adminPin)
    {
        return Messages::invalidAdminPin();
    }
    return "";
}

void Admin_Login_Form::switchWindow_AdminStudentsList()
{
    // Switch ui window to Admin_Students_List
    admin_students_list = new Admin_Students_List;
    admin_students_list->show();
    this->hide();
}
