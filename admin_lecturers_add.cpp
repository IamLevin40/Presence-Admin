#include "global_main.h"

#include "admin_lecturers_add.h"
#include "ui_admin_lecturers_add.h"


// Include header files for forward declaration
#include "admin_students_list.h"
#include "admin_classes_list.h"
#include "admin_lecturers_list.h"


Admin_Lecturers_Add::Admin_Lecturers_Add(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Admin_Lecturers_Add)
{
    ui->setupUi(this);

    // Connect ui objects to functions based on user interaction
    connect(ui->backButton, &QPushButton::clicked, this, &Admin_Lecturers_Add::switchWindow_AdminLecturersList);
    connect(ui->studentListButton, &QPushButton::clicked, this, &Admin_Lecturers_Add::switchWindow_AdminStudentsList);
    connect(ui->classListButton, &QPushButton::clicked, this, &Admin_Lecturers_Add::switchWindow_AdminClassesList);
    connect(ui->lecturerListButton, &QPushButton::clicked, this, &Admin_Lecturers_Add::switchWindow_AdminLecturersList);

    // Load window position
    QPoint windowPos = WindowPositionManager::loadWindowPosition();
    windowPos.setY(windowPos.y() - 31);
    move(windowPos);
}


Admin_Lecturers_Add::~Admin_Lecturers_Add()
{
    delete ui;
}


void Admin_Lecturers_Add::switchWindow_AdminStudentsList()
{
    // Switch ui window to Admin_Students_List
    admin_students_list = new Admin_Students_List;
    admin_students_list->show();
    this->hide();
}


void Admin_Lecturers_Add::switchWindow_AdminClassesList()
{
    // Switch ui window to Admin_Classes_List
    admin_classes_list = new Admin_Classes_List;
    admin_classes_list->show();
    this->hide();
}


void Admin_Lecturers_Add::switchWindow_AdminLecturersList()
{
    // Switch ui window to Admin_Lecturers_List
    admin_lecturers_list = new Admin_Lecturers_List;
    admin_lecturers_list->show();
    this->hide();
}


void Admin_Lecturers_Add::moveEvent(QMoveEvent *event)
{
    // Saves window position every movement instance of a window
    QMainWindow::moveEvent(event);
    WindowPositionManager::saveWindowPosition(event->pos());
}
