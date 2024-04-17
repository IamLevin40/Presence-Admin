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
