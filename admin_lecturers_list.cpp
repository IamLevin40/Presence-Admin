#include "admin_lecturers_list.h"
#include "ui_admin_lecturers_list.h"

Admin_Lecturers_List::Admin_Lecturers_List(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Admin_Lecturers_List)
{
    ui->setupUi(this);
}

Admin_Lecturers_List::~Admin_Lecturers_List()
{
    delete ui;
}
