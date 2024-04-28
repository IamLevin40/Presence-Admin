#include "global_main.h"

#include "admin_classes_students_list.h"
#include "ui_admin_classes_students_list.h"


// Include header files for forward declaration
#include "admin_students_list.h"
#include "admin_classes_list.h"
#include "admin_lecturers_list.h"
#include "admin_classes_students_add.h"


Admin_Classes_Students_List::Admin_Classes_Students_List(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Admin_Classes_Students_List)
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
    Admin_Classes_Students_List::selectInfoFromDatabase($selectKeys_ClassInfo);
    Admin_Classes_Students_List::deleteUnknownFromDatabase($selectKeys_ClassInfo);
    Admin_Classes_Students_List::filterSearchCall();
    DateTimeUtils::updateDateTimeUtils(ui->dateLabel, ui->timeLabel);

    // Connect ui objects to functions based on user interaction
    connect(ui->prevPageButton, &QPushButton::clicked, this, [=](){ FilteringManager::incrementPage(ui->numberPageTextbox, -1); });
    connect(ui->nextPageButton, &QPushButton::clicked, this, [=](){ FilteringManager::incrementPage(ui->numberPageTextbox, 1); });
    connect(ui->numberPageTextbox, &QLineEdit::textChanged, this, [=](){
        FilteringManager::validatePageEdit(ui->numberPageTextbox); Admin_Classes_Students_List::filterSearchCall();
    });

    connect(ui->backButton, &QPushButton::clicked, this, &Admin_Classes_Students_List::switchWindow_AdminClassesList);
    connect(ui->studentListButton, &QPushButton::clicked, this, &Admin_Classes_Students_List::switchWindow_AdminStudentsList);
    connect(ui->classListButton, &QPushButton::clicked, this, &Admin_Classes_Students_List::switchWindow_AdminClassesList);
    connect(ui->lecturerListButton, &QPushButton::clicked, this, &Admin_Classes_Students_List::switchWindow_AdminLecturersList);
    connect(ui->addStudentButton, &QPushButton::clicked, this, &Admin_Classes_Students_List::switchWindow_AdminClassesStudentsAdd);

    // Load window position
    QPoint windowPos = WindowPositionManager::loadWindowPosition();
    windowPos.setY(windowPos.y() - 31);
    move(windowPos);
}


Admin_Classes_Students_List::~Admin_Classes_Students_List()
{
    delete ui;
}


void Admin_Classes_Students_List::selectInfoFromDatabase(const QStringList &keys_classInfo)
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

    // Prepare sql command for selecting data
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
    Admin_Classes_Students_List::displayInfoFromDatabase(classDataList);
}


void Admin_Classes_Students_List::displayInfoFromDatabase(const QStringList &dataList)
{
    // Assign members from dataList to variables
    QString subjectCode = dataList[0];
    QString subjectDesc = dataList[1];
    QString schoolYear = dataList[2];
    QString semester = dataList[3];
    QString program = dataList[4];
    QString year = dataList[5];
    QString section = dataList[6];
    QString day = dataList[7];
    QString time = dataList[8];
    QString room = dataList[9];
    QString lecturerId = dataList[10];

    // Get information from LecturerInfo with lecturerId
    QStringList lecturerInfo = Admin_Classes_Students_List::getLecturerInfo(lecturerId);
    QString lastName = lecturerInfo[0];
    QString firstName = lecturerInfo[1];

    // Display class information on top of students data
    ui->subjectCodeLabel->setText(subjectCode);
    ui->subjectDescLabel->setText(subjectDesc);
    ui->lecturerNameLabel->setText(QString("%1, %2").arg(lastName, firstName));
    ui->schoolYearLabel->setText(QString("SY %1   SEM %2").arg(schoolYear, semester));
    ui->programLabel->setText(QString("%1 %2%3").arg(program, year, section));
    ui->scheduleLabel->setText(QString("%1   %2").arg(day, time));
    ui->roomLabel->setText(room);
}


QStringList Admin_Classes_Students_List::getLecturerInfo(const QString &lecturerId)
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


