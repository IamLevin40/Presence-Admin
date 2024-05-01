#include "global_main.h"

#include "admin_students_list.h"
#include "ui_admin_students_list.h"


// Include header files for forward declaration
#include "admin_login_form.h"
#include "admin_classes_list.h"
#include "admin_lecturers_list.h"
#include "admin_students_enroll.h"
#include "admin_students_update.h"


Admin_Students_List::Admin_Students_List(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Admin_Students_List)
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
    Admin_Students_List::populateCombobox();
    Admin_Students_List::filterSearchCall();
    DateTimeUtils::updateDateTimeUtils(ui->dateLabel, ui->timeLabel);

    // Connect ui objects to functions based on user interaction
    connect(ui->searchButton, &QPushButton::clicked, this, &Admin_Students_List::filterSearchCall);
    connect(ui->searchTextbox, &QLineEdit::textChanged, this, &Admin_Students_List::filterSearchCall);
    connect(ui->prevPageButton, &QPushButton::clicked, this, [=](){ FilteringManager::incrementPage(ui->numberPageTextbox, -1); });
    connect(ui->nextPageButton, &QPushButton::clicked, this, [=](){ FilteringManager::incrementPage(ui->numberPageTextbox, 1); });
    connect(ui->numberPageTextbox, &QLineEdit::textChanged, this, [=](){
        FilteringManager::validatePageEdit(ui->numberPageTextbox); Admin_Students_List::filterSearchCall();
    });

    connect(ui->backButton, &QPushButton::clicked, this, &Admin_Students_List::switchWindow_AdminLoginForm);
    connect(ui->classListButton, &QPushButton::clicked, this, &Admin_Students_List::switchWindow_AdminClassesList);
    connect(ui->lecturerListButton, &QPushButton::clicked, this, &Admin_Students_List::switchWindow_AdminLecturersList);
    connect(ui->enrollStudentButton, &QPushButton::clicked, this, &Admin_Students_List::switchWindow_AdminStudentsEnroll);

    // Load window position
    QPoint windowPos = WindowPositionManager::loadWindowPosition();
    windowPos.setY(windowPos.y() - 31);
    move(windowPos);
}


Admin_Students_List::~Admin_Students_List()
{
    delete ui;
}


void Admin_Students_List::filterSearchCall()
{
    // Fetch string input text() from textboxes and comboboxes
    int pageNumber = ui->numberPageTextbox->text().toInt();
    QString college = ui->collegeCombobox->currentText();
    QString program = ui->programCombobox->currentText();
    QString year = ui->yearCombobox->currentText();
    QString section = ui->sectionCombobox->currentText();
    QString isRegular = ui->isRegularCombobox->currentText();
    QString searchText = ui->searchTextbox->text().toUpper();

    // Extract unnecessary substring for data handling
    int collegeIndex = college.indexOf('|');
    int programIndex = program.indexOf('|');
    college = college.left(collegeIndex).trimmed();
    program = program.left(programIndex).trimmed();

    // Proceed to selecting data from database
    Admin_Students_List::selectStudentInfo(pageNumber, college, program, year, section, isRegular, searchText);
}


void Admin_Students_List::selectStudentInfo(const int &pageNumber, const QString &college, const QString &program,
                                            const QString &year, const QString &section, const QString &isRegular,
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
    QList<QStringList> studentDataList;
    QSqlDatabase::database().transaction();
    QSqlQuery query(database);

    // Determine search type based on the search text format
    QString searchType;
    if (!searchText.isEmpty())
    {
        QChar firstChar = searchText.at(0);
        if (firstChar.isDigit()) { searchType = "StudentId LIKE :searchText"; }
        else { searchType = "(LastName LIKE :searchText OR FirstName LIKE :searchText)"; }
    }

    // Prepare sql command for selecting data
    QString sqlQuery = "SELECT * FROM StudentInfo WHERE ";
    if (!searchText.isEmpty())
    {
        sqlQuery += searchType + " AND ";
    }
    sqlQuery += "(College = :college OR :college IS NULL) AND \
                (Program = :program OR :program IS NULL) AND \
                (Year = :year OR :year IS NULL) AND \
                (Section = :section OR :section IS NULL) AND \
                (IsRegular = :isRegular OR :isRegular IS NULL) \
                ORDER BY LastName, FirstName ASC \
                LIMIT :limit OFFSET :offset";
    query.prepare(sqlQuery);

    // Bind values to the query
    if (!searchText.isEmpty())
    {
        query.bindValue(":searchText", "%" + searchText + "%");
    }
    query.bindValue(":college", college.isEmpty() ? QVariant::fromValue(nullptr) : QVariant::fromValue(college));
    query.bindValue(":program", program.isEmpty() ? QVariant::fromValue(nullptr) : QVariant::fromValue(program));
    query.bindValue(":year", year.isEmpty() ? QVariant::fromValue(nullptr) : QVariant::fromValue(year));
    query.bindValue(":section", section.isEmpty() ? QVariant::fromValue(nullptr) : QVariant::fromValue(section));
    query.bindValue(":isRegular", isRegular.isEmpty() ? QVariant::fromValue(nullptr) : QVariant::fromValue(isRegular == "Regular" ? 1 : 0));
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
    }

    // Close the database after using
    QSqlDatabase::database().commit();
    database.close();

    // Proceed to display data list
    Admin_Students_List::displayStudentInfo(studentDataList);
}


