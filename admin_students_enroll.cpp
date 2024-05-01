#include "global_main.h"

#include "admin_students_enroll.h"
#include "ui_admin_students_enroll.h"


// Include header files for forward declaration
#include "admin_students_list.h"
#include "admin_classes_list.h"
#include "admin_lecturers_list.h"


Admin_Students_Enroll::Admin_Students_Enroll(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Admin_Students_Enroll)
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
    Admin_Students_Enroll::populateCombobox();
    DateTimeUtils::updateDateTimeUtils(ui->dateLabel, ui->timeLabel);

    // Connect ui objects to functions based on user interaction
    connect(ui->confirmButton, &QPushButton::clicked, this, &Admin_Students_Enroll::studentEnrollCall);

    connect(ui->backButton, &QPushButton::clicked, this, &Admin_Students_Enroll::switchWindow_AdminStudentsList);
    connect(ui->studentListButton, &QPushButton::clicked, this, &Admin_Students_Enroll::switchWindow_AdminStudentsList);
    connect(ui->classListButton, &QPushButton::clicked, this, &Admin_Students_Enroll::switchWindow_AdminClassesList);
    connect(ui->lecturerListButton, &QPushButton::clicked, this, &Admin_Students_Enroll::switchWindow_AdminLecturersList);

    // Connect clicked signal of the radio buttons to function
    connect(ui->regularRadio, &QRadioButton::clicked, this, [=](){ Admin_Students_Enroll::handleIsRegularRadio("Regular"); });
    connect(ui->irregularRadio, &QRadioButton::clicked, this, [=](){ Admin_Students_Enroll::handleIsRegularRadio("Irregular"); });

    // Load window position
    QPoint windowPos = WindowPositionManager::loadWindowPosition();
    windowPos.setY(windowPos.y() - 31);
    move(windowPos);
}


Admin_Students_Enroll::~Admin_Students_Enroll()
{
    delete ui;
}


void Admin_Students_Enroll::studentEnrollCall()
{
    // Fetch string input text() from textboxes and comboboxes
    QString studentId = ui->studentIdTextbox->text().toUpper();
    QString lastName = ui->lastNameTextbox->text().toUpper();
    QString firstName = ui->firstNameTextbox->text().toUpper();
    QString college = ui->collegeCombobox->currentText();
    QString program = ui->programCombobox->currentText();
    QString year = ui->yearCombobox->currentText();
    QString section = ui->sectionCombobox->currentText();

    // Extract unnecessary substring for data handling
    int collegeIndex = college.indexOf('|');
    int programIndex = program.indexOf('|');
    college = college.left(collegeIndex).trimmed();
    program = program.left(programIndex).trimmed();

    // Must return result in QString from verify function
    QString result = Admin_Students_Enroll::verifyStudentEnroll(studentId, lastName, firstName, college, program, year, section);

    // Return error if the result is not empty
    if (!result.isEmpty())
    {
        GlobalTimer::displayTextForDuration(ui->errorLabel, result, 5000);
        return;
    }

    // Proceed to inserting data to database
    Admin_Students_Enroll::insertStudentData(studentId, lastName, firstName, college, program, year, section);
}


QString Admin_Students_Enroll::verifyStudentEnroll(const QString &studentId, const QString &lastName, const QString &firstName,
                                                   const QString &college, const QString &program,
                                                   const QString &year, const QString &section)
{
    if (regularity == "") { return Messages::noSelectedIsRegular(); }
    if (studentId == "") { return Messages::emptyStudentId(); }
    if (studentId.length() != $studentIdLength) { return Messages::incompleteLengthStudentId(); }
    if (studentId.contains(" ")) { return Messages::invalidStudentId(); }
    if (lastName == "") { return Messages::emptyLastName(); }
    if (firstName == "") { return Messages::emptyFirstName(); }
    if (college == "") { return Messages::noSelectedCollege(); }
    if (program == "") { return Messages::noSelectedProgram(); }
    if (year == "") { return Messages::noSelectedYear(); }
    if (section == "") { return Messages::noSelectedSection(); }
    return "";
}


