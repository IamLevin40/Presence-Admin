#include "global_main.h"

#include "admin_classes_list.h"
#include "ui_admin_classes_list.h"


// Include header files for forward declaration
#include "admin_login_form.h"
#include "admin_students_list.h"
#include "admin_lecturers_list.h"
#include "admin_classes_create.h"
#include "admin_classes_update.h"
#include "admin_classes_students_list.h"


Admin_Classes_List::Admin_Classes_List(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Admin_Classes_List)
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
    Admin_Classes_List::populateCombobox();
    Admin_Classes_List::filterSearchCall();
    DateTimeUtils::updateDateTimeUtils(ui->dateLabel, ui->timeLabel);

    // Connect ui objects to functions based on user interaction
    connect(ui->searchButton, &QPushButton::clicked, this, &Admin_Classes_List::filterSearchCall);
    connect(ui->searchTextbox, &QLineEdit::textChanged, this, &Admin_Classes_List::filterSearchCall);
    connect(ui->prevPageButton, &QPushButton::clicked, this, [=](){ FilteringManager::incrementPage(ui->numberPageTextbox, -1); });
    connect(ui->nextPageButton, &QPushButton::clicked, this, [=](){ FilteringManager::incrementPage(ui->numberPageTextbox, 1); });
    connect(ui->numberPageTextbox, &QLineEdit::textChanged, this, [=](){
        FilteringManager::validatePageEdit(ui->numberPageTextbox); Admin_Classes_List::filterSearchCall();
    });

    connect(ui->backButton, &QPushButton::clicked, this, &Admin_Classes_List::switchWindow_AdminLoginForm);
    connect(ui->studentListButton, &QPushButton::clicked, this, &Admin_Classes_List::switchWindow_AdminStudentsList);
    connect(ui->lecturerListButton, &QPushButton::clicked, this, &Admin_Classes_List::switchWindow_AdminLecturersList);
    connect(ui->createClassButton, &QPushButton::clicked, this, &Admin_Classes_List::switchWindow_AdminClassesCreate);

    // Load window position
    QPoint windowPos = WindowPositionManager::loadWindowPosition();
    windowPos.setY(windowPos.y() - 31);
    move(windowPos);
}


Admin_Classes_List::~Admin_Classes_List()
{
    delete ui;
}


void Admin_Classes_List::filterSearchCall()
{
    // Fetch string input text() from textboxes and comboboxes
    int pageNumber = ui->numberPageTextbox->text().toInt();
    QString schoolYear = ui->schoolYearCombobox->currentText();
    QString semester = ui->semesterCombobox->currentText();
    QString program = ui->programCombobox->currentText();
    QString year = ui->yearCombobox->currentText();
    QString section = ui->sectionCombobox->currentText();
    QString searchText = ui->searchTextbox->text().toUpper();

    // Extract unnecessary substring for data handling
    int programIndex = program.indexOf('|');
    program = program.left(programIndex).trimmed();

    // Proceed to selecting data from database
    Admin_Classes_List::selectClassInfo(pageNumber, schoolYear, semester, program, year, section, searchText);
}


