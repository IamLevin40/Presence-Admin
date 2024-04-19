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

    // Initiate functions
    Admin_Students_Enroll::populateCombobox();

    // Give function to buttons
    connect(ui->confirmButton, &QPushButton::clicked, this, &Admin_Students_Enroll::studentEnrollCall);

    connect(ui->backButton, &QPushButton::clicked, this, &Admin_Students_Enroll::switchWindow_AdminStudentsList);
    connect(ui->studentListButton, &QPushButton::clicked, this, &Admin_Students_Enroll::switchWindow_AdminStudentsList);
    connect(ui->classListButton, &QPushButton::clicked, this, &Admin_Students_Enroll::switchWindow_AdminClassesList);
    connect(ui->lecturerListButton, &QPushButton::clicked, this, &Admin_Students_Enroll::switchWindow_AdminLecturersList);

    // Connect clicked signal of the radio buttons to function
    connect(ui->regularRadio, &QRadioButton::clicked, this, [=](){ handleIsRegularRadio("regular"); });
    connect(ui->irregularRadio, &QRadioButton::clicked, this, [=](){ handleIsRegularRadio("irregular"); });

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
    QString studentId = ui->studentIdTextbox->text();
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
    QString result = verifyStudentEnroll(studentId, lastName, firstName, college, program, year, section);

    // Return error if the result is not empty
    if (!result.isEmpty())
    {
        ui->errorLabel->setText(result);
        return;
    }

    Admin_Students_Enroll::connectToDatabase(studentId, lastName, firstName, college, program, year, section);
}


QString Admin_Students_Enroll::verifyStudentEnroll(const QString &studentId, const QString &lastName, const QString &firstName,
                                                   const QString &college, const QString &program,
                                                   const QString &year, const QString &section)
{
    if (isRegular == "") { return Messages::noSelectedIsRegular(); }
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


void Admin_Students_Enroll::connectToDatabase(const QString &studentId, const QString &lastName, const QString &firstName,
                                              const QString &college, const QString &program,
                                              const QString &year, const QString &section)
{
    // qDebug() << isRegular << ", " << studentId << ", " << lastName << ", " << firstName << ", "
    //          << college << ", " << program << ", " << year << ", " << section;

    // Return error if unable to access the database
    if (!database.open())
    {
        ui->errorLabel->setText(Messages::unaccessDatabase());
        return;
    }

    // Generate random 6-digit pin
    int generatedPin = QRandomGenerator::global()->bounded(111111, 1000000);

    // Convert QString to bool in short form
    short isRegularBool = (isRegular == "true" ? 1 : 0);

    // Set up queries for database
    QSqlDatabase::database().transaction();
    QSqlQuery query(database);

    // Prepare sql command for inserting data and bind values to corresponding column
    query.prepare("INSERT INTO StudentInfo(StudentId, Pin, LastName, FirstName, College, Program, Year, Section, IsRegular) \
                  VALUES(:studentId, :pin, :lastName, :firstName, :college, :program, :year, :section, :isRegular)");
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
        ui->errorLabel->setText(Messages::errorInsertData());
        return;
    }

    QSqlDatabase::database().commit();
    database.close();

    Admin_Students_Enroll::switchWindow_AdminStudentsList();
}


void Admin_Students_Enroll::handleIsRegularRadio(const QString &option)
{
    // Return true if option selected is regular, otherwise false
    isRegular = (option == "regular" ? QString("true") : QString("false"));
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
    admin_students_List = new Admin_Students_List;
    admin_students_List->show();
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
