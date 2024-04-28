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

    // Set up sql online database with connection parameters
    database = QSqlDatabase::addDatabase($sql_Driver);
    database.setHostName($db_Hostname);
    database.setUserName($db_Username);
    database.setPassword($db_Password);
    database.setDatabaseName($db_Database);
    database.setPort($db_Port);

    // Initiate functions on awake
    Admin_Lecturers_Add::populateCombobox();
    DateTimeUtils::updateDateTimeUtils(ui->dateLabel, ui->timeLabel);

    // Connect ui objects to functions based on user interaction
    connect(ui->confirmButton, &QPushButton::clicked, this, &Admin_Lecturers_Add::lecturerAddCall);

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


void Admin_Lecturers_Add::lecturerAddCall()
{
    // Fetch string input text() from textboxes and comboboxes
    QString lecturerId = ui->lecturerIdTextbox->text();
    QString lastName = ui->lastNameTextbox->text().toUpper();
    QString firstName = ui->firstNameTextbox->text().toUpper();
    QString college = ui->collegeCombobox->currentText();

    // Extract unnecessary substring for data handling
    int collegeIndex = college.indexOf('|');
    college = college.left(collegeIndex).trimmed();

    // Must return result in QString from verify function
    QString result = Admin_Lecturers_Add::verifyLecturerAdd(lecturerId, lastName, firstName, college);

    // Return error if the result is not empty
    if (!result.isEmpty())
    {
        GlobalTimer::displayTextForDuration(ui->errorLabel, result, 5000);
        return;
    }

    // Proceed to inserting data to database
    Admin_Lecturers_Add::insertDataToDatabase(lecturerId, lastName, firstName, college);
}


QString Admin_Lecturers_Add::verifyLecturerAdd(const QString &lecturerId, const QString &lastName, const QString &firstName, const QString &college)
{
    if (lecturerId == "") { return Messages::emptyLecturerId(); }
    if (lecturerId.length() != $lecturerIdLength) { return Messages::incompleteLengthLecturerId(); }
    if (lecturerId.contains(" ")) { return Messages::invalidLecturerId(); }

    bool isNum;
    int _lecturerId = lecturerId.toInt(&isNum);
    if (!isNum) { return _lecturerId + "" + Messages::invalidLecturerId(); }

    if (lastName == "") { return Messages::emptyLastName(); }
    if (firstName == "") { return Messages::emptyFirstName(); }
    if (college == "") { return Messages::noSelectedCollege(); }
    return "";
}


void Admin_Lecturers_Add::insertDataToDatabase(const QString &lecturerId, const QString &lastName, const QString &firstName, const QString &college)
{
    // Return error if unable to access the database
    if (!database.open())
    {
        GlobalTimer::displayTextForDuration(ui->errorLabel, Messages::unaccessDatabase(), 5000);
        return;
    }

    // Generate random 6-digit pin
    int generatedPin = QRandomGenerator::global()->bounded(111111, 1000000);

    // Set up queries for database
    QSqlDatabase::database().transaction();
    QSqlQuery query(database);

    // Prepare sql command for inserting data
    query.prepare("INSERT INTO LecturerInfo(LecturerId, Pin, LastName, FirstName, College) \
                  VALUES(:lecturerId, :pin, :lastName, :firstName, :college)");

    // Bind values to the query
    query.bindValue(":lecturerId", lecturerId);
    query.bindValue(":pin", generatedPin);
    query.bindValue(":lastName", lastName);
    query.bindValue(":firstName", firstName);
    query.bindValue(":college", college);

    // Return error if unable to insert data
    if (!query.exec())
    {
        QSqlDatabase::database().rollback();
        QString errorText = query.lastError().databaseText();

        // Return error if lecturer number already exist
        if (errorText.contains("UNIQUE", Qt::CaseInsensitive) || errorText.contains("duplicate", Qt::CaseInsensitive))
        {
            GlobalTimer::displayTextForDuration(ui->errorLabel, Messages::alreadyExistLecturerId(), 5000);
            return;
        }

        GlobalTimer::displayTextForDuration(ui->errorLabel, Messages::errorInsertData(), 5000);
        return;
    }

    // Close the database after using
    QSqlDatabase::database().commit();
    database.close();

    Admin_Lecturers_Add::switchWindow_AdminLecturersList();
}


void Admin_Lecturers_Add::populateCombobox()
{
    // Populate comboboxes with items
    ui->collegeCombobox->addItems(Colleges::$colleges);

    // Connect comboboxes to checkComboboxIndex()
    connect(ui->collegeCombobox, &QComboBox::currentIndexChanged, this, [=](){ FilteringManager::checkComboboxIndex(ui->collegeCombobox); });
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