void Admin_Classes_List::selectClassInfo(const int &pageNumber, const QString &schoolYear, const QString &semester,
                                         const QString &program, const QString &year, const QString &section,
                                         const QString &searchText)
{
    // Return error if unable to access the database
    if (!database.open())
    {
        GlobalTimer::displayTextForDuration(ui->errorLabel, Messages::unaccessDatabase(), 5000);
        return;
    }

    // Calculate offset for pagination
    int offset = (pageNumber - 1) * $dataLimitPerPage;

    // Set up data list and queries for database
    QList<QStringList> classDataList;
    QSqlDatabase::database().transaction();
    QSqlQuery query(database);

    // Determine search type based on the search text format
    QString searchType;
    if (!searchText.isEmpty())
    {
        searchType = "(SubjectCode LIKE :searchText OR SubjectDesc LIKE :searchText)";
    }

    // Prepare sql command for selecting data
    QString sqlQuery = "SELECT * FROM ClassInfo WHERE ";
    if (!searchText.isEmpty())
    {
        sqlQuery += searchType + " AND ";
    }
    sqlQuery += "(SchoolYear = :schoolYear OR :schoolYear IS NULL) AND \
                (Semester = :semester OR :semester IS NULL) AND \
                (Program = :program OR :program IS NULL) AND \
                (Year = :year OR :year IS NULL) AND \
                (Section = :section OR :section IS NULL) \
                ORDER BY SchoolYear, Semester DESC, SubjectCode, Program, Year, Section ASC \
                LIMIT :limit OFFSET :offset";
    query.prepare(sqlQuery);

    // Bind values to the query
    if (!searchText.isEmpty())
    {
        query.bindValue(":searchText", "%" + searchText + "%");
    }
    query.bindValue(":schoolYear", schoolYear.isEmpty() ? QVariant::fromValue(nullptr) : QVariant::fromValue(schoolYear));
    query.bindValue(":semester", semester.isEmpty() ? QVariant::fromValue(nullptr) : QVariant::fromValue(semester));
    query.bindValue(":program", program.isEmpty() ? QVariant::fromValue(nullptr) : QVariant::fromValue(program));
    query.bindValue(":year", year.isEmpty() ? QVariant::fromValue(nullptr) : QVariant::fromValue(year));
    query.bindValue(":section", section.isEmpty() ? QVariant::fromValue(nullptr) : QVariant::fromValue(section));
    query.bindValue(":limit", $dataLimitPerPage);
    query.bindValue(":offset", offset);

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
            QStringList rowData;
            rowData << query.value(0).toString();   // SubjectCode
            rowData << query.value(1).toString();   // SubjectDesc
            rowData << query.value(2).toString();   // SchoolYear
            rowData << query.value(3).toString();   // Semester
            rowData << query.value(4).toString();   // Program
            rowData << query.value(5).toString();   // Year
            rowData << query.value(6).toString();   // Section
            rowData << query.value(7).toString();   // Day
            rowData << query.value(8).toString();   // Time
            rowData << query.value(9).toString();   // Room
            rowData << query.value(10).toString();  // LecturerId

            classDataList.append(rowData);
        }
    }

    // Close the database after using
    QSqlDatabase::database().commit();
    database.close();

    // Proceed to display data list
    Admin_Classes_List::displayClassInfo(classDataList);
}


