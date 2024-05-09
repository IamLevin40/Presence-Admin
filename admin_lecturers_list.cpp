#include "global_main.h"

#include "admin_lecturers_list.h"
#include "ui_admin_lecturers_list.h"


// Include header files for forward declaration
#include "admin_login_form.h"
#include "admin_students_list.h"
#include "admin_classes_list.h"
#include "admin_lecturers_add.h"
#include "admin_lecturers_update.h"
#include "admin_lecturers_send_email.h"


Admin_Lecturers_List::Admin_Lecturers_List(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Admin_Lecturers_List)
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
    Admin_Lecturers_List::populateCombobox();
    Admin_Lecturers_List::filterSearchCall();
    DateTimeUtils::updateDateTimeUtils(ui->dateLabel, ui->timeLabel);

    // Connect ui objects to functions based on user interaction
    connect(ui->searchButton, &QPushButton::clicked, this, &Admin_Lecturers_List::filterSearchCall);
    connect(ui->searchTextbox, &QLineEdit::textChanged, this, &Admin_Lecturers_List::filterSearchCall);
    connect(ui->prevPageButton, &QPushButton::clicked, this, [=](){ FilteringManager::incrementPage(ui->numberPageTextbox, -1); });
    connect(ui->nextPageButton, &QPushButton::clicked, this, [=](){ FilteringManager::incrementPage(ui->numberPageTextbox, 1); });
    connect(ui->numberPageTextbox, &QLineEdit::textChanged, this, [=](){
        FilteringManager::validatePageEdit(ui->numberPageTextbox); Admin_Lecturers_List::filterSearchCall();
    });

    connect(ui->backButton, &QPushButton::clicked, this, &Admin_Lecturers_List::switchWindow_AdminLoginForm);
    connect(ui->studentListButton, &QPushButton::clicked, this, &Admin_Lecturers_List::switchWindow_AdminStudentsList);
    connect(ui->classListButton, &QPushButton::clicked, this, &Admin_Lecturers_List::switchWindow_AdminClassesList);
    connect(ui->addLecturerButton, &QPushButton::clicked, this, &Admin_Lecturers_List::switchWindow_AdminLecturersAdd);

    // Load window position
    QPoint windowPos = WindowPositionManager::loadWindowPosition();
    windowPos.setY(windowPos.y() - 31);
    move(windowPos);
}


Admin_Lecturers_List::~Admin_Lecturers_List()
{
    delete ui;
}


void Admin_Lecturers_List::filterSearchCall()
{
    // Fetch string input text() from textboxes and comboboxes
    int pageNumber = ui->numberPageTextbox->text().toInt();
    QString college = ui->collegeCombobox->currentText();
    QString searchText = ui->searchTextbox->text().toUpper();

    // Extract unnecessary substring for data handling
    int collegeIndex = college.indexOf('|');
    college = college.left(collegeIndex).trimmed();

    // Proceed to selecting data from database
    Admin_Lecturers_List::selectLecturerInfo(pageNumber, college, searchText);
}


void Admin_Lecturers_List::selectLecturerInfo(const int &pageNumber, const QString &college, const QString &searchText)
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
    QList<QStringList> lecturerDataList;
    QSqlDatabase::database().transaction();
    QSqlQuery query(database);

    // Determine search type based on the search text format
    QString searchType;
    if (!searchText.isEmpty())
    {
        QChar firstChar = searchText.at(0);
        if (firstChar.isDigit()) { searchType = "LecturerId LIKE :searchText"; }
        else { searchType = "(LastName LIKE :searchText OR FirstName LIKE :searchText)"; }
    }

    // Prepare sql command for selecting data
    QString sqlQuery = "SELECT * FROM LecturerInfo WHERE ";
    if (!searchText.isEmpty())
    {
        sqlQuery += searchType + " AND ";
    }
    sqlQuery += "(College = :college OR :college IS NULL) \
                ORDER BY College, LastName, FirstName ASC \
                LIMIT :limit OFFSET :offset";
    query.prepare(sqlQuery);

    // Bind values to the query
    if (!searchText.isEmpty())
    {
        query.bindValue(":searchText", "%" + searchText + "%");
    }
    query.bindValue(":college", college.isEmpty() ? QVariant::fromValue(nullptr) : QVariant::fromValue(college));
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
            rowData << query.value(0).toString();   // LecturerId
            rowData << query.value(1).toString();   // Pin
            rowData << query.value(2).toString();   // LastName
            rowData << query.value(3).toString();   // FirstName
            rowData << query.value(4).toString();   // College

            lecturerDataList.append(rowData);
        }
    }

    // Close the database after using
    QSqlDatabase::database().commit();
    database.close();

    // Proceed to display data list
    Admin_Lecturers_List::displayLecturerInfo(lecturerDataList);
}