void Admin_Classes_Students_List::deleteUnknownFromDatabase(const QStringList &keys_classInfo)
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

    // Set up data list and queries for database
    QStringList studentDataList;
    QSqlDatabase::database().transaction();
    QSqlQuery query(database);

    // Prepare sql command for selecting data
    query.prepare("SELECT * FROM " + tableName);

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
            studentDataList << query.value(0).toString();   // StudentId
        }
    }
    query.clear();

    for (const auto &studentId : studentDataList)
    {
        // Prepare sql command for finding data
        query.prepare("SELECT COUNT(*) FROM StudentInfo WHERE StudentId = :studentId");

        // Bind values to the query
        query.bindValue(":studentId", studentId);

        // Return error if unable to insert data
        if (!query.exec())
        {
            QSqlDatabase::database().rollback();
            GlobalTimer::displayTextForDuration(ui->errorLabel, Messages::errorSelectData(), 5000);
            return;
        }

        query.next();
        int count = query.value(0).toInt();
        bool isStudentIdExist = count > 0;

        query.clear();
        if (!isStudentIdExist)
        {
            // Prepare sql command for deleting data
            query.prepare("DELETE FROM " + tableName + " WHERE StudentId = :studentId");

            // Bind values to the query
            query.bindValue(":studentId", studentId);

            // Return error if unable to delete data
            if (!query.exec())
            {
                QSqlDatabase::database().rollback();
                GlobalTimer::displayTextForDuration(ui->errorLabel, Messages::errorDeleteData(), 5000);
                return;
            }

            query.clear();
        }
    }

    // Close the database after using
    QSqlDatabase::database().commit();
    database.close();
}


void Admin_Classes_Students_List::filterSearchCall()
{
    // Fetch string input text() from textboxes
    int pageNumber = ui->numberPageTextbox->text().toInt();
    QStringList keys_classInfo = $selectKeys_ClassInfo;

    // Proceed to selecting data from database
    Admin_Classes_Students_List::selectDataFromDatabase(pageNumber, keys_classInfo);
}


void Admin_Classes_Students_List::selectDataFromDatabase(const int &pageNumber, const QStringList &keys_classInfo)
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

    // Calculate offset for pagination
    int offset = (pageNumber - 1) * $dataLimitPerPage;

    // Set up data list and queries for database
    QList<QStringList> studentRecordList;
    QList<QStringList> studentDataList;
    QSqlDatabase::database().transaction();
    QSqlQuery query(database);

    // Prepare sql command for selecting data
    query.prepare("SELECT * FROM " + tableName + " LIMIT :limit OFFSET :offset");

    // Bind values to the query
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
            rowData << query.value(0).toString();   // StudentId
            rowData << query.value(1).toString();   // PresentCount
            rowData << query.value(2).toString();   // AbsentCount

            studentRecordList.append(rowData);
        }
    }
    query.clear();

    for (const auto &student : studentRecordList)
    {
        QString studentId = student[0];

        // Prepare sql command for selecting data
        query.prepare("SELECT * FROM StudentInfo WHERE StudentId = :studentId");

        // Bind values to the query
        query.bindValue(":studentId", studentId);

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
            query.next();

            QStringList rowData;
            rowData << query.value(0).toString();   // StudentId
            rowData << query.value(1).toString();   // Pin
            rowData << query.value(2).toString();   // LastName
            rowData << query.value(3).toString();   // FirstName
            rowData << query.value(4).toString();   // College
            rowData << query.value(5).toString();   // Program
            rowData << query.value(6).toString();   // Year
            rowData << query.value(7).toString();   // Section
            rowData << query.value(8).toString();   // IsRegular

            studentDataList.append(rowData);
        }

        query.clear();
    }

    // Close the database after using
    QSqlDatabase::database().commit();
    database.close();

    // Proceed to display data list
    Admin_Classes_Students_List::displayDataFromDatabase(studentDataList, studentRecordList);
}


