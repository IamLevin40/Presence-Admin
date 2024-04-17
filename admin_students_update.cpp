#include "admin_students_update.h"
#include "ui_admin_students_update.h"

Admin_Students_Update::Admin_Students_Update(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Admin_Students_Update)
{
    ui->setupUi(this);
}

Admin_Students_Update::~Admin_Students_Update()
{
    delete ui;
}
