#include "admin_classes_update.h"
#include "ui_admin_classes_update.h"

Admin_Classes_Update::Admin_Classes_Update(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Admin_Classes_Update)
{
    ui->setupUi(this);
}

Admin_Classes_Update::~Admin_Classes_Update()
{
    delete ui;
}
