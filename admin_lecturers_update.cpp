#include "admin_lecturers_update.h"
#include "ui_admin_lecturers_update.h"

Admin_Lecturers_Update::Admin_Lecturers_Update(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Admin_Lecturers_Update)
{
    ui->setupUi(this);
}

Admin_Lecturers_Update::~Admin_Lecturers_Update()
{
    delete ui;
}
