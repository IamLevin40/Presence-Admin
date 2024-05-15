#include "global_main.h"

#include "admin_lecturers_send_email.h"
#include "ui_admin_lecturers_send_email.h"

// Include header files for forward declaration
#include "admin_students_list.h"
#include "admin_classes_list.h"
#include "admin_lecturers_list.h"


Admin_Lecturers_Send_Email::Admin_Lecturers_Send_Email(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Admin_Lecturers_Send_Email)
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
    Admin_Lecturers_Send_Email::selectLecturerInfo($updateKey_LecturerInfo);
    DateTimeUtils::updateDateTimeUtils(ui->dateLabel, ui->timeLabel);

    // Connect ui objects to functions based on user interaction
    connect(ui->sendButton, &QPushButton::clicked, this, &Admin_Lecturers_Send_Email::sendEmailCall);
    connect(ui->passwordToggle, &QPushButton::clicked, this, [=](){ ToggleManager::toggleEchoMode(ui->passwordTextbox, ui->passwordToggle); });

    connect(ui->backButton, &QPushButton::clicked, this, &Admin_Lecturers_Send_Email::switchWindow_AdminLecturersList);
    connect(ui->studentListButton, &QPushButton::clicked, this, &Admin_Lecturers_Send_Email::switchWindow_AdminStudentsList);
    connect(ui->classListButton, &QPushButton::clicked, this, &Admin_Lecturers_Send_Email::switchWindow_AdminClassesList);
    connect(ui->lecturerListButton, &QPushButton::clicked, this, &Admin_Lecturers_Send_Email::switchWindow_AdminLecturersList);

    // Load window position
    QPoint windowPos = WindowPositionManager::loadWindowPosition();
    windowPos.setY(windowPos.y() - 31);
    move(windowPos);
}


Admin_Lecturers_Send_Email::~Admin_Lecturers_Send_Email()
{
    delete ui;
}


void Admin_Lecturers_Send_Email::selectLecturerInfo(const QString &key_lecturerId)
{
    // Return error if unable to access the database
    if (!database.open())
    {
        GlobalTimer::displayTextForDuration(ui->errorLabel, Messages::unaccessDatabase(), 5000);
        return;
    }

    // Set up data list and queries for database
    QStringList lecturerDataList;
    QSqlDatabase::database().transaction();
    QSqlQuery query(database);

    // Prepare sql command for selecting data
    query.prepare("SELECT LecturerId, Pin, LastName, College FROM LecturerInfo \
                  WHERE LecturerId = :lecturerId");

    // Bind values to the query
    query.bindValue(":lecturerId", key_lecturerId);

    // Return error if unable to select data
    if (!query.exec())
    {
        QSqlDatabase::database().rollback();
        GlobalTimer::displayTextForDuration(ui->errorLabel, Messages::errorSelectData(), 5000);
        return;
    }

    // Fetch and store data
    query.next();
    lecturerDataList << query.value(0).toString();   // LecturerId
    lecturerDataList << query.value(1).toString();   // Pin
    lecturerDataList << query.value(2).toString();   // LastName
    lecturerDataList << query.value(3).toString();   // College

    // Close the database after using
    QSqlDatabase::database().commit();
    database.close();

    // Proceed to display data list
    Admin_Lecturers_Send_Email::pregenerateEmail(lecturerDataList);
}


void Admin_Lecturers_Send_Email::pregenerateEmail(const QStringList &dataList)
{
    // Assign members from dataList to variables
    QString lecturerId = dataList[0];
    QString pin = dataList[1];
    QString lastName = dataList[2];
    QString college = dataList[3];

    ui->subjectTextbox->setText(EmailMsg::preSubjectForLecturer);
    ui->bodyTextbox->setText(EmailMsg::preBodyForLecturer.arg(lastName, lecturerId, pin, college));
}


void Admin_Lecturers_Send_Email::sendEmailCall()
{
    QString adminGmail = ui->gmailTextbox->text();
    QString adminPassword = ui->passwordTextbox->text();
    QString recipient = ui->recipientTextbox->text();
    QString subject = ui->subjectTextbox->text();
    QString body = ui->bodyTextbox->toPlainText();

    // Must return result in QString from verify function
    QString result = Admin_Lecturers_Send_Email::verifyEmail(adminGmail, adminPassword, recipient, subject, body);

    // Return error if the result is not empty
    if (!result.isEmpty())
    {
        GlobalTimer::displayTextForDuration(ui->errorLabel, result, 5000);
        return;
    }

    // Proceed to sending email via gmail
    EmailMsg::sendEmailViaGmail(adminGmail, adminPassword, recipient, subject, body, ui->errorLabel);
}


QString Admin_Lecturers_Send_Email::verifyEmail(const QString &adminGmail, const QString &adminPassword,
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


void Admin_Lecturers_Send_Email::switchWindow_AdminStudentsList()
{
    // Switch ui window to Admin_Students_List
    admin_students_list = new Admin_Students_List;
    admin_students_list->show();
    this->hide();
}


void Admin_Lecturers_Send_Email::switchWindow_AdminClassesList()
{
    // Switch ui window to Admin_Classes_List
    admin_classes_list = new Admin_Classes_List;
    admin_classes_list->show();
    this->hide();
}


void Admin_Lecturers_Send_Email::switchWindow_AdminLecturersList()
{
    // Switch ui window to Admin_Lecturers_List
    admin_lecturers_list = new Admin_Lecturers_List;
    admin_lecturers_list->show();
    this->hide();
}


void Admin_Lecturers_Send_Email::moveEvent(QMoveEvent *event)
{
    // Saves window position every movement instance of a window
    QMainWindow::moveEvent(event);
    WindowPositionManager::saveWindowPosition(event->pos());
}


/*
 *   Copyright (c) 2024 - WVSU-CICT BSCS 1B: Batch 2027
 *   All rights reserved.
 */
