#include "global_main.h"

#include "admin_classes_update.h"
#include "ui_admin_classes_update.h"


// Include header files for forward declaration
#include "admin_students_list.h"
#include "admin_classes_list.h"
#include "admin_lecturers_list.h"


Admin_Classes_Update::Admin_Classes_Update(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Admin_Classes_Update)
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
    Admin_Classes_Update::populateCombobox();
    Admin_Classes_Update::selectClassInfo($updateKeys_ClassInfo);
    DateTimeUtils::updateDateTimeUtils(ui->dateLabel, ui->timeLabel);

    // Connect ui objects to functions based on user interaction
    connect(ui->updateButton, &QPushButton::clicked, this, &Admin_Classes_Update::classUpdateCall);

    connect(ui->backButton, &QPushButton::clicked, this, &Admin_Classes_Update::switchWindow_AdminClassesList);
    connect(ui->studentListButton, &QPushButton::clicked, this, &Admin_Classes_Update::switchWindow_AdminStudentsList);
    connect(ui->classListButton, &QPushButton::clicked, this, &Admin_Classes_Update::switchWindow_AdminClassesList);
    connect(ui->lecturerListButton, &QPushButton::clicked, this, &Admin_Classes_Update::switchWindow_AdminLecturersList);

    // Load window position
    QPoint windowPos = WindowPositionManager::loadWindowPosition();
    windowPos.setY(windowPos.y() - 31);
    move(windowPos);
}


Admin_Classes_Update::~Admin_Classes_Update()
{
    delete ui;
}


void Admin_Classes_Update::selectClassInfo(const QStringList &keys_classInfo)
{
    // Return error if unable to access the database
    if (!database.open())
    {
        GlobalTimer::displayTextForDuration(ui->errorLabel, Messages::unaccessDatabase(), 5000);
        return;
    }

    // Separate keys onto different key variables
    QString key_subjectCode = keys_classInfo[0];
    QString key_program = keys_classInfo[1];
    QString key_year = keys_classInfo[2];
    QString key_section = keys_classInfo[3];
    QString key_semester = keys_classInfo[4];
    QString key_schoolYear = keys_classInfo[5];

    // Set up data list and queries for database
    QStringList classDataList;
    QSqlDatabase::database().transaction();
    QSqlQuery query(database);

    // Prepare sql command for deleting data
    query.prepare("SELECT * FROM ClassInfo WHERE \
                  SubjectCode = :subjectCode AND \
                  Program = :program AND \
                  Year = :year AND \
                  Section = :section AND \
                  Semester = :semester AND \
                  SchoolYear = :schoolYear");

    // Bind values to the query
    query.bindValue(":subjectCode", key_subjectCode);
    query.bindValue(":program", key_program);
    query.bindValue(":year", key_year);
    query.bindValue(":section", key_section);
    query.bindValue(":semester", key_semester);
    query.bindValue(":schoolYear", key_schoolYear);

    // Return error if unable to select data
    if (!query.exec())
    {
        QSqlDatabase::database().rollback();
        GlobalTimer::displayTextForDuration(ui->errorLabel, Messages::errorSelectData(), 5000);
        return;
    }

    // Fetch and store data
    query.next();
    classDataList << query.value(0).toString();     // SubjectCode
    classDataList << query.value(1).toString();     // SubjectDesc
    classDataList << query.value(2).toString();     // SchoolYear
    classDataList << query.value(3).toString();     // Semester
    classDataList << query.value(4).toString();     // Program
    classDataList << query.value(5).toString();     // Year
    classDataList << query.value(6).toString();     // Section
    classDataList << query.value(7).toString();     // Day
    classDataList << query.value(8).toString();     // Time
    classDataList << query.value(9).toString();     // Room
    classDataList << query.value(10).toString();    // LecturerId

    // Close the database after using
    QSqlDatabase::database().commit();
    database.close();

    // Proceed to display data list
    Admin_Classes_Update::updateField(classDataList);
}


