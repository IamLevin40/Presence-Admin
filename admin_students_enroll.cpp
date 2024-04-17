#include "admin_students_enroll.h"
#include "ui_admin_students_enroll.h"

Admin_Students_Enroll::Admin_Students_Enroll(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Admin_Students_Enroll)
{
    ui->setupUi(this);
}

Admin_Students_Enroll::~Admin_Students_Enroll()
{
    delete ui;
}