void Admin_Classes_Students_List::displayDataFromDatabase(const QList<QStringList> &dataList, const QList<QStringList> &recordList)
{
    // Clear existing QGroupBox objects
    qDeleteAll(groupBoxList);
    groupBoxList.clear();

    int posX = 0;
    int posY = 0;

    // Create objects and add data groups
    int sizeList = recordList.size();
    for (int i = 0; i < sizeList; i++)
    {
        // Assign members from dataList to variables
        const QStringList &data = dataList.at(i);
        const QStringList &record = recordList.at(i);

        QString studentId = record.at(0);
        QString presentCount = record.at(1);
        QString absentCount = record.at(2);

        QString lastName = data.at(2);
        QString firstName = data.at(3);
        QString college = data.at(4);
        QString program = data.at(5);
        QString year = data.at(6);
        QString section = data.at(7);
        QString isRegular = data.at(8);

        // Create data group box
        QGroupBox *dataGroup = new QGroupBox(ui->classesStudentsListArea);
        dataGroup->setGeometry(posX, posY, 310, 60);

        // Add the new QGroupBox pointer to the list
        groupBoxList.append(dataGroup);

        // Set up dataHud
        QLabel *dataHud = new QLabel(dataGroup);
        dataHud->setPixmap(QPixmap(":/res/assets/data_display.png"));
        dataHud->setGeometry(0, 0, 310, 60);

        // Set up dataAvatarLabel
        QLabel *dataAvatarLabel = new QLabel(dataGroup);
        dataAvatarLabel->setPixmap(QPixmap(QString(":/res/assets/%1_avatar.png").arg(college.toLower())));
        dataAvatarLabel->setGeometry(10, 10, 40, 40);

        // Set up dataNameLabel
        QLabel *dataNameLabel = new QLabel(dataGroup);
        dataNameLabel->setText(QString("%1, %2").arg(lastName, firstName));
        dataNameLabel->setStyleSheet("QLabel { color: #FFFFFF; font-family: Poppins; font-size: 12px; font-style: normal; font-weight: 600; line-height: normal; }");
        dataNameLabel->setGeometry(60, 12, 180, 10);

        // Set up dataProgramLabel
        QLabel *dataProgramLabel = new QLabel(dataGroup);
        dataProgramLabel->setText(QString("%1 - %2 %3%4").arg(college, program, year, section));
        dataProgramLabel->setStyleSheet("QLabel { color: #FFFFFF; font-family: Poppins; font-size: 9px; font-style: normal; font-weight: 400; line-height: normal; }");
        dataProgramLabel->setGeometry(60, 25, 180, 10);

        // Set up dataStudentIdLabel
        QLabel *dataStudentIdLabel = new QLabel(dataGroup);
        dataStudentIdLabel->setText(studentId);
        dataStudentIdLabel->setStyleSheet("QLabel { color: #15CAE3; font-family: Poppins; font-size: 9px; font-style: normal; font-weight: 400; line-height: normal; }");
        dataStudentIdLabel->setGeometry(60, 40, 70, 10);

        // Set up dataIsRegularLabel
        QLabel *dataIsRegularLabel = new QLabel(dataGroup);
        dataIsRegularLabel->setText(isRegular == "1" ? "Regular" : "Irregular");
        dataIsRegularLabel->setStyleSheet("QLabel { color: #15CAE3; font-family: Poppins; font-size: 9px; font-style: normal; font-weight: 400; line-height: normal; }");
        dataIsRegularLabel->setGeometry(130, 40, 70, 10);

        // Set up dataPresentTitleLabel
        QLabel *dataPresentTitleLabel = new QLabel(dataGroup);
        dataPresentTitleLabel->setText("Present   |");
        dataPresentTitleLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        dataPresentTitleLabel->setStyleSheet("QLabel { color: #FFFFFF; font-family: Poppins; font-size: 9px; font-style: normal; font-weight: 400; line-height: normal; }");
        dataPresentTitleLabel->setGeometry(225, 30, 45, 10);

        // Set up dataAbsentTitleLabel
        QLabel *dataAbsentTitleLabel = new QLabel(dataGroup);
        dataAbsentTitleLabel->setText("Absent   |");
        dataAbsentTitleLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        dataAbsentTitleLabel->setStyleSheet("QLabel { color: #FFFFFF; font-family: Poppins; font-size: 9px; font-style: normal; font-weight: 400; line-height: normal; }");
        dataAbsentTitleLabel->setGeometry(225, 40, 45, 10);

        // Set up dataPresentCountLabel
        QLabel *dataPresentCountLabel = new QLabel(dataGroup);
        dataPresentCountLabel->setText(presentCount);
        dataPresentCountLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        dataPresentCountLabel->setStyleSheet("QLabel { color: #FFFFFF; font-family: Poppins; font-size: 9px; font-style: normal; font-weight: 400; line-height: normal; }");
        dataPresentCountLabel->setGeometry(275, 30, 25, 10);

        // Set up dataAbsentCountLabel
        QLabel *dataAbsentCountLabel = new QLabel(dataGroup);
        dataAbsentCountLabel->setText(absentCount);
        dataAbsentCountLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        dataAbsentCountLabel->setStyleSheet("QLabel { color: #FFFFFF; font-family: Poppins; font-size: 9px; font-style: normal; font-weight: 400; line-height: normal; }");
        dataAbsentCountLabel->setGeometry(275, 40, 25, 10);

        // Set up dataDeleteButton
        QPushButton *dataDeleteButton = new QPushButton(dataGroup);
        dataDeleteButton->setIcon(QIcon(":/res/assets/delete.png"));
        dataDeleteButton->setStyleSheet("QPushButton { border: 0px; border-radius: 0px; background: none; }");
        dataDeleteButton->setGeometry(285, 10, 15, 15);
        connect(dataDeleteButton, &QPushButton::clicked, this, [=]() {
            Admin_Classes_Students_List::deleteDataFromDatabase(studentId);
        });

        // Add data group box to contents layout
        posY += dataGroup->height() + 5;
        dataGroup->setVisible(true);
        dataGroup->raise();
    }
}


