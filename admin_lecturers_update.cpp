#include "global_main.h"

#include "admin_lecturers_update.h"
#include "ui_admin_lecturers_update.h"


// Include header files for forward declaration
#include "admin_students_list.h"
#include "admin_classes_list.h"
#include "admin_lecturers_list.h"


Admin_Lecturers_Update::Admin_Lecturers_Update(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Admin_Lecturers_Update)
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
    Admin_Lecturers_Update::populateCombobox();
    Admin_Lecturers_Update::selectLecturerInfo($updateKey_LecturerInfo);
    DateTimeUtils::updateDateTimeUtils(ui->dateLabel, ui->timeLabel);

    // Connect ui objects to functions based on user interaction
    connect(ui->updateButton, &QPushButton::clicked, this, &Admin_Lecturers_Update::lecturerUpdateCall);

    connect(ui->backButton, &QPushButton::clicked, this, &Admin_Lecturers_Update::switchWindow_AdminLecturersList);
    connect(ui->studentListButton, &QPushButton::clicked, this, &Admin_Lecturers_Update::switchWindow_AdminStudentsList);
    connect(ui->classListButton, &QPushButton::clicked, this, &Admin_Lecturers_Update::switchWindow_AdminClassesList);
    connect(ui->lecturerListButton, &QPushButton::clicked, this, &Admin_Lecturers_Update::switchWindow_AdminLecturersList);

    // Load window position
    QPoint windowPos = WindowPositionManager::loadWindowPosition();
    windowPos.setY(windowPos.y() - 31);
    move(windowPos);
}


Admin_Lecturers_Update::~Admin_Lecturers_Update()
{
    delete ui;
}


void Admin_Lecturers_Update::selectLecturerInfo(const QString &key_lecturerId)
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
    query.prepare("SELECT * FROM LecturerInfo WHERE LecturerId = :lecturerId");

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
    lecturerDataList << query.value(3).toString();   // FirstName
    lecturerDataList << query.value(4).toString();   // College

    // Close the database after using
    QSqlDatabase::database().commit();
    database.close();

    // Proceed to display data list
    Admin_Lecturers_Update::updateField(lecturerDataList);
}


void Admin_Lecturers_Update::updateField(const QStringList &dataList)
{
    // Assign members from dataList to variables
    QString lecturerId = dataList[0];
    QString pin = dataList[1];
    QString lastName = dataList[2];
    QString firstName = dataList[3];
    QString college = dataList[4];

    // Update the form with the current data
    ui->lecturerIdLabel->setText(lecturerId);
    ui->lastNameTextbox->setText(lastName);
    ui->firstNameTextbox->setText(firstName);
    ui->pinTextbox->setText(pin);

    FilteringManager::selectOptionByText(ui->collegeCombobox, college);
}


void Admin_Lecturers_Update::lecturerUpdateCall()
{
    // Fetch string input text() from textboxes and comboboxes
    QString lastName = ui->lastNameTextbox->text().toUpper();
    QString firstName = ui->firstNameTextbox->text().toUpper();
    QString college = ui->collegeCombobox->currentText();
    QString pin = ui->pinTextbox->text();

    // Extract unnecessary substring for data handling
    int collegeIndex = college.indexOf('|');
    college = college.left(collegeIndex).trimmed();

    // Must return result in QString from verify function
    QString result = Admin_Lecturers_Update::verifyLecturerUpdate(lastName, firstName, college, pin);

    // Return error if the result is not empty
    if (!result.isEmpty())
    {
        GlobalTimer::displayTextForDuration(ui->errorLabel, result, 5000);
        return;
    }

    // Proceed to updating data from database
    Admin_Lecturers_Update::updateLecturerInfo(lastName, firstName, college, pin);
}


QString Admin_Lecturers_Update::verifyLecturerUpdate(const QString &lastName, const QString &firstName, const QString &college, const QString &pin)
{
    if (lastName == "") { return Messages::emptyLastName(); }
    if (firstName == "") { return Messages::emptyFirstName(); }
    if (college == "") { return Messages::noSelectedCollege(); }

    if (pin == "") { return Messages::emptyPin(); }
    if (pin.length() != $pinLength) { return Messages::incompleteLengthPin(); }
    if (pin.contains(" ")) { return Messages::invalidPin(); }

    bool isNum;
    int _pin = pin.toInt(&isNum);
    if (!isNum) { return _pin + "" + Messages::invalidPin(); }

    return "";
}


void Admin_Lecturers_Update::updateLecturerInfo(const QString &lastName, const QString &firstName, const QString &college, const QString &pin)
{
    // Return error if unable to access the database
    if (!database.open())
    {
        GlobalTimer::displayTextForDuration(ui->errorLabel, Messages::unaccessDatabase(), 5000);
        return;
    }

    // Set up queries for database
    QSqlDatabase::database().transaction();
    QSqlQuery query(database);

    // Prepare sql command for updating data
    query.prepare("UPDATE LecturerInfo SET \
                  Pin = :pin, LastName = :lastName, FirstName = :firstName, College = :college \
                  WHERE LecturerId = :lecturerId");

    // Bind values to the query
    query.bindValue(":pin", pin);
    query.bindValue(":lastName", lastName);
    query.bindValue(":firstName", firstName);
    query.bindValue(":college", college);
    query.bindValue(":lecturerId", $updateKey_LecturerInfo);

    // Return error if unable to update data
    if (!query.exec())
    {
        QSqlDatabase::database().rollback();
        GlobalTimer::displayTextForDuration(ui->errorLabel, Messages::errorUpdateData(), 5000);
        return;
    }

    // Close the database after using
    QSqlDatabase::database().commit();
    database.close();

    Admin_Lecturers_Update::switchWindow_AdminLecturersList();
}


void Admin_Lecturers_Update::populateCombobox()
{
    // Populate comboboxes with items
    ui->collegeCombobox->addItems(Colleges::$colleges);

    // Connect comboboxes to checkComboboxIndex()
    connect(ui->collegeCombobox, &QComboBox::currentIndexChanged, this, [=](){ FilteringManager::checkComboboxIndex(ui->collegeCombobox); });
}


void Admin_Lecturers_Update::switchWindow_AdminStudentsList()
{
    // Switch ui window to Admin_Students_List
    admin_students_list = new Admin_Students_List;
    admin_students_list->show();
    this->hide();
}


void Admin_Lecturers_Update::switchWindow_AdminClassesList()
{
    // Switch ui window to Admin_Classes_List
    admin_classes_list = new Admin_Classes_List;
    admin_classes_list->show();
    this->hide();
}


void Admin_Lecturers_Update::switchWindow_AdminLecturersList()
{
    // Switch ui window to Admin_Lecturers_List
    admin_lecturers_list = new Admin_Lecturers_List;
    admin_lecturers_list->show();
    this->hide();
}


void Admin_Lecturers_Update::moveEvent(QMoveEvent *event)
{
    // Saves window position every movement instance of a window
    QMainWindow::moveEvent(event);
    WindowPositionManager::saveWindowPosition(event->pos());
}


/*
 *   Copyright (c) 2024 - WVSU-CICT BSCS 1B: Batch 2027
 *   All rights reserved.
 */
