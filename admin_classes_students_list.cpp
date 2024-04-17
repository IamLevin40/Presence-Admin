#include "admin_classes_students_list.h"
#include "ui_admin_classes_students_list.h"

Admin_Classes_Students_List::Admin_Classes_Students_List(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Admin_Classes_Students_List)
{
    ui->setupUi(this);
}

Admin_Classes_Students_List::~Admin_Classes_Students_List()
{
    delete ui;
}
