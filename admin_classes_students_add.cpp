#include "global_main.h"

#include "admin_classes_students_add.h"
#include "ui_admin_classes_students_add.h"


// Include header files for forward declaration
#include "admin_students_list.h"
#include "admin_classes_list.h"
#include "admin_lecturers_list.h"
#include "admin_classes_students_list.h"


Admin_Classes_Students_Add::Admin_Classes_Students_Add(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Admin_Classes_Students_Add)
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
    DateTimeUtils::updateDateTimeUtils(ui->dateLabel, ui->timeLabel);

    // Connect ui objects to functions based on user interaction
    connect(ui->confirmButton, &QPushButton::clicked, this, &Admin_Classes_Students_Add::classStudentAddCall);

    connect(ui->backButton, &QPushButton::clicked, this, &Admin_Classes_Students_Add::switchWindow_AdminClassesStudentsList);
    connect(ui->studentListButton, &QPushButton::clicked, this, &Admin_Classes_Students_Add::switchWindow_AdminStudentsList);
    connect(ui->classListButton, &QPushButton::clicked, this, &Admin_Classes_Students_Add::switchWindow_AdminClassesList);
    connect(ui->lecturerListButton, &QPushButton::clicked, this, &Admin_Classes_Students_Add::switchWindow_AdminLecturersList);

    // Load window position
    QPoint windowPos = WindowPositionManager::loadWindowPosition();
    windowPos.setY(windowPos.y() - 31);
    move(windowPos);
}


Admin_Classes_Students_Add::~Admin_Classes_Students_Add()
{
    delete ui;
}


void Admin_Classes_Students_Add::classStudentAddCall()
{
    // Fetch string input text() from textboxes and comboboxes
    QString studentId = ui->studentIdTextbox->text().toUpper();
    QStringList keys_classInfo = $selectKeys_ClassInfo;

    // Must return result in QString from verify function
    QString result = Admin_Classes_Students_Add::verifyClassStudentAdd(studentId);

    // Return error if the result is not empty
    if (!result.isEmpty())
    {
        GlobalTimer::displayTextForDuration(ui->errorLabel, result, 5000);
        return;
    }

    // Proceed to inserting data to database
    Admin_Classes_Students_Add::insertDataToDatabase(studentId, keys_classInfo);
}


QString Admin_Classes_Students_Add::verifyClassStudentAdd(const QString &studentId)
{
    if (studentId == "") { return Messages::emptyStudentId(); }
    if (studentId.length() != $studentIdLength) { return Messages::incompleteLengthStudentId(); }
    if (studentId.contains(" ")) { return Messages::invalidStudentId(); }

    // Return error if unable to access the database
    if (!database.open()) { return Messages::unaccessDatabase(); }

    // Set up queries for database
    QSqlDatabase::database().transaction();
    QSqlQuery query(database);

    // Prepare sql command for finding data
    query.prepare("SELECT COUNT(*) FROM StudentInfo WHERE StudentId = :studentId");

    // Bind values to the query
    query.bindValue(":studentId", studentId);

    // Return error if unable to insert data
    if (!query.exec())
    {
        QSqlDatabase::database().rollback();
        return Messages::errorSelectData();
    }

    query.next();
    int count = query.value(0).toInt();
    bool isStudentIdExist = count > 0;

    // Close the database after using
    QSqlDatabase::database().commit();
    database.close();

    if (!isStudentIdExist) { return Messages::notExistStudentId(); }

    return "";
}


void Admin_Classes_Students_Add::insertDataToDatabase(const QString &studentId, const QStringList &keys_classInfo)
{
    // Return error if unable to access the database
    if (!database.open())
    {
        GlobalTimer::displayTextForDuration(ui->errorLabel, Messages::unaccessDatabase(), 5000);
        return;
    }

    // Get data from select key
    QString key_subjectCode = keys_classInfo[0];
    QString key_program = keys_classInfo[1];
    QString key_year = keys_classInfo[2];
    QString key_section = keys_classInfo[3];
    QString key_semester = keys_classInfo[4];
    QString key_schoolYear = keys_classInfo[5];

    QString tableName = QString("%1%2%3%4_S%5SY%6").arg(key_subjectCode, key_program, key_year, key_section, key_semester, FilteringManager::convertSchoolYear(key_schoolYear));

    // Set up queries for database
    QSqlDatabase::database().transaction();
    QSqlQuery query(database);

    // Prepare sql command for checking the number of columns
    query.prepare("SELECT COUNT(*) FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_SCHEMA = :database AND TABLE_NAME = :tableName");

    // Bind values to the query
    query.bindValue(":database", $db_Database);
    query.bindValue(":tableName", tableName);

    // Return error if unable to get info
    if (!query.exec())
    {
        QSqlDatabase::database().rollback();
        GlobalTimer::displayTextForDuration(ui->errorLabel, Messages::errorSelectData(), 5000);
        return;
    }

    query.next();
    int columnCount = query.value(0).toInt() - 3;   // Subtracted the count of first three columns

    query.clear();

    // Prepare sql command for inserting data
    query.prepare("INSERT INTO " + tableName + "(StudentId, PresentCount, AbsentCount) \
                  VALUES(:studentId, :presentCount, :absentCount)");

    // Bind values to the query
    query.bindValue(":studentId", studentId);
    query.bindValue(":presentCount", 0);
    query.bindValue(":absentCount", columnCount);

    // Return error if unable to insert data
    if (!query.exec())
    {
        QSqlDatabase::database().rollback();
        QString errorText = query.lastError().databaseText();

        // Return error if student number already enrolled
        if (errorText.contains("UNIQUE", Qt::CaseInsensitive) || errorText.contains("duplicate", Qt::CaseInsensitive))
        {
            GlobalTimer::displayTextForDuration(ui->errorLabel, Messages::alreadyEnrolledStudentId(), 5000);
            return;
        }

        GlobalTimer::displayTextForDuration(ui->errorLabel, Messages::errorInsertData(), 5000);
        return;
    }

    // Close the database after using
    QSqlDatabase::database().commit();
    database.close();

    Admin_Classes_Students_Add::switchWindow_AdminClassesStudentsList();
}


void Admin_Classes_Students_Add::switchWindow_AdminStudentsList()
{
    // Switch ui window to Admin_Students_List
    admin_students_list = new Admin_Students_List;
    admin_students_list->show();
    this->hide();
}


void Admin_Classes_Students_Add::switchWindow_AdminClassesList()
{
    // Switch ui window to Admin_Classes_List
    admin_classes_list = new Admin_Classes_List;
    admin_classes_list->show();
    this->hide();
}


void Admin_Classes_Students_Add::switchWindow_AdminLecturersList()
{
    // Switch ui window to Admin_Lecturers_List
    admin_lecturers_list = new Admin_Lecturers_List;
    admin_lecturers_list->show();
    this->hide();
}


void Admin_Classes_Students_Add::switchWindow_AdminClassesStudentsList()
{
    // Switch ui window to Admin_Classes_Students_Add
    admin_classes_students_list = new Admin_Classes_Students_List;
    admin_classes_students_list->show();
    this->hide();
}


void Admin_Classes_Students_Add::moveEvent(QMoveEvent *event)
{
    // Saves window position every movement instance of a window
    QMainWindow::moveEvent(event);
    WindowPositionManager::saveWindowPosition(event->pos());
}