void Admin_Lecturers_List::displayLecturerInfo(const QList<QStringList> &dataList)
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
        QString lecturerId = data[0];
        QString pin = data[1];
        QString lastName = data[2];
        QString firstName = data[3];
        QString college = data[4];

        // Create data group box
        QGroupBox *dataGroup = new QGroupBox(ui->lecturersListArea);
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

        // Set up dataCollegeLabel
        QLabel *dataCollegeLabel = new QLabel(dataGroup);
        dataCollegeLabel->setText(college);
        dataCollegeLabel->setStyleSheet("QLabel { color: #FFFFFF; font-family: Poppins; font-size: 9px; font-style: normal; font-weight: 400; line-height: normal; }");
        dataCollegeLabel->setGeometry(60, 25, 180, 10);

        // Set up dataLecturerIdLabel
        QLabel *dataLecturerIdLabel = new QLabel(dataGroup);
        dataLecturerIdLabel->setText(lecturerId);
        dataLecturerIdLabel->setStyleSheet("QLabel { color: #15CAE3; font-family: Poppins; font-size: 9px; font-style: normal; font-weight: 400; line-height: normal; }");
        dataLecturerIdLabel->setGeometry(60, 40, 70, 10);

        // Set up dataPinLabel
        QLabel *dataPinLabel = new QLabel(dataGroup);
        dataPinLabel->setText(pin);
        dataPinLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        dataPinLabel->setStyleSheet("QLabel { color: #15CAE3; font-family: Poppins; font-size: 9px; font-style: normal; font-weight: 400; line-height: normal; }");
        dataPinLabel->setGeometry(240, 40, 60, 10);

        // Set up dataSendMailButton
        QPushButton *dataSendMailButton = new QPushButton(dataGroup);
        dataSendMailButton->setIcon(QIcon(":/res/assets/send_mail.png"));
        dataSendMailButton->setStyleSheet("QPushButton { border: 0px; border-radius: 0px; background: none; }");
        dataSendMailButton->setGeometry(245, 10, 15, 15);
        connect(dataSendMailButton, &QPushButton::clicked, this, [=]() {
            Admin_Lecturers_List::switchWindow_AdminLecturersSendEmail(lecturerId);
        });

        // Set up dataEditButton
        QPushButton *dataEditButton = new QPushButton(dataGroup);
        dataEditButton->setIcon(QIcon(":/res/assets/edit.png"));
        dataEditButton->setStyleSheet("QPushButton { border: 0px; border-radius: 0px; background: none; }");
        dataEditButton->setGeometry(265, 10, 15, 15);
        connect(dataEditButton, &QPushButton::clicked, this, [=]() {
            Admin_Lecturers_List::switchWindow_AdminLecturersUpdate(lecturerId);
        });

        // Set up dataDeleteButton
        QPushButton *dataDeleteButton = new QPushButton(dataGroup);
        dataDeleteButton->setIcon(QIcon(":/res/assets/delete.png"));
        dataDeleteButton->setStyleSheet("QPushButton { border: 0px; border-radius: 0px; background: none; }");
        dataDeleteButton->setGeometry(285, 10, 15, 15);
        connect(dataDeleteButton, &QPushButton::clicked, this, [=]() {
            Admin_Lecturers_List::displayDeleteWarning(lecturerId);
        });

        // Add data group box to contents layout
        posY += dataGroup->height() + 5;
        dataGroup->setVisible(true);
        dataGroup->raise();
    }
}