void Admin_Classes_List::displayClassInfo(const QList<QStringList> &dataList)
{
    // Clear existing QGroupBox objects
    qDeleteAll(groupBoxList);
    groupBoxList.clear();

    int posX = 0;
    int posY = 0;

    // Create objects and add data groups
    for (const auto &data : dataList)
    {
        // Assign members from dataList to variables
        QString subjectCode = data[0];
        QString subjectDesc = data[1];
        QString schoolYear = data[2];
        QString semester = data[3];
        QString program = data[4];
        QString year = data[5];
        QString section = data[6];
        QString day = data[7];
        QString time = data[8];
        QString room = data[9];
        QString lecturerId = data[10];

        // Get information from LecturerInfo with lecturerId
        QStringList lecturerInfo = Admin_Classes_List::getLecturerInfo(lecturerId);
        QString lastName = lecturerInfo[0];
        QString firstName = lecturerInfo[1];

        // List down class information for class info keys
        QStringList classInfo;
        classInfo << subjectCode;
        classInfo << program;
        classInfo << year;
        classInfo << section;
        classInfo << semester;
        classInfo << schoolYear;

        // Create data group box
        QGroupBox *dataGroup = new QGroupBox(ui->classesListArea);
        dataGroup->setGeometry(posX, posY, 310, 60);

        // Add the new QGroupBox pointer to the list
        groupBoxList.append(dataGroup);

        // Set up dataHud
        QLabel *dataHud = new QLabel(dataGroup);
        dataHud->setPixmap(QPixmap(":/res/assets/data_display.png"));
        dataHud->setGeometry(0, 0, 310, 60);

        // Set up dataSubjectCodeLabel
        QLabel *dataSubjectCodeLabel = new QLabel(dataGroup);
        dataSubjectCodeLabel->setText(StringManipulator::separateSubjectCode(subjectCode));
        dataSubjectCodeLabel->setStyleSheet("QLabel { color: #FFFFFF; font-family: Poppins; font-size: 9px; font-style: normal; font-weight: 600; line-height: normal; }");
        dataSubjectCodeLabel->setGeometry(10, 12, 45, 10);

        // Set up dataSubjectDescLabel
        QLabel *dataSubjectDescLabel = new QLabel(dataGroup);
        dataSubjectDescLabel->setText(subjectDesc);
        dataSubjectDescLabel->setStyleSheet("QLabel { color: #FFFFFF; font-family: Poppins; font-size: 9px; font-style: normal; font-weight: 600; line-height: normal; }");
        dataSubjectDescLabel->setGeometry(60, 12, 180, 10);

        // Set up dataLecturerLabel
        QLabel *dataLecturerLabel = new QLabel(dataGroup);
        dataLecturerLabel->setText(QString("%1, %2").arg(lastName, firstName));
        dataLecturerLabel->setStyleSheet("QLabel { color: #FFFFFF; font-family: Poppins; font-size: 7px; font-style: normal; font-weight: 400; line-height: normal; }");
        dataLecturerLabel->setGeometry(60, 20, 180, 10);

        // Set up dataSchoolYearLabel
        QLabel *dataSchoolYearLabel = new QLabel(dataGroup);
        dataSchoolYearLabel->setText(QString("SY %1   SEM %2").arg(schoolYear, semester));
        dataSchoolYearLabel->setStyleSheet("QLabel { color: #15CAE3; font-family: Poppins; font-size: 7px; font-style: normal; font-weight: 400; line-height: normal; }");
        dataSchoolYearLabel->setGeometry(60, 32, 180, 10);

        // Set up dataProgramLabel
        QLabel *dataProgramLabel = new QLabel(dataGroup);
        dataProgramLabel->setText(QString("%1 %2%3").arg(program, year, section));
        dataProgramLabel->setStyleSheet("QLabel { color: #15CAE3; font-family: Poppins; font-size: 7px; font-style: normal; font-weight: 400; line-height: normal; }");
        dataProgramLabel->setGeometry(10, 40, 45, 10);

        // Set up dataScheduleLabel
        QLabel *dataScheduleLabel = new QLabel(dataGroup);
        dataScheduleLabel->setText(QString("%1   %2").arg(day, time));
        dataScheduleLabel->setStyleSheet("QLabel { color: #15CAE3; font-family: Poppins; font-size: 7px; font-style: normal; font-weight: 400; line-height: normal; }");
        dataScheduleLabel->setGeometry(60, 40, 180, 10);

        // Set up dataRoomLabel
        QLabel *dataRoomLabel = new QLabel(dataGroup);
        dataRoomLabel->setText(room);
        dataRoomLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        dataRoomLabel->setStyleSheet("QLabel { color: #15CAE3; font-family: Poppins; font-size: 7px; font-style: normal; font-weight: 400; line-height: normal; }");
        dataRoomLabel->setGeometry(240, 40, 60, 10);

        // Set up dataListButton
        QPushButton *dataListButton = new QPushButton(dataGroup);
        dataListButton->setIcon(QIcon(":res/assets/list.png"));
        dataListButton->setStyleSheet("QPushButton { border: 0px; border-radius: 0px; background: none; }");
        dataListButton->setGeometry(245, 10, 15, 15);
        connect(dataListButton, &QPushButton::clicked, this, [=]() {
            Admin_Classes_List::switchWindow_AdminClassesStudentsList(classInfo);
        });

        // Set up dataEditButton
        QPushButton *dataEditButton = new QPushButton(dataGroup);
        dataEditButton->setIcon(QIcon(":/res/assets/edit.png"));
        dataEditButton->setStyleSheet("QPushButton { border: 0px; border-radius: 0px; background: none; }");
        dataEditButton->setGeometry(265, 10, 15, 15);
        connect(dataEditButton, &QPushButton::clicked, this, [=]() {
            Admin_Classes_List::switchWindow_AdminClassesUpdate(classInfo);
        });

        // Set up dataDeleteButton
        QPushButton *dataDeleteButton = new QPushButton(dataGroup);
        dataDeleteButton->setIcon(QIcon(":/res/assets/delete.png"));
        dataDeleteButton->setStyleSheet("QPushButton { border: 0px; border-radius: 0px; background: none; }");
        dataDeleteButton->setGeometry(285, 10, 15, 15);
        connect(dataDeleteButton, &QPushButton::clicked, this, [=]() {
            Admin_Classes_List::displayDeleteWarning(classInfo);
        });

        // Add data group box to contents layout
        posY += dataGroup->height() + 5;
        dataGroup->setVisible(true);
        dataGroup->raise();
    }
}


