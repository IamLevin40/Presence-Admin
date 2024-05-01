#include "global_main.h"

#include "admin_classes_create.h"
#include "ui_admin_classes_create.h"


// Include header files for forward declaration
#include "admin_students_list.h"
#include "admin_classes_list.h"
#include "admin_lecturers_list.h"


Admin_Classes_Create::Admin_Classes_Create(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Admin_Classes_Create)
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
    Admin_Classes_Create::populateCombobox();
    DateTimeUtils::updateDateTimeUtils(ui->dateLabel, ui->timeLabel);

    // Connect ui objects to functions based on user interaction
    connect(ui->confirmButton, &QPushButton::clicked, this, &Admin_Classes_Create::classCreateCall);

    connect(ui->backButton, &QPushButton::clicked, this, &Admin_Classes_Create::switchWindow_AdminClassesList);
    connect(ui->studentListButton, &QPushButton::clicked, this, &Admin_Classes_Create::switchWindow_AdminStudentsList);
    connect(ui->classListButton, &QPushButton::clicked, this, &Admin_Classes_Create::switchWindow_AdminClassesList);
    connect(ui->lecturerListButton, &QPushButton::clicked, this, &Admin_Classes_Create::switchWindow_AdminLecturersList);

    // Load window position
    QPoint windowPos = WindowPositionManager::loadWindowPosition();
    windowPos.setY(windowPos.y() - 31);
    move(windowPos);
}


Admin_Classes_Create::~Admin_Classes_Create()
{
    delete ui;
}


void Admin_Classes_Create::classCreateCall()
{
    // Fetch string input text() from textboxes and comboboxes
    QString subjectCode = ui->subjectCodeTextbox->text().toUpper();
    QString subjectDesc = ui->subjectDescTextbox->text().toUpper();
    QString schoolYear = ui->schoolYearCombobox->currentText();
    QString semester = ui->semesterCombobox->currentText();
    QString program = ui->programCombobox->currentText();
    QString year = ui->yearCombobox->currentText();
    QString section = ui->sectionCombobox->currentText();
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

    // Extract unnecessary substring for data handling
    static const QRegularExpression whitespace("\\s+");
    subjectCode.replace(whitespace, "");
    int programIndex = program.indexOf('|');
    program = program.left(programIndex).trimmed();

    // Must return result in QString from verify function
    QString result = Admin_Classes_Create::verifyClassCreate(subjectCode, subjectDesc, schoolYear, semester,
                                                            program, year, section, room, lecturerId,
                                                            firstDay, firstStartTime, firstEndTime,
                                                            secondDay, secondStartTime, secondEndTime,
                                                            thirdDay, thirdStartTime, thirdEndTime);

    // Return error if the result is not empty
    if (!result.isEmpty())
    {
        GlobalTimer::displayTextForDuration(ui->errorLabel, result, 5000);
        return;
    }

    // Proceed to inserting data to database
    Admin_Classes_Create::insertClassData(subjectCode, subjectDesc, schoolYear, semester,
                                               program, year, section, room, lecturerId,
                                               firstDay, firstStartTime, firstEndTime,
                                               secondDay, secondStartTime, secondEndTime,
                                               thirdDay, thirdStartTime, thirdEndTime);
}


