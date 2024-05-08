#include "global_main.h"

#include "admin_login_form.h"
#include "ui_admin_login_form.h"


// Include header files for forward declaration
#include "admin_students_list.h"


Admin_Login_Form::Admin_Login_Form(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Admin_Login_Form)
{
    ui->setupUi(this);

    // Connect ui objects to functions based on user interaction
    connect(ui->loginButton, &QPushButton::clicked, this, &Admin_Login_Form::loginCall);
    connect(ui->adminPinToggle, &QPushButton::clicked, this, [=](){ ToggleManager::toggleEchoMode(ui->adminPinTextbox, ui->adminPinToggle); });

    // Load window position
    QPoint windowPos = WindowPositionManager::loadWindowPosition();
    windowPos.setY(windowPos.y() - 31);
    move(windowPos);
}


Admin_Login_Form::~Admin_Login_Form()
{
    delete ui;
}


void Admin_Login_Form::loginCall()
{
    // Fetch string input text() from textboxes
    QString adminId = ui->adminIdTextbox->text();
    QString adminPin = ui->adminPinTextbox->text();

    // Must return result in QString from authentication function
    QString result = Admin_Login_Form::authenticateAdmin(adminId, adminPin);

    // Return error if the result is not empty
    if (!result.isEmpty())
    {
        GlobalTimer::displayTextForDuration(ui->errorLabel, result, 5000);
        return;
    }

    Admin_Login_Form::switchWindow_AdminStudentsList();
}


QString Admin_Login_Form::authenticateAdmin(const QString &adminId, const QString &adminPin)
{
    // Return message if any conditions met true, otherwise return empty string
    if (adminId == "") { return Messages::emptyAdminId(); }
    if (adminId != $adminId) { return Messages::invalidAdminId(); }
    if (adminPin == "") { return Messages::emptyAdminPin(); }
    if (adminPin != $adminPin) { return Messages::invalidAdminPin(); }
    return "";
}


void Admin_Login_Form::switchWindow_AdminStudentsList()
{
    // Switch ui window to Admin_Students_List
    admin_students_list = new Admin_Students_List;
    admin_students_list->show();
    this->hide();
}


void Admin_Login_Form::moveEvent(QMoveEvent *event)
{
    // Saves window position every movement instance of a window
    QMainWindow::moveEvent(event);
    WindowPositionManager::saveWindowPosition(event->pos());
}
