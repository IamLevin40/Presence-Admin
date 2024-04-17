#include "admin_classes_create.h"
#include "ui_admin_classes_create.h"

Admin_Classes_Create::Admin_Classes_Create(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Admin_Classes_Create)
{
    ui->setupUi(this);
}

Admin_Classes_Create::~Admin_Classes_Create()
{
    delete ui;
}