void Admin_Classes_List::displayDeleteWarning(const QStringList &keys_classInfo)
{
    // Find and delete the existing group box
    QGroupBox *existingGroupBox = ui->centralwidget->findChild<QGroupBox*>("deleteWarningGroup");
    if (existingGroupBox) { delete existingGroupBox; }

    // Create deleteWarningGroup
    QGroupBox *deleteWarningGroup = new QGroupBox(ui->centralwidget);
    deleteWarningGroup->setObjectName("deleteWarningGroup");
    deleteWarningGroup->setStyleSheet("QGroupBox { border: 0px; border-radius: 0px; background-color: rgba(255, 255, 255, 128); }");
    deleteWarningGroup->setGeometry(0, 95, 360, 420);

    // Set up deleteWarningBox
    QGroupBox *deleteWarningBox = new QGroupBox(deleteWarningGroup);
    deleteWarningBox->setStyleSheet("QGroupBox { border: 0px; border-radius: 0px; background: none; }");
    deleteWarningBox->setGeometry(55, 110, 250, 110);

    // Set up deleteWarningLabel
    QLabel *deleteWarningLabel = new QLabel(deleteWarningBox);
    deleteWarningLabel->setPixmap(QPixmap(":/res/assets/delete_warning.png"));
    deleteWarningLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    deleteWarningLabel->setGeometry(0, 0, 250, 90);

    // Set up deleteYesButton
    QPushButton *deleteYesButton = new QPushButton(deleteWarningBox);
    deleteYesButton->setIcon(QIcon(":/res/assets/yes.png"));
    deleteYesButton->setIconSize(QSize(90, 40));
    deleteYesButton->setStyleSheet("QPushButton { border: 0px; border-radius: 0px; background: none; }");
    deleteYesButton->setGeometry(35, 60, 80, 40);
    connect(deleteYesButton, &QPushButton::clicked, this, [=]() {
        Admin_Classes_List::deleteClassInfo(keys_classInfo);
    });

    // Set up deleteNoButton
    QPushButton *deleteNoButton = new QPushButton(deleteWarningBox);
    deleteNoButton->setIcon(QIcon(":/res/assets/no.png"));
    deleteNoButton->setIconSize(QSize(90, 40));
    deleteNoButton->setStyleSheet("QPushButton { border: 0px; border-radius: 0px; background: none; }");
    deleteNoButton->setGeometry(135, 60, 80, 40);
    connect(deleteNoButton, &QPushButton::clicked, this, Admin_Classes_List::disregardDelete);

    deleteWarningGroup->setVisible(true);
    deleteWarningGroup->raise();
}


