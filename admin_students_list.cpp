#include "admin_students_list.h"
#include "ui_admin_students_list.h"

Admin_Students_List::Admin_Students_List(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Admin_Students_List)
{
    ui->setupUi(this);
}

Admin_Students_List::~Admin_Students_List()
{
    delete ui;
}