QString Admin_Classes_Create::verifyClassCreate(const QString &subjectCode, const QString &subjectDesc,
                                                const QString &schoolYear, const QString &semester, const QString &program,
                                                const QString &year, const QString &section, const QString &room, const QString &lecturerId,
                                                const QString &firstDay, const QString &firstStartTime, const QString &firstEndTime,
                                                const QString &secondDay, const QString &secondStartTime, const QString &secondEndTime,
                                                const QString &thirdDay, const QString &thirdStartTime, const QString &thirdEndTime)
{
    if (subjectCode == "") { return Messages::emptySubjectCode(); }
    if (subjectDesc == "") { return Messages::emptySubjectDesc(); }
    if (schoolYear == "") { return Messages::noSelectedSchoolYear(); }
    if (semester == "") { return Messages::noSelectedSemester(); }
    if (program == "") { return Messages::noSelectedProgram(); }
    if (year == "") { return Messages::noSelectedYear(); }
    if (section == "") { return Messages::noSelectedSection(); }
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
        return Messages::errorSelectData();
    }

    query.next();
    int count = query.value(0).toInt();
    bool isLecturerIdExist = count > 0;

    // Close the database after using
    QSqlDatabase::database().commit();
    database.close();

    if (!isLecturerIdExist) { return Messages::notExistLecturerId(); }

    return "";
}


void Admin_Classes_Create::insertClassData(const QString &subjectCode, const QString &subjectDesc,
                                           const QString &schoolYear, const QString &semester, const QString &program,
                                           const QString &year, const QString &section, const QString &room, const QString &lecturerId,
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

    // Set up queries for database
    QSqlDatabase::database().transaction();
    QSqlQuery query(database);

    // Prepare sql command for inserting data
    query.prepare("INSERT INTO ClassInfo(SubjectCode, SubjectDesc, SchoolYear, Semester, Program, Year, Section, Day, Time, Room, LecturerId) \
                  VALUES(:subjectCode, :subjectDesc, :schoolYear, :semester, :program, :year, :section, :day, :time, :room, :lecturerId)");

    // Bind values to the query
    query.bindValue(":subjectCode", subjectCode);
    query.bindValue(":subjectDesc", subjectDesc);
    query.bindValue(":schoolYear", schoolYear);
    query.bindValue(":semester", semester);
    query.bindValue(":program", program);
    query.bindValue(":year", year);
    query.bindValue(":section", section);
    query.bindValue(":day", combinedDay);
    query.bindValue(":time", combinedTime);
    query.bindValue(":room", room);
    query.bindValue(":lecturerId", lecturerId);

    // Return error if unable to insert data
    if (!query.exec())
    {
        QSqlDatabase::database().rollback();
        GlobalTimer::displayTextForDuration(ui->errorLabel, Messages::errorInsertData(), 5000);
        return;
    }
    query.clear();

    // Create separate table after successfully inserting data
    QString tableName = QString("%1%2%3%4_S%5SY%6").arg(subjectCode, program, year, section, semester, FilteringManager::convertSchoolYear(schoolYear));

    QString queryString = "CREATE TABLE " + $db_Database + "." + tableName + " (\
                          `StudentId` CHAR(9) NOT NULL, \
                          `PresentCount` TINYINT NOT NULL, \
                          `AbsentCount` TINYINT NOT NULL, \
                          PRIMARY KEY (`StudentId`(9)))";
    query.prepare(queryString);

    // Return error if unable to create table
    if (!query.exec())
    {
        qDebug() << query.lastError().text();
        QSqlDatabase::database().rollback();
        GlobalTimer::displayTextForDuration(ui->errorLabel, Messages::errorCreateTable(), 5000);
        return;
    }

    // Close the database after using
    QSqlDatabase::database().commit();
    database.close();

    Admin_Classes_Create::addStudentsToClass(tableName, program, year, section);
    Admin_Classes_Create::switchWindow_AdminClassesList();
}


