#include "global_main.h"

#include "admin_students_update.h"
#include "ui_admin_students_update.h"


// Include header files for forward declaration
#include "admin_students_list.h"
#include "admin_classes_list.h"
#include "admin_lecturers_list.h"


Admin_Students_Update::Admin_Students_Update(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Admin_Students_Update)
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
    Admin_Students_Update::populateCombobox();
    Admin_Students_Update::selectDataFromDatabase($updateKey_StudentInfo);

    // Connect ui objects to functions based on user interaction
    connect(ui->updateButton, &QPushButton::clicked, this, &Admin_Students_Update::studentUpdateCall);

    connect(ui->backButton, &QPushButton::clicked, this, &Admin_Students_Update::switchWindow_AdminStudentsList);
    connect(ui->studentListButton, &QPushButton::clicked, this, &Admin_Students_Update::switchWindow_AdminStudentsList);
    connect(ui->classListButton, &QPushButton::clicked, this, &Admin_Students_Update::switchWindow_AdminClassesList);
    connect(ui->lecturerListButton, &QPushButton::clicked, this, &Admin_Students_Update::switchWindow_AdminLecturersList);

    // Connect clicked signal of the radio buttons to function
    connect(ui->regularRadio, &QRadioButton::clicked, this, [=](){ Admin_Students_Update::handleIsRegularRadio("Regular"); });
    connect(ui->irregularRadio, &QRadioButton::clicked, this, [=](){ Admin_Students_Update::handleIsRegularRadio("Irregular"); });

    // Load window position
    QPoint windowPos = WindowPositionManager::loadWindowPosition();
    windowPos.setY(windowPos.y() - 31);
    move(windowPos);
}


Admin_Students_Update::~Admin_Students_Update()
{
    delete ui;
}


void Admin_Students_Update::selectDataFromDatabase(const QString &key_studentId)
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
    query.prepare("SELECT * FROM StudentInfo WHERE StudentId = :studentId");

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
    studentDataList << query.value(3).toString();   // FirstName
    studentDataList << query.value(4).toString();   // College
    studentDataList << query.value(5).toString();   // Program
    studentDataList << query.value(6).toString();   // Year
    studentDataList << query.value(7).toString();   // Section
    studentDataList << query.value(8).toString();   // IsRegular

    // Close the database after using
    QSqlDatabase::database().commit();
    database.close();

    // Proceed to display data list
    Admin_Students_Update::displayDataFromDatabase(studentDataList);
}


void Admin_Students_Update::displayDataFromDatabase(const QStringList &dataList)
{
    // Assign members from dataList to variables
    QString studentId = dataList[0];
    QString pin = dataList[1];
    QString lastName = dataList[2];
    QString firstName = dataList[3];
    QString college = dataList[4];
    QString program = dataList[5];
    QString year = dataList[6];
    QString section = dataList[7];
    QString isRegular = dataList[8];

    // Update the form with the current data
    if (isRegular == "1")
    {
        ui->regularRadio->setChecked(true);
        regularity = QString("true");
    }
    else
    {
        ui->irregularRadio->setChecked(true);
        regularity = QString("false");
    }

    ui->studentIdLabel->setText(studentId);
    ui->lastNameTextbox->setText(lastName);
    ui->firstNameTextbox->setText(firstName);
    ui->pinTextbox->setText(pin);

    FilteringManager::selectOptionByText(ui->collegeCombobox, college);
    FilteringManager::selectOptionByText(ui->programCombobox, program);
    ui->yearCombobox->setCurrentText(year);
    ui->sectionCombobox->setCurrentText(section);
}


void Admin_Students_Update::studentUpdateCall()
{
    // Fetch string input text() from textboxes and comboboxes
    QString lastName = ui->lastNameTextbox->text().toUpper();
    QString firstName = ui->firstNameTextbox->text().toUpper();
    QString college = ui->collegeCombobox->currentText();
    QString program = ui->programCombobox->currentText();
    QString year = ui->yearCombobox->currentText();
    QString section = ui->sectionCombobox->currentText();
    QString pin = ui->pinTextbox->text();

    // Extract unnecessary substring for data handling
    int collegeIndex = college.indexOf('|');
    int programIndex = program.indexOf('|');
    college = college.left(collegeIndex).trimmed();
    program = program.left(programIndex).trimmed();

    // Must return result in QString from verify function
    QString result = Admin_Students_Update::verifyStudentUpdate(lastName, firstName, college, program, year, section, pin);

    // Return error if the result is not empty
    if (!result.isEmpty())
    {
        GlobalTimer::displayTextForDuration(ui->errorLabel, result, 5000);
        return;
    }

    // Proceed to updating data from database
    Admin_Students_Update::updateDataFromDatabase(lastName, firstName, college, program, year, section, pin);
}