void Admin_Classes_Update::updateField(const QStringList &dataList)
{
    // Assign members from dataList to variables
    QString subjectCode = dataList[0];
    QString subjectDesc = dataList[1];
    QString schoolYear = dataList[2];
    QString semester = dataList[3];
    QString program = dataList[4];
    QString year = dataList[5];
    QString section = dataList[6];
    QString combinedDay = dataList[7];
    QString combinedTime = dataList[8];
    QString room = dataList[9];
    QString lecturerId = dataList[10];

    // Split days and times into substrings by the delimiter
    QString firstDay, firstStartTime, firstEndTime;
    QString secondDay, secondStartTime, secondEndTime;
    QString thirdDay, thirdStartTime, thirdEndTime;

    QStringList daysList = combinedDay.split(" | ");
    if (daysList.size() >= 1) { firstDay = daysList[0]; }
    if (daysList.size() >= 2) { secondDay = daysList[1]; }
    if (daysList.size() >= 3) { thirdDay = daysList[2]; }

    QStringList timeRanges = combinedTime.split(" | ");
    for (int i = 0; i < timeRanges.size(); ++i) {
        QStringList times = timeRanges[i].split('-');
        if (times.size() >= 2) {
            switch (i) {
            case 0:
                firstStartTime = times[0];
                firstEndTime = times[1];
                break;
            case 1:
                secondStartTime = times[0];
                secondEndTime = times[1];
                break;
            case 2:
                thirdStartTime = times[0];
                thirdEndTime = times[1];
                break;
            default:
                break;
            }
        }
    }

    // Update the form with the current data
    ui->subjectCodeLabel->setText(StringManipulator::separateSubjectCode(subjectCode));
    ui->subjectDescTextbox->setText(subjectDesc);
    ui->schoolYearLabel->setText(schoolYear);
    ui->semesterLabel->setText(semester);
    ui->programLabel->setText(program);
    ui->yearLabel->setText(year);
    ui->sectionLabel->setText(section);
    ui->roomTextbox->setText(room);
    ui->lecturerIdTextbox->setText(lecturerId);

    FilteringManager::selectOptionByText(ui->firstDayCombobox, firstDay);
    FilteringManager::selectOptionByText(ui->firstStartCombobox, firstStartTime);
    FilteringManager::selectOptionByText(ui->firstEndCombobox, firstEndTime);
    FilteringManager::selectOptionByText(ui->secondDayCombobox, secondDay);
    FilteringManager::selectOptionByText(ui->secondStartCombobox, secondStartTime);
    FilteringManager::selectOptionByText(ui->secondEndCombobox, secondEndTime);
    FilteringManager::selectOptionByText(ui->thirdDayCombobox, thirdDay);
    FilteringManager::selectOptionByText(ui->thirdStartCombobox, thirdStartTime);
    FilteringManager::selectOptionByText(ui->thirdEndCombobox, thirdEndTime);
}


void Admin_Classes_Update::classUpdateCall()
{
    // Fetch string input text() from textboxes and comboboxes
    QString subjectDesc = ui->subjectDescTextbox->text().toUpper();
    QString room = ui->roomTextbox->text().toUpper();
    QString lecturerId = ui->lecturerIdTextbox->text();
    QString firstDay = ui->firstDayCombobox->currentText();
    QString firstStartTime = ui->firstStartCombobox->currentText();
    QString firstEndTime = ui->firstEndCombobox->currentText();
    QString secondDay = ui->secondDayCombobox->currentText();
    QString secondStartTime = ui->secondStartCombobox->currentText();
    QString secondEndTime = ui->secondEndCombobox->currentText();
    QString thirdDay = ui->thirdDayCombobox->currentText();
    QString thirdStartTime = ui->thirdStartCombobox->currentText();
    QString thirdEndTime = ui->thirdEndCombobox->currentText();

    // Must return result in QString from verify function
    QString result = Admin_Classes_Update::verifyClassUpdate(subjectDesc, room, lecturerId,
                                                            firstDay, firstStartTime, firstEndTime,
                                                            secondDay, secondStartTime, secondEndTime,
                                                            thirdDay, thirdStartTime, thirdEndTime);

    // Return error if the result is not empty
    if (!result.isEmpty())
    {
        GlobalTimer::displayTextForDuration(ui->errorLabel, result, 5000);
        return;
    }

    // Proceed to updating data from database
    Admin_Classes_Update::updateClassInfo(subjectDesc, room, lecturerId,
                                                 firstDay, firstStartTime, firstEndTime,
                                                 secondDay, secondStartTime, secondEndTime,
                                                 thirdDay, thirdStartTime, thirdEndTime);
}


