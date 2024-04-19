#include "global_main.h"

#include "admin_classes_list.h"
#include "ui_admin_classes_list.h"


// Include header files for forward declaration
#include "admin_login_form.h"
#include "admin_students_list.h"
#include "admin_lecturers_list.h"
#include "admin_classes_create.h"


Admin_Classes_List::Admin_Classes_List(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Admin_Classes_List)
{
    ui->setupUi(this);

    // Give function to buttons
    connect(ui->backButton, &QPushButton::clicked, this, &Admin_Classes_List::switchWindow_AdminLoginForm);
    connect(ui->studentListButton, &QPushButton::clicked, this, &Admin_Classes_List::switchWindow_AdminStudentsList);
    connect(ui->lecturerListButton, &QPushButton::clicked, this, &Admin_Classes_List::switchWindow_AdminLecturersList);
    connect(ui->createClassButton, &QPushButton::clicked, this, &Admin_Classes_List::switchWindow_AdminClassesCreate);

    // Load window position
    QPoint windowPos = WindowPositionManager::loadWindowPosition();
    windowPos.setY(windowPos.y() - 31);
    move(windowPos);
}


Admin_Classes_List::~Admin_Classes_List()
{
    delete ui;
}


void Admin_Classes_List::switchWindow_AdminLoginForm()
{
    // Switch ui window to Admin_Login_Form
    admin_login_form = new Admin_Login_Form;
    admin_login_form->show();
    this->hide();
}


void Admin_Classes_List::switchWindow_AdminStudentsList()
{
    // Switch ui window to Admin_Students_List
    admin_students_list = new Admin_Students_List;
    admin_students_list->show();
    this->hide();
}


void Admin_Classes_List::switchWindow_AdminLecturersList()
{
    // Switch ui window to Admin_Lecturers_List
    admin_lecturers_list = new Admin_Lecturers_List;
    admin_lecturers_list->show();
    this->hide();
}


void Admin_Classes_List::switchWindow_AdminClassesCreate()
{
    // Switch ui window to Admin_Classes_Create
    admin_classes_create = new Admin_Classes_Create;
    admin_classes_create->show();
    this->hide();
}


void Admin_Classes_List::moveEvent(QMoveEvent *event)
{
    // Saves window position every movement instance of a window
    QMainWindow::moveEvent(event);
    WindowPositionManager::saveWindowPosition(event->pos());
}