QString Admin_Students_Update::verifyStudentUpdate(const QString &lastName, const QString &firstName,
                                                const QString &college, const QString &program,
                                                const QString &year, const QString &section, const QString &pin)
{
    if (regularity == "") { return Messages::noSelectedIsRegular(); }
    if (lastName == "") { return Messages::emptyLastName(); }
    if (firstName == "") { return Messages::emptyFirstName(); }
    if (college == "") { return Messages::noSelectedCollege(); }
    if (program == "") { return Messages::noSelectedProgram(); }
    if (year == "") { return Messages::noSelectedYear(); }
    if (section == "") { return Messages::noSelectedSection(); }

    if (pin == "") { return Messages::emptyPin(); }
    if (pin.length() != $pinLength) { return Messages::incompleteLengthPin(); }
    if (pin.contains(" ")) { return Messages::invalidPin(); }

    bool isNum;
    int _pin = pin.toInt(&isNum);
    if (!isNum) { return _pin + "" + Messages::invalidPin(); }

    return "";
}


void Admin_Students_Update::updateDataFromDatabase(const QString &lastname, const QString &firstName,
                                                   const QString &college, const QString &program,
                                                   const QString &year, const QString &section, const QString &pin)
{
    // Return error if unable to access the database
    if (!database.open())
    {
        GlobalTimer::displayTextForDuration(ui->errorLabel, Messages::unaccessDatabase(), 5000);
        return;
    }

    // Convert QString to bool in short form
    short isRegularBool = (regularity == "true" ? 1 : 0);

    // Set up queries for database
    QSqlDatabase::database().transaction();
    QSqlQuery query(database);

    // Prepare sql command for updating data
    query.prepare("UPDATE StudentInfo SET \
                  Pin = :pin, LastName = :lastName, FirstName = :firstName, College = :college, Program = :program, \
                  Year = :year, Section = :section, IsRegular = :isRegular \
                  WHERE StudentId = :studentId");

    // Bind values to the query
    query.bindValue(":pin", pin);
    query.bindValue(":lastName", lastname);
    query.bindValue(":firstName", firstName);
    query.bindValue(":college", college);
    query.bindValue(":program", program);
    query.bindValue(":year", year);
    query.bindValue(":section", section);
    query.bindValue(":isRegular", isRegularBool);
    query.bindValue(":studentId", $updateKey_StudentInfo);

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

    Admin_Students_Update::switchWindow_AdminStudentsList();
}


void Admin_Students_Update::handleIsRegularRadio(const QString &option)
{
    // Return true if option selected is regular, otherwise false
    regularity = (option == "Regular" ? QString("true") : QString("false"));
}


void Admin_Students_Update::populateCombobox()
{
    // Populate comboboxes with items
    ui->collegeCombobox->addItems(Colleges::$colleges);
    ui->programCombobox->setDisabled(true);
    ui->yearCombobox->addItems(Colleges::$years);
    ui->sectionCombobox->addItems(Colleges::$sections);

    // Connect collegeComboBox's currentIndexChanged signal to collegeIndexChanged()
    connect(ui->collegeCombobox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &Admin_Students_Update::collegeIndexChanged);

    // Connect comboboxes to checkComboboxIndex()
    connect(ui->collegeCombobox, &QComboBox::currentIndexChanged, this, [=](){ FilteringManager::checkComboboxIndex(ui->collegeCombobox); });
    connect(ui->programCombobox, &QComboBox::currentIndexChanged, this, [=](){ FilteringManager::checkComboboxIndex(ui->programCombobox); });
    connect(ui->yearCombobox, &QComboBox::currentIndexChanged, this, [=](){ FilteringManager::checkComboboxIndex(ui->yearCombobox); });
    connect(ui->sectionCombobox, &QComboBox::currentIndexChanged, this, [=](){ FilteringManager::checkComboboxIndex(ui->sectionCombobox); });
}


void Admin_Students_Update::collegeIndexChanged(int index)
{
    // Fetch selected college and find corresponding programs for that
    const QString college = ui->collegeCombobox->itemText(index);
    const QStringList programs = Colleges::$offeredPrograms.value(college);

    // Clear, add, and enable/disable combobox based on whether programs are available
    ui->programCombobox->clear();
    ui->programCombobox->addItems(programs);
    ui->programCombobox->setEnabled(!programs.isEmpty());
}


void Admin_Students_Update::switchWindow_AdminStudentsList()
{
    // Switch ui window to Admin_Students_List
    admin_students_List = new Admin_Students_List;
    admin_students_List->show();
    this->hide();
}


void Admin_Students_Update::switchWindow_AdminClassesList()
{
    // Switch ui window to Admin_Classes_List
    admin_classes_list = new Admin_Classes_List;
    admin_classes_list->show();
    this->hide();
}


void Admin_Students_Update::switchWindow_AdminLecturersList()
{
    // Switch ui window to Admin_Lecturers_List
    admin_lecturers_list = new Admin_Lecturers_List;
    admin_lecturers_list->show();
    this->hide();
}


void Admin_Students_Update::moveEvent(QMoveEvent *event)
{
    // Saves window position every movement instance of a window
    QMainWindow::moveEvent(event);
    WindowPositionManager::saveWindowPosition(event->pos());
}