QString Admin_Classes_Update::verifyClassUpdate(const QString &subjectDesc, const QString &room, const QString &lecturerId,
                                                const QString &firstDay, const QString &firstStartTime, const QString &firstEndTime,
                                                const QString &secondDay, const QString &secondStartTime, const QString &secondEndTime,
                                                const QString &thirdDay, const QString &thirdStartTime, const QString &thirdEndTime)
{
    if (subjectDesc == "") { return Messages::emptySubjectDesc(); }
    if (room == "") { return Messages::emptyRoom(); }

    if (firstDay == "") { return Messages::noSelectedFirstDay(); }
    if (firstStartTime == "") { return Messages::noSelectedStartTimeFirstDay(); }
    if (firstEndTime == "") { return Messages::noSelectedEndTimeFirstDay(); }

    if (!secondDay.isEmpty())
    {
        if (secondStartTime == "") { return Messages::noSelectedStartTimeSecondDay(); }
        if (secondEndTime == "") { return Messages::noSelectedEndTimeSecondDay(); }
    }

    if (!thirdDay.isEmpty())
    {
        if (thirdStartTime == "") { return Messages::noSelectedStartTimeThirdDay(); }
        if (thirdEndTime == "") { return Messages::noSelectedEndTimeThirdDay(); }
    }

    if (lecturerId == "") { return Messages::emptyLecturerId(); }
    if (lecturerId.length() != $lecturerIdLength) { return Messages::incompleteLengthLecturerId(); }
    if (lecturerId.contains(" ")) { return Messages::invalidLecturerId(); }

    // Return error if unable to access the database
    if (!database.open()) { return Messages::unaccessDatabase(); }

    // Set up queries for database
    QSqlDatabase::database().transaction();
    QSqlQuery query(database);

    // Prepare sql command for finding data
    query.prepare("SELECT COUNT(*) FROM LecturerInfo WHERE LecturerId = :lecturerId");

    // Bind values to the query
    query.bindValue(":lecturerId", lecturerId);

    // Return error if unable to insert data
    if (!query.exec())
    {
        QSqlDatabase::database().rollback();
        return Messages::errorInsertData();
    }

    query.next();
    int count = query.value(0).toInt();
    bool isLecturerIdExist = count > 0;

    // Close the database after using
    QSqlDatabase::database().commit();
    database.close();;

    if (!isLecturerIdExist) { return Messages::notExistLecturerId(); }

    return "";
}


void Admin_Classes_Update::updateClassInfo(const QString &subjectDesc, const QString &room, const QString &lecturerId,
                                           const QString &firstDay, const QString &firstStartTime, const QString &firstEndTime,
                                           const QString &secondDay, const QString &secondStartTime, const QString &secondEndTime,
                                           const QString &thirdDay, const QString &thirdStartTime, const QString &thirdEndTime)
{
    // Return error if unable to access the database
    if (!database.open())
    {
        GlobalTimer::displayTextForDuration(ui->errorLabel, Messages::unaccessDatabase(), 5000);
        return;
    }

    // Combine day and time into one QString
    QString combinedDay = firstDay;
    QString combinedTime = firstStartTime + "-" + firstEndTime;

    if (!secondDay.isEmpty())
    {
        combinedDay += " | " + secondDay;
        combinedTime += " | " + secondStartTime + "-" + secondEndTime;
    }

    if (!thirdDay.isEmpty())
    {
        combinedDay += " | " + thirdDay;
        combinedTime += " | " + thirdStartTime + "-" + thirdEndTime;
    }

    // Separate keys onto different key variables
    QString key_subjectCode = $updateKeys_ClassInfo[0];
    QString key_program = $updateKeys_ClassInfo[1];
    QString key_year = $updateKeys_ClassInfo[2];
    QString key_section = $updateKeys_ClassInfo[3];
    QString key_semester = $updateKeys_ClassInfo[4];
    QString key_schoolYear = $updateKeys_ClassInfo[5];

    // Set up queries for database
    QSqlDatabase::database().transaction();
    QSqlQuery query(database);

    // Prepare sql command for updating data
    query.prepare("UPDATE ClassInfo SET \
                  SubjectDesc = :subjectDesc, Room = :room, LecturerId = :lecturerId, \
                  Day = :day, Time = :time \
                  WHERE SubjectCode = :subjectCode AND Program = :program AND Year = :year AND \
                  Section = :section AND Semester = :semester AND SchoolYear = :schoolYear");

    // Bind values to the query
    query.bindValue(":subjectDesc", subjectDesc);
    query.bindValue(":room", room);
    query.bindValue(":lecturerId", lecturerId);
    query.bindValue(":day", combinedDay);
    query.bindValue(":time", combinedTime);
    query.bindValue(":subjectCode", key_subjectCode);
    query.bindValue(":program", key_program);
    query.bindValue(":year", key_year);
    query.bindValue(":section", key_section);
    query.bindValue(":semester", key_semester);
    query.bindValue(":schoolYear", key_schoolYear);

    // Return error if unable to update data
    if (!query.exec())
    {
        qDebug() << query.lastError().text();
        QSqlDatabase::database().rollback();
        GlobalTimer::displayTextForDuration(ui->errorLabel, Messages::errorUpdateData(), 5000);
        return;
    }

    // Close the database after using
    QSqlDatabase::database().commit();
    database.close();

    Admin_Classes_Update::switchWindow_AdminClassesList();
}


