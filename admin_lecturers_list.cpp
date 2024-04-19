#include "global_main.h"

#include "admin_lecturers_list.h"
#include "ui_admin_lecturers_list.h"


// Include header files for forward declaration
#include "admin_login_form.h"
#include "admin_students_list.h"
#include "admin_classes_list.h"
#include "admin_lecturers_add.h"


Admin_Lecturers_List::Admin_Lecturers_List(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Admin_Lecturers_List)
{
    ui->setupUi(this);

    // Give function to buttons
    connect(ui->backButton, &QPushButton::clicked, this, &Admin_Lecturers_List::switchWindow_AdminLoginForm);
    connect(ui->studentListButton, &QPushButton::clicked, this, &Admin_Lecturers_List::switchWindow_AdminStudentsList);
    connect(ui->classListButton, &QPushButton::clicked, this, &Admin_Lecturers_List::switchWindow_AdminClassesList);
    connect(ui->addLecturerButton, &QPushButton::clicked, this, &Admin_Lecturers_List::switchWindow_AdminLecturersAdd);

    // Load window position
    QPoint windowPos = WindowPositionManager::loadWindowPosition();
    windowPos.setY(windowPos.y() - 31);
    move(windowPos);
}


Admin_Lecturers_List::~Admin_Lecturers_List()
{
    delete ui;
}


void Admin_Lecturers_List::switchWindow_AdminLoginForm()
{
    // Switch ui window to Admin_Login_Form
    admin_login_form = new Admin_Login_Form;
    admin_login_form->show();
    this->hide();
}


void Admin_Lecturers_List::switchWindow_AdminStudentsList()
{
    // Switch ui window to Admin_Students_List
    admin_students_list = new Admin_Students_List;
    admin_students_list->show();
    this->hide();
}


void Admin_Lecturers_List::switchWindow_AdminClassesList()
{
    // Switch ui window to Admin_Classes_List
    admin_classes_list = new Admin_Classes_List;
    admin_classes_list->show();
    this->hide();
}


void Admin_Lecturers_List::switchWindow_AdminLecturersAdd()
{
    // Switch ui window to Admin_Lecturers_Add
    admin_lecturers_add = new Admin_Lecturers_Add;
    admin_lecturers_add->show();
    this->hide();
}


void Admin_Lecturers_List::moveEvent(QMoveEvent *event)
{
    // Saves window position every movement instance of a window
    QMainWindow::moveEvent(event);
    WindowPositionManager::saveWindowPosition(event->pos());
}
