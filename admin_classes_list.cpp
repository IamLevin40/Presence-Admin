#include "admin_classes_list.h"
#include "ui_admin_classes_list.h"

Admin_Classes_List::Admin_Classes_List(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Admin_Classes_List)
{
    ui->setupUi(this);
}

Admin_Classes_List::~Admin_Classes_List()
{
    delete ui;
}