void Admin_Classes_Update::populateCombobox()
{
    // Populate comboboxes with items
    ui->firstDayCombobox->addItems(Schedules::$dayOptions);
    ui->firstStartCombobox->addItems(Schedules::$timeOptions);
    ui->firstEndCombobox->addItems(Schedules::$timeOptions);
    ui->secondDayCombobox->addItems(Schedules::$dayOptions);
    ui->secondStartCombobox->addItems(Schedules::$timeOptions);
    ui->secondEndCombobox->addItems(Schedules::$timeOptions);
    ui->thirdDayCombobox->addItems(Schedules::$dayOptions);
    ui->thirdStartCombobox->addItems(Schedules::$timeOptions);
    ui->thirdEndCombobox->addItems(Schedules::$timeOptions);

    // Connect two comboboxes for setting up dependency; disable combobox if the previous one has not been selected
    QVector<QComboBox *> comboBoxes {
        ui->firstDayCombobox, ui->firstStartCombobox, ui->firstEndCombobox,
        ui->secondDayCombobox, ui->secondStartCombobox, ui->secondEndCombobox,
        ui->thirdDayCombobox, ui->thirdStartCombobox, ui->thirdEndCombobox
    };
    FilteringManager::setupComboboxDependency(comboBoxes);

    // Connect comboboxes to checkComboboxIndex()
    connect(ui->firstDayCombobox, &QComboBox::currentIndexChanged, this, [=](){ FilteringManager::checkComboboxIndex(ui->firstDayCombobox); });
    connect(ui->secondDayCombobox, &QComboBox::currentIndexChanged, this, [=](){ FilteringManager::checkComboboxIndex(ui->secondDayCombobox); });
    connect(ui->thirdDayCombobox, &QComboBox::currentIndexChanged, this, [=](){ FilteringManager::checkComboboxIndex(ui->thirdDayCombobox); });
}


void Admin_Classes_Update::switchWindow_AdminStudentsList()
{
    // Switch ui window to Admin_Students_List
    admin_students_list = new Admin_Students_List;
    admin_students_list->show();
    this->hide();
}


void Admin_Classes_Update::switchWindow_AdminClassesList()
{
    // Switch ui window to Admin_Classes_List
    admin_classes_list = new Admin_Classes_List;
    admin_classes_list->show();
    this->hide();
}


void Admin_Classes_Update::switchWindow_AdminLecturersList()
{
    // Switch ui window to Admin_Lecturers_List
    admin_lecturers_list = new Admin_Lecturers_List;
    admin_lecturers_list->show();
    this->hide();
}


void Admin_Classes_Update::moveEvent(QMoveEvent *event)
{
    // Saves window position every movement instance of a window
    QMainWindow::moveEvent(event);
    WindowPositionManager::saveWindowPosition(event->pos());
}


/*
 *   Copyright (c) 2024 - WVSU-CICT BSCS 1B: Batch 2027
 *   All rights reserved.
 */