void Admin_Classes_List::deleteClassInfo(const QStringList &keys_classInfo)
{
    // Return error if unable to access the database
    if (!database.open())
    {
        GlobalTimer::displayTextForDuration(ui->errorLabel, Messages::unaccessDatabase(), 5000);
        return;
    }

    // Assign members from dataList to variables
    QString key_subjectCode = keys_classInfo[0];
    QString key_program = keys_classInfo[1];
    QString key_year = keys_classInfo[2];
    QString key_section = keys_classInfo[3];
    QString key_semester = keys_classInfo[4];
    QString key_schoolYear = keys_classInfo[5];

    // Set up queries for database
    QSqlDatabase::database().transaction();
    QSqlQuery query(database);

    // Prepare sql command for deleting data
    query.prepare("DELETE FROM ClassInfo WHERE \
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

    // Return error if unable to delete data
    if (!query.exec())
    {
        QSqlDatabase::database().rollback();
        GlobalTimer::displayTextForDuration(ui->errorLabel, Messages::errorDeleteData(), 5000);
        return;
    }
    query.clear();

    // Delete table after successfully deleting data
    QString tableName = QString("%1%2%3%4_S%5SY%6").arg(key_subjectCode, key_program, key_year, key_section, key_semester, StringManipulator::convertSchoolYear(key_schoolYear));

    QString queryString = "DROP TABLE " + tableName;
    query.prepare(queryString);

    // Return error if unable to delete table
    if (!query.exec())
    {
        qDebug() << query.lastError().text();
        QSqlDatabase::database().rollback();
        GlobalTimer::displayTextForDuration(ui->errorLabel, Messages::errorDeleteTable(), 5000);
        return;
    }

    // Close the database after using
    QSqlDatabase::database().commit();
    database.close();

    Admin_Classes_List::disregardDelete();
    Admin_Classes_List::filterSearchCall();
}


void Admin_Classes_List::disregardDelete()
{
    // Find and delete the existing group box
    QGroupBox *existingGroupBox = ui->centralwidget->findChild<QGroupBox*>("deleteWarningGroup");
    if (existingGroupBox) { delete existingGroupBox; }
}


void Admin_Classes_List::populateCombobox()
{
    // Populate comboboxes with items
    ui->schoolYearCombobox->addItems(Schedules::$schoolYears);
    ui->semesterCombobox->addItems(Schedules::$semesters);
    ui->programCombobox->addItems(Colleges::$programs);
    ui->yearCombobox->addItems(Colleges::$years);
    ui->sectionCombobox->addItems(Colleges::$sections);

    // Connect comboboxes to checkComboboxIndex() and filterSearchCall()
    connect(ui->schoolYearCombobox, &QComboBox::currentIndexChanged, this, [=](){
        FilteringManager::checkComboboxIndex(ui->schoolYearCombobox); Admin_Classes_List::filterSearchCall();
    });
    connect(ui->semesterCombobox, &QComboBox::currentIndexChanged, this, [=](){
        FilteringManager::checkComboboxIndex(ui->semesterCombobox); Admin_Classes_List::filterSearchCall();
    });
    connect(ui->programCombobox, &QComboBox::currentIndexChanged, this, [=](){
        FilteringManager::checkComboboxIndex(ui->programCombobox); Admin_Classes_List::filterSearchCall();
    });
    connect(ui->yearCombobox, &QComboBox::currentIndexChanged, this, [=](){
        FilteringManager::checkComboboxIndex(ui->yearCombobox); Admin_Classes_List::filterSearchCall();
    });
    connect(ui->sectionCombobox, &QComboBox::currentIndexChanged, this, [=](){
        FilteringManager::checkComboboxIndex(ui->sectionCombobox); Admin_Classes_List::filterSearchCall();
    });
}


QStringList Admin_Classes_List::getLecturerInfo(const QString &lecturerId)
{
    // Return error if unable to access the database
    if (!database.open())
    {
        GlobalTimer::displayTextForDuration(ui->errorLabel, Messages::unaccessDatabase(), 5000);
    }

    // Set up data list and queries for database
    QStringList lecturerInfo;
    QSqlDatabase::database().transaction();
    QSqlQuery query(database);

    // Prepare sql command for selecting data
    query.prepare("SELECT LastName, FirstName FROM LecturerInfo WHERE LecturerId = :lecturerId");

    // Bind values to the query
    query.bindValue(":lecturerId", lecturerId);

    // Return error if unable to select data
    if (!query.exec())
    {
        QSqlDatabase::database().rollback();
        GlobalTimer::displayTextForDuration(ui->errorLabel, Messages::errorSelectData(), 5000);
    }

    query.next();
    lecturerInfo << query.value(0).toString();  // LastName
    lecturerInfo << query.value(1).toString();  // FirstName

    return lecturerInfo;
}


void Admin_Classes_List::switchWindow_AdminLoginForm()
{
    // Switch ui window to Admin_Login_Form
    admin_login_form = new Admin_Login_Form;
    admin_login_form->show();
    this->hide();
}


void Admin_Classes_List::switchWindow_AdminStudentsList()
{
    // Switch ui window to Admin_Students_List
    admin_students_list = new Admin_Students_List;
    admin_students_list->show();
    this->hide();
}


void Admin_Classes_List::switchWindow_AdminLecturersList()
{
    // Switch ui window to Admin_Lecturers_List
    admin_lecturers_list = new Admin_Lecturers_List;
    admin_lecturers_list->show();
    this->hide();
}


void Admin_Classes_List::switchWindow_AdminClassesCreate()
{
    // Switch ui window to Admin_Classes_Create
    admin_classes_create = new Admin_Classes_Create;
    admin_classes_create->show();
    this->hide();
}


void Admin_Classes_List::switchWindow_AdminClassesUpdate(const QStringList &updateKeys_ClassInfo)
{
    // Set primary key for updating class info
    $updateKeys_ClassInfo = updateKeys_ClassInfo;

    // Switch ui window to Admin_Classes_Update
    admin_classes_update = new Admin_Classes_Update;
    admin_classes_update->show();
    this->hide();
}


void Admin_Classes_List::switchWindow_AdminClassesStudentsList(const QStringList &selectKeys_ClassInfo)
{
    // Set primary key for selecting class info
    $selectKeys_ClassInfo = selectKeys_ClassInfo;

    // Switch ui window to Admin_Classes_Students_List
    admin_classes_students_list = new Admin_Classes_Students_List;
    admin_classes_students_list->show();
    this->hide();
}


void Admin_Classes_List::moveEvent(QMoveEvent *event)
{
    // Saves window position every movement instance of a window
    QMainWindow::moveEvent(event);
    WindowPositionManager::saveWindowPosition(event->pos());
}


/*
 *   Copyright (c) 2024 - WVSU-CICT BSCS 1B: Batch 2027
 *   All rights reserved.
 */
