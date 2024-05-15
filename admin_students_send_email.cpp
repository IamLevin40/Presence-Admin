#include "global_main.h"

#include "admin_students_send_email.h"
#include "ui_admin_students_send_email.h"


// Include header files for forward declaration
#include "admin_students_list.h"
#include "admin_classes_list.h"
#include "admin_lecturers_list.h"


Admin_Students_Send_Email::Admin_Students_Send_Email(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Admin_Students_Send_Email)
{
    ui->setupUi(this);

    // Set up sql online database with connection parameters
    database = QSqlDatabase::addDatabase($sql_Driver);
    database.setHostName($db_Hostname);
    database.setUserName($db_Username);
    database.setPassword($db_Password);
    database.setDatabaseName($db_Database);
    database.setPort($db_Port);

    // Initiate functions on awake
    Admin_Students_Send_Email::selectStudentInfo($updateKey_StudentInfo);
    DateTimeUtils::updateDateTimeUtils(ui->dateLabel, ui->timeLabel);

    // Connect ui objects to functions based on user interaction
    connect(ui->sendButton, &QPushButton::clicked, this, &Admin_Students_Send_Email::sendEmailCall);
    connect(ui->passwordToggle, &QPushButton::clicked, this, [=](){ ToggleManager::toggleEchoMode(ui->passwordTextbox, ui->passwordToggle); });

    connect(ui->backButton, &QPushButton::clicked, this, &Admin_Students_Send_Email::switchWindow_AdminStudentsList);
    connect(ui->studentListButton, &QPushButton::clicked, this, &Admin_Students_Send_Email::switchWindow_AdminStudentsList);
    connect(ui->classListButton, &QPushButton::clicked, this, &Admin_Students_Send_Email::switchWindow_AdminClassesList);
    connect(ui->lecturerListButton, &QPushButton::clicked, this, &Admin_Students_Send_Email::switchWindow_AdminLecturersList);

    // Load window position
    QPoint windowPos = WindowPositionManager::loadWindowPosition();
    windowPos.setY(windowPos.y() - 31);
    move(windowPos);
}


Admin_Students_Send_Email::~Admin_Students_Send_Email()
{
    delete ui;
}


void Admin_Students_Send_Email::selectStudentInfo(const QString &key_studentId)
{
    // Return error if unable to access the database
    if (!database.open())
    {
        GlobalTimer::displayTextForDuration(ui->errorLabel, Messages::unaccessDatabase(), 5000);
        return;
    }

    // Set up data list and queries for database
    QStringList studentDataList;
    QSqlDatabase::database().transaction();
    QSqlQuery query(database);

    // Prepare sql command for selecting data
    query.prepare("SELECT StudentId, Pin, LastName, College FROM StudentInfo \
                  WHERE StudentId = :studentId");

    // Bind values to the query
    query.bindValue(":studentId", key_studentId);

    // Return error if unable to select data
    if (!query.exec())
    {
        QSqlDatabase::database().rollback();
        GlobalTimer::displayTextForDuration(ui->errorLabel, Messages::errorSelectData(), 5000);
        return;
    }

    // Fetch and store data
    query.next();
    studentDataList << query.value(0).toString();   // StudentId
    studentDataList << query.value(1).toString();   // Pin
    studentDataList << query.value(2).toString();   // LastName
    studentDataList << query.value(3).toString();   // College

    // Close the database after using
    QSqlDatabase::database().commit();
    database.close();

    // Proceed to display data list
    Admin_Students_Send_Email::pregenerateEmail(studentDataList);
}


void Admin_Students_Send_Email::pregenerateEmail(const QStringList &dataList)
{
    // Assign members from dataList to variables
    QString studentId = dataList[0];
    QString pin = dataList[1];
    QString lastName = dataList[2];
    QString college = dataList[3];

    ui->subjectTextbox->setText(EmailMsg::preSubjectForStudent);
    ui->bodyTextbox->setText(EmailMsg::preBodyForStudent.arg(lastName, studentId, pin, college));
}


void Admin_Students_Send_Email::sendEmailCall()
{
    QString adminGmail = ui->gmailTextbox->text();
    QString adminPassword = ui->passwordTextbox->text();
    QString recipient = ui->recipientTextbox->text();
    QString subject = ui->subjectTextbox->text();
    QString body = ui->bodyTextbox->toPlainText();

    // Must return result in QString from verify function
    QString result = Admin_Students_Send_Email::verifyEmail(adminGmail, adminPassword, recipient, subject, body);

    // Return error if the result is not empty
    if (!result.isEmpty())
    {
        GlobalTimer::displayTextForDuration(ui->errorLabel, result, 5000);
        return;
    }

    // Proceed to sending email via gmail
    EmailMsg::sendEmailViaGmail(adminGmail, adminPassword, recipient, subject, body, ui->errorLabel);
}


QString Admin_Students_Send_Email::verifyEmail(const QString &adminGmail, const QString &adminPassword,
                                               const QString &recipient, const QString &subject, const QString &body)
{
    if (adminGmail == "") { return Messages::emptyAdminGmail(); }
    if (adminGmail.contains(" ")) { return Messages::invalidAdminGmail(); }
    if (adminPassword == "") { return Messages::emptyAdminPassword(); }
    if (recipient == "") { return Messages::emptyRecipient(); }
    if (recipient.contains(" ")) { return Messages::invalidRecipient(); }
    if (subject == "") { return Messages::emptySubjectEmail(); }
    if (body == "") { return Messages::emptyBodyEmail(); }

    return "";
}


void Admin_Students_Send_Email::switchWindow_AdminStudentsList()
{
    // Switch ui window to Admin_Students_List
    admin_students_list = new Admin_Students_List;
    admin_students_list->show();
    this->hide();
}


void Admin_Students_Send_Email::switchWindow_AdminClassesList()
{
    // Switch ui window to Admin_Classes_List
    admin_classes_list = new Admin_Classes_List;
    admin_classes_list->show();
    this->hide();
}


void Admin_Students_Send_Email::switchWindow_AdminLecturersList()
{
    // Switch ui window to Admin_Lecturers_List
    admin_lecturers_list = new Admin_Lecturers_List;
    admin_lecturers_list->show();
    this->hide();
}


void Admin_Students_Send_Email::moveEvent(QMoveEvent *event)
{
    // Saves window position every movement instance of a window
    QMainWindow::moveEvent(event);
    WindowPositionManager::saveWindowPosition(event->pos());
}


/*
 *   Copyright (c) 2024 - WVSU-CICT BSCS 1B: Batch 2027
 *   All rights reserved.
 */
