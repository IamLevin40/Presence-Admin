#include "admin_classes_students_add.h"
#include "ui_admin_classes_students_add.h"

Admin_Classes_Students_Add::Admin_Classes_Students_Add(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Admin_Classes_Students_Add)
{
    ui->setupUi(this);
}

Admin_Classes_Students_Add::~Admin_Classes_Students_Add()
{
    delete ui;
}