void Admin_Students_List::displayStudentInfo(const QList<QStringList> &dataList)
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
        QString studentId = data[0];
        QString pin = data[1];
        QString lastName = data[2];
        QString firstName = data[3];
        QString college = data[4];
        QString program = data[5];
        QString year = data[6];
        QString section = data[7];
        QString isRegular = data[8];

        // Create data group box
        QGroupBox *dataGroup = new QGroupBox(ui->studentsListArea);
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

        // Set up dataPinLabel
        QLabel *dataPinLabel = new QLabel(dataGroup);
        dataPinLabel->setText(pin);
        dataPinLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        dataPinLabel->setStyleSheet("QLabel { color: #15CAE3; font-family: Poppins; font-size: 9px; font-style: normal; font-weight: 400; line-height: normal; }");
        dataPinLabel->setGeometry(240, 40, 60, 10);

        // Set up dataEditButton
        QPushButton *dataEditButton = new QPushButton(dataGroup);
        dataEditButton->setIcon(QIcon(":/res/assets/edit.png"));
        dataEditButton->setStyleSheet("QPushButton { border: 0px; border-radius: 0px; background: none; }");
        dataEditButton->setGeometry(265, 10, 15, 15);
        connect(dataEditButton, &QPushButton::clicked, this, [=]() {
            Admin_Students_List::switchWindow_AdminStudentsUpdate(studentId);
        });

        // Set up dataDeleteButton
        QPushButton *dataDeleteButton = new QPushButton(dataGroup);
        dataDeleteButton->setIcon(QIcon(":/res/assets/delete.png"));
        dataDeleteButton->setStyleSheet("QPushButton { border: 0px; border-radius: 0px; background: none; }");
        dataDeleteButton->setGeometry(285, 10, 15, 15);
        connect(dataDeleteButton, &QPushButton::clicked, this, [=]() {
            Admin_Students_List::deleteStudentInfo(studentId);
        });

        // Add data group box to contents layout
        posY += dataGroup->height() + 5;
        dataGroup->setVisible(true);
        dataGroup->raise();
    }
}


void Admin_Students_List::deleteStudentInfo(const QString &key_studentId)
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

    // Prepare sql command for deleting data
    query.prepare("DELETE FROM StudentInfo WHERE StudentId = :studentId");

    // Bind values to the query
    query.bindValue(":studentId", key_studentId);

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

    Admin_Students_List::filterSearchCall();
}


void Admin_Students_List::populateCombobox()
{
    // Populate comboboxes with items
    ui->collegeCombobox->addItems(Colleges::$colleges);
    ui->programCombobox->setDisabled(true);
    ui->yearCombobox->addItems(Colleges::$years);
    ui->sectionCombobox->addItems(Colleges::$sections);
    ui->isRegularCombobox->addItems(Colleges::$isRegular);

    // Connect collegeComboBox's currentIndexChanged signal to collegeIndexChanged()
    connect(ui->collegeCombobox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &Admin_Students_List::collegeIndexChanged);

    // Connect comboboxes to checkComboboxIndex() and filterSearchCall()
    connect(ui->collegeCombobox, &QComboBox::currentIndexChanged, this, [=](){
        FilteringManager::checkComboboxIndex(ui->collegeCombobox); Admin_Students_List::filterSearchCall();
    });
    connect(ui->programCombobox, &QComboBox::currentIndexChanged, this, [=](){
        FilteringManager::checkComboboxIndex(ui->programCombobox); Admin_Students_List::filterSearchCall();
    });
    connect(ui->yearCombobox, &QComboBox::currentIndexChanged, this, [=](){
        FilteringManager::checkComboboxIndex(ui->yearCombobox); Admin_Students_List::filterSearchCall();
    });
    connect(ui->sectionCombobox, &QComboBox::currentIndexChanged, this, [=](){
        FilteringManager::checkComboboxIndex(ui->sectionCombobox); Admin_Students_List::filterSearchCall();
    });
    connect(ui->isRegularCombobox, &QComboBox::currentIndexChanged, this, [=](){
        FilteringManager::checkComboboxIndex(ui->isRegularCombobox); Admin_Students_List::filterSearchCall();
    });
}


void Admin_Students_List::collegeIndexChanged(int index)
{
    // Fetch selected college and find corresponding programs for that
    const QString college = ui->collegeCombobox->itemText(index);
    const QStringList programs = Colleges::$offeredPrograms.value(college);

    // Clear, add, and enable/disable combobox based on whether programs are available
    ui->programCombobox->clear();
    ui->programCombobox->addItems(programs);
    ui->programCombobox->setEnabled(!programs.isEmpty());
}


void Admin_Students_List::switchWindow_AdminLoginForm()
{
    // Switch ui window to Admin_Login_Form
    admin_login_form = new Admin_Login_Form;
    admin_login_form->show();
    this->hide();
}


void Admin_Students_List::switchWindow_AdminClassesList()
{
    // Switch ui window to Admin_Classes_List
    admin_classes_list = new Admin_Classes_List;
    admin_classes_list->show();
    this->hide();
}


void Admin_Students_List::switchWindow_AdminLecturersList()
{
    // Switch ui window to Admin_Lecturers_List
    admin_lecturers_list = new Admin_Lecturers_List;
    admin_lecturers_list->show();
    this->hide();
}


void Admin_Students_List::switchWindow_AdminStudentsEnroll()
{
    // Switch ui window to Admin_Students_Enroll
    admin_students_enroll = new Admin_Students_Enroll;
    admin_students_enroll->show();
    this->hide();
}


void Admin_Students_List::switchWindow_AdminStudentsUpdate(const QString &key_studentId)
{
    // Set primary key for updating student info
    $updateKey_StudentInfo = key_studentId;

    // Switch ui window to Admin_Students_Update
    admin_students_update = new Admin_Students_Update;
    admin_students_update->show();
    this->hide();
}


void Admin_Students_List::moveEvent(QMoveEvent *event)
{
    // Saves window position every movement instance of a window
    QMainWindow::moveEvent(event);
    WindowPositionManager::saveWindowPosition(event->pos());
}
