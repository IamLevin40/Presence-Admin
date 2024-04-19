#include "global_main.h"

#include "admin_students_list.h"
#include "ui_admin_students_list.h"


// Include header files for forward declaration
#include "admin_login_form.h"
#include "admin_classes_list.h"
#include "admin_lecturers_list.h"
#include "admin_students_enroll.h"


Admin_Students_List::Admin_Students_List(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Admin_Students_List)
{
    ui->setupUi(this);

    // Give function to buttons
    connect(ui->backButton, &QPushButton::clicked, this, &Admin_Students_List::switchWindow_AdminLoginForm);
    connect(ui->classListButton, &QPushButton::clicked, this, &Admin_Students_List::switchWindow_AdminClassesList);
    connect(ui->lecturerListButton, &QPushButton::clicked, this, &Admin_Students_List::switchWindow_AdminLecturersList);
    connect(ui->enrollStudentButton, &QPushButton::clicked, this, &Admin_Students_List::switchWindow_AdminStudentsEnroll);

    // Load window position
    QPoint windowPos = WindowPositionManager::loadWindowPosition();
    windowPos.setY(windowPos.y() - 31);
    move(windowPos);
}


Admin_Students_List::~Admin_Students_List()
{
    delete ui;
}


void Admin_Students_List::switchWindow_AdminLoginForm()
{
    // Switch ui window to Admin_Login_Form
    admin_login_form = new Admin_Login_Form;
    admin_login_form->show();
    this->hide();
}


void Admin_Students_List::switchWindow_AdminClassesList()
{
    // Switch ui window to Admin_Classes_List
    admin_classes_list = new Admin_Classes_List;
    admin_classes_list->show();
    this->hide();
}


void Admin_Students_List::switchWindow_AdminLecturersList()
{
    // Switch ui window to Admin_Lecturers_List
    admin_lecturers_list = new Admin_Lecturers_List;
    admin_lecturers_list->show();
    this->hide();
}


void Admin_Students_List::switchWindow_AdminStudentsEnroll()
{
    // Switch ui window to Admin_Students_Enroll
    admin_students_enroll = new Admin_Students_Enroll;
    admin_students_enroll->show();
    this->hide();
}


void Admin_Students_List::moveEvent(QMoveEvent *event)
{
    // Saves window position every movement instance of a window
    QMainWindow::moveEvent(event);
    WindowPositionManager::saveWindowPosition(event->pos());
}