void Admin_Lecturers_List::displayDeleteWarning(const QString &key_lecturerId)
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
        Admin_Lecturers_List::deleteLecturerInfo(key_lecturerId);
    });

    // Set up deleteNoButton
    QPushButton *deleteNoButton = new QPushButton(deleteWarningBox);
    deleteNoButton->setIcon(QIcon(":/res/assets/no.png"));
    deleteNoButton->setIconSize(QSize(90, 40));
    deleteNoButton->setStyleSheet("QPushButton { border: 0px; border-radius: 0px; background: none; }");
    deleteNoButton->setGeometry(135, 60, 80, 40);
    connect(deleteNoButton, &QPushButton::clicked, this, Admin_Lecturers_List::disregardDelete);

    deleteWarningGroup->setVisible(true);
    deleteWarningGroup->raise();
}


void Admin_Lecturers_List::deleteLecturerInfo(const QString &key_lecturerId)
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
    query.prepare("DELETE FROM LecturerInfo WHERE LecturerId = :lecturerId");

    // Bind values to the query
    query.bindValue(":lecturerId", key_lecturerId);

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

    Admin_Lecturers_List::disregardDelete();
    Admin_Lecturers_List::filterSearchCall();
}


void Admin_Lecturers_List::disregardDelete()
{
    // Find and delete the existing group box
    QGroupBox *existingGroupBox = ui->centralwidget->findChild<QGroupBox*>("deleteWarningGroup");
    if (existingGroupBox) { delete existingGroupBox; }
}


void Admin_Lecturers_List::populateCombobox()
{
    // Populate comboboxes with items
    ui->collegeCombobox->addItems(Colleges::$colleges);

    // Connect comboboxes to checkComboboxIndex() and filterSearchCall()
    connect(ui->collegeCombobox, &QComboBox::currentIndexChanged, this, [=](){
        FilteringManager::checkComboboxIndex(ui->collegeCombobox); Admin_Lecturers_List::filterSearchCall();
    });
}


void Admin_Lecturers_List::switchWindow_AdminLoginForm()
{
    // Switch ui window to Admin_Login_Form
    admin_login_form = new Admin_Login_Form;
    admin_login_form->show();
    this->hide();
}


void Admin_Lecturers_List::switchWindow_AdminStudentsList()
{
    // Switch ui window to Admin_Students_List
    admin_students_list = new Admin_Students_List;
    admin_students_list->show();
    this->hide();
}


void Admin_Lecturers_List::switchWindow_AdminClassesList()
{
    // Switch ui window to Admin_Classes_List
    admin_classes_list = new Admin_Classes_List;
    admin_classes_list->show();
    this->hide();
}


void Admin_Lecturers_List::switchWindow_AdminLecturersAdd()
{
    // Switch ui window to Admin_Lecturers_Add
    admin_lecturers_add = new Admin_Lecturers_Add;
    admin_lecturers_add->show();
    this->hide();
}


void Admin_Lecturers_List::switchWindow_AdminLecturersUpdate(const QString &key_lecturerId)
{
    // Set primary key for updating student info
    $updateKey_LecturerInfo = key_lecturerId;

    // Switch ui window to Admin_Lecturers_Update
    admin_lecturers_update = new Admin_Lecturers_Update;
    admin_lecturers_update->show();
    this->hide();
}


void Admin_Lecturers_List::switchWindow_AdminLecturersSendEmail(const QString &key_lecturerId)
{
    // Set primary key for updating student info
    $updateKey_LecturerInfo = key_lecturerId;

    // Switch ui window to Admin_Lecturers_Send_Email
    admin_lecturers_send_email = new Admin_Lecturers_Send_Email;
    admin_lecturers_send_email->show();
    this->hide();
}


void Admin_Lecturers_List::moveEvent(QMoveEvent *event)
{
    // Saves window position every movement instance of a window
    QMainWindow::moveEvent(event);
    WindowPositionManager::saveWindowPosition(event->pos());
}