void Admin_Students_Enroll::insertStudentData(const QString &studentId, const QString &lastName, const QString &firstName,
                                              const QString &college, const QString &program,
                                              const QString &year, const QString &section)
{
    // Return error if unable to access the database
    if (!database.open())
    {
        GlobalTimer::displayTextForDuration(ui->errorLabel, Messages::unaccessDatabase(), 5000);
        return;
    }

    // Generate random 6-digit pin
    int generatedPin = QRandomGenerator::global()->bounded(111111, 1000000);

    // Convert QString to bool in short form
    short isRegularBool = (regularity == "true" ? 1 : 0);

    // Set up queries for database
    QSqlDatabase::database().transaction();
    QSqlQuery query(database);

    // Prepare sql command for inserting data
    query.prepare("INSERT INTO StudentInfo(StudentId, Pin, LastName, FirstName, College, Program, Year, Section, IsRegular) \
                  VALUES(:studentId, :pin, :lastName, :firstName, :college, :program, :year, :section, :isRegular)");

    // Bind values to the query
    query.bindValue(":studentId", studentId);
    query.bindValue(":pin", generatedPin);
    query.bindValue(":lastName", lastName);
    query.bindValue(":firstName", firstName);
    query.bindValue(":college", college);
    query.bindValue(":program", program);
    query.bindValue(":year", year);
    query.bindValue(":section", section);
    query.bindValue(":isRegular", isRegularBool);

    // Return error if unable to insert data
    if (!query.exec())
    {
        QSqlDatabase::database().rollback();
        QString errorText = query.lastError().databaseText();

        // Return error if student number already exist
        if (errorText.contains("UNIQUE", Qt::CaseInsensitive) || errorText.contains("duplicate", Qt::CaseInsensitive))
        {
            GlobalTimer::displayTextForDuration(ui->errorLabel, Messages::alreadyExistStudentId(), 5000);
            return;
        }

        GlobalTimer::displayTextForDuration(ui->errorLabel, Messages::errorInsertData(), 5000);
        return;
    }

    // Close the database after using
    QSqlDatabase::database().commit();
    database.close();

    Admin_Students_Enroll::switchWindow_AdminStudentsList();
}


void Admin_Students_Enroll::handleIsRegularRadio(const QString &option)
{
    // Return true if option selected is regular, otherwise false
    regularity = (option == "Regular" ? QString("true") : QString("false"));
}


void Admin_Students_Enroll::populateCombobox()
{
    // Populate comboboxes with items
    ui->collegeCombobox->addItems(Colleges::$colleges);
    ui->programCombobox->setDisabled(true);
    ui->yearCombobox->addItems(Colleges::$years);
    ui->sectionCombobox->addItems(Colleges::$sections);

    // Connect collegeComboBox's currentIndexChanged signal to collegeIndexChanged()
    connect(ui->collegeCombobox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &Admin_Students_Enroll::collegeIndexChanged);

    // Connect comboboxes to checkComboboxIndex()
    connect(ui->collegeCombobox, &QComboBox::currentIndexChanged, this, [=](){ FilteringManager::checkComboboxIndex(ui->collegeCombobox); });
    connect(ui->programCombobox, &QComboBox::currentIndexChanged, this, [=](){ FilteringManager::checkComboboxIndex(ui->programCombobox); });
    connect(ui->yearCombobox, &QComboBox::currentIndexChanged, this, [=](){ FilteringManager::checkComboboxIndex(ui->yearCombobox); });
    connect(ui->sectionCombobox, &QComboBox::currentIndexChanged, this, [=](){ FilteringManager::checkComboboxIndex(ui->sectionCombobox); });
}


void Admin_Students_Enroll::collegeIndexChanged(int index)
{
    // Fetch selected college and find corresponding programs for that
    const QString college = ui->collegeCombobox->itemText(index);
    const QStringList programs = Colleges::$offeredPrograms.value(college);

    // Clear, add, and enable/disable combobox based on whether programs are available
    ui->programCombobox->clear();
    ui->programCombobox->addItems(programs);
    ui->programCombobox->setEnabled(!programs.isEmpty());
}


void Admin_Students_Enroll::switchWindow_AdminStudentsList()
{
    // Switch ui window to Admin_Students_List
    admin_students_list = new Admin_Students_List;
    admin_students_list->show();
    this->hide();
}


void Admin_Students_Enroll::switchWindow_AdminClassesList()
{
    // Switch ui window to Admin_Classes_List
    admin_classes_list = new Admin_Classes_List;
    admin_classes_list->show();
    this->hide();
}


void Admin_Students_Enroll::switchWindow_AdminLecturersList()
{
    // Switch ui window to Admin_Lecturers_List
    admin_lecturers_list = new Admin_Lecturers_List;
    admin_lecturers_list->show();
    this->hide();
}


void Admin_Students_Enroll::moveEvent(QMoveEvent *event)
{
    // Saves window position every movement instance of a window
    QMainWindow::moveEvent(event);
    WindowPositionManager::saveWindowPosition(event->pos());
}