void Admin_Classes_Students_List::deleteDataFromDatabase(const QString &studentId)
{
    // Return error if unable to access the database
    if (!database.open())
    {
        GlobalTimer::displayTextForDuration(ui->errorLabel, Messages::unaccessDatabase(), 5000);
        return;
    }

    // Separate keys onto different key variables
    QString key_subjectCode = $selectKeys_ClassInfo[0];
    QString key_program = $selectKeys_ClassInfo[1];
    QString key_year = $selectKeys_ClassInfo[2];
    QString key_section = $selectKeys_ClassInfo[3];
    QString key_semester = $selectKeys_ClassInfo[4];
    QString key_schoolYear = $selectKeys_ClassInfo[5];

    QString tableName = QString("%1%2%3%4_S%5SY%6").arg(key_subjectCode, key_program, key_year, key_section, key_semester, FilteringManager::convertSchoolYear(key_schoolYear));

    // Set up queries for database
    QSqlDatabase::database().transaction();
    QSqlQuery query(database);

    // Prepare sql command for deleting data
    query.prepare("DELETE FROM " + tableName + " WHERE StudentId = :studentId");

    // Bind values to the query
    query.bindValue(":studentId", studentId);

    // Return error if unable to delete data
    if (!query.exec())
    {
        QSqlDatabase::database().rollback();
        GlobalTimer::displayTextForDuration(ui->errorLabel, Messages::errorDeleteData(), 5000);
        return;
    }

    // Close the database after using
    QSqlDatabase::database().commit();
    database.close();

    Admin_Classes_Students_List::filterSearchCall();
}


void Admin_Classes_Students_List::switchWindow_AdminStudentsList()
{
    // Switch ui window to Admin_Students_List
    admin_students_list = new Admin_Students_List;
    admin_students_list->show();
    this->hide();
}


void Admin_Classes_Students_List::switchWindow_AdminClassesList()
{
    // Switch ui window to Admin_Classes_List
    admin_classes_list = new Admin_Classes_List;
    admin_classes_list->show();
    this->hide();
}


void Admin_Classes_Students_List::switchWindow_AdminLecturersList()
{
    // Switch ui window to Admin_Lecturers_List
    admin_lecturers_list = new Admin_Lecturers_List;
    admin_lecturers_list->show();
    this->hide();
}


void Admin_Classes_Students_List::switchWindow_AdminClassesStudentsAdd()
{
    // Switch ui window to Admin_Classes_Students_Add
    admin_classes_students_add = new Admin_Classes_Students_Add;
    admin_classes_students_add->show();
    this->hide();
}


void Admin_Classes_Students_List::moveEvent(QMoveEvent *event)
{
    // Saves window position every movement instance of a window
    QMainWindow::moveEvent(event);
    WindowPositionManager::saveWindowPosition(event->pos());
}