void Admin_Classes_Create::addStudentsToClass(const QString &tableName, const QString &program, const QString &year, const QString &section)
{
    // Return error if unable to access the database
    if (!database.open())
    {
        GlobalTimer::displayTextForDuration(ui->errorLabel, Messages::unaccessDatabase(), 5000);
        return;
    }

    // Set up data list and queries for database
    QStringList studentIdList;
    QSqlDatabase::database().transaction();
    QSqlQuery query(database);

    // Prepare sql command for selecting data
    query.prepare("SELECT StudentId FROM StudentInfo WHERE \
                  Program = :program AND Year = :year AND Section = :section");

    // Bind values to the query
    query.bindValue(":program", program);
    query.bindValue(":year", year);
    query.bindValue(":section", section);

    // Return error if unable to select data
    if (!query.exec())
    {
        QSqlDatabase::database().rollback();
        GlobalTimer::displayTextForDuration(ui->errorLabel, Messages::errorSelectData(), 5000);
        return;
    }
    else
    {
        // Fetch and store the result rows
        while (query.next())
        {
            studentIdList << query.value(0).toString();
        }
    }
    query.clear();

    // Iterate all studentId from studentIdList
    for (const auto &studentId : studentIdList)
    {
        // Prepare sql command for inserting data
        QString queryString = "INSERT INTO " + tableName + "(StudentId) ";
        queryString += "VALUES(:studentId)";
        query.prepare(queryString);

        // Bind values to the query
        query.bindValue(":studentId", studentId);

        if (!query.exec())
        {
            QSqlDatabase::database().rollback();
            GlobalTimer::displayTextForDuration(ui->errorLabel, Messages::errorInsertData(), 5000);
            return;
        }
    }

    // Close the database after using
    QSqlDatabase::database().commit();
    database.close();
}


void Admin_Classes_Create::populateCombobox()
{
    // Populate comboboxes with items
    ui->schoolYearCombobox->addItems(Schedules::$schoolYears);
    ui->semesterCombobox->addItems(Schedules::$semesters);
    ui->programCombobox->addItems(Colleges::$programs);
    ui->yearCombobox->addItems(Colleges::$years);
    ui->sectionCombobox->addItems(Colleges::$sections);

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
    connect(ui->schoolYearCombobox, &QComboBox::currentIndexChanged, this, [=](){ FilteringManager::checkComboboxIndex(ui->schoolYearCombobox); });
    connect(ui->semesterCombobox, &QComboBox::currentIndexChanged, this, [=](){ FilteringManager::checkComboboxIndex(ui->semesterCombobox); });
    connect(ui->programCombobox, &QComboBox::currentIndexChanged, this, [=](){ FilteringManager::checkComboboxIndex(ui->programCombobox); });
    connect(ui->yearCombobox, &QComboBox::currentIndexChanged, this, [=](){ FilteringManager::checkComboboxIndex(ui->yearCombobox); });
    connect(ui->sectionCombobox, &QComboBox::currentIndexChanged, this, [=](){ FilteringManager::checkComboboxIndex(ui->sectionCombobox); });
    connect(ui->firstDayCombobox, &QComboBox::currentIndexChanged, this, [=](){ FilteringManager::checkComboboxIndex(ui->firstDayCombobox); });
    connect(ui->secondDayCombobox, &QComboBox::currentIndexChanged, this, [=](){ FilteringManager::checkComboboxIndex(ui->secondDayCombobox); });
    connect(ui->thirdDayCombobox, &QComboBox::currentIndexChanged, this, [=](){ FilteringManager::checkComboboxIndex(ui->thirdDayCombobox); });
}


void Admin_Classes_Create::switchWindow_AdminStudentsList()
{
    // Switch ui window to Admin_Students_List
    admin_students_list = new Admin_Students_List;
    admin_students_list->show();
    this->hide();
}


void Admin_Classes_Create::switchWindow_AdminClassesList()
{
    // Switch ui window to Admin_Classes_List
    admin_classes_list = new Admin_Classes_List;
    admin_classes_list->show();
    this->hide();
}


void Admin_Classes_Create::switchWindow_AdminLecturersList()
{
    // Switch ui window to Admin_Lecturers_List
    admin_lecturers_list = new Admin_Lecturers_List;
    admin_lecturers_list->show();
    this->hide();
}


void Admin_Classes_Create::moveEvent(QMoveEvent *event)
{
    // Saves window position every movement instance of a window
    QMainWindow::moveEvent(event);
    WindowPositionManager::saveWindowPosition(event->pos());
}
