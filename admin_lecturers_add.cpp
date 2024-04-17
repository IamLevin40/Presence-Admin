#include "admin_lecturers_add.h"
#include "ui_admin_lecturers_add.h"

Admin_Lecturers_Add::Admin_Lecturers_Add(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Admin_Lecturers_Add)
{
    ui->setupUi(this);
}

Admin_Lecturers_Add::~Admin_Lecturers_Add()
{
    delete ui;
}
