#include "global_main.h"


// Global variables
QString $appCompany = "BSCS1B";
QString $appName = "Presence-Admin";

QString $adminId = "admin";
QString $adminPin = "123456";
short $studentIdLength = 9;
short $lecturerIdLength = 4;
short $pinLength = 6;
short $minPage = 1;
short $maxPage = 999;
short $dataLimitPerPage = 5;


// Global database
QString $sql_Driver= "QMYSQL";
QString $db_Hostname = "sql6.freesqldatabase.com";
QString $db_Username = "sql6701487";
QString $db_Password = "6adX9UfkzD";
QString $db_Database = "sql6701487";
int $db_Port = 3306;

QString $updateKey_StudentInfo = "";
QString $updateKey_LecturerInfo = "";
QStringList $updateKeys_ClassInfo;
QStringList $selectKeys_ClassInfo;


// Global colleges and its corresponding programs
namespace Colleges
{
    const QStringList $colleges = {
        "",
        "CAS | College of Arts & Sciences",
        "CBM | College of Business & Management",
        "CICT | College of Information and Communications Technology",
        "COC | College of Communication",
        "COD | College of Dentistry",
        "COE | College of Education",
        "COL | College of Law",
        "COM | College of Medicine",
        "CON | College of Nursing",
        "PESCAR | College of Physical Education, Sports, Culture, Arts, and Recreation"
    };
    const QStringList $programs = {
        "",
        // CAS
        "ABELS | BA in English Language Studies",
        "ABFL | BA in Foreign Languages",
        "ABPOLSCI | BA in Political Science",
        "BSAM | BS in Applied Mathematics",
        "BSBIO | BS in Biology",
        "BSCHEM | BS in Chemistry",
        // CBM
        "BSBAFM | BS in Business Administration major in Financial Management",
        "BSBAOM | BS in Business Administration major in Operation Management",
        "BSCM | BS in Cooperatives Management",
        "BSHM | BS in Hospitality Management",
        "BSTM | BS in Tourism Management",
        // CICT
        "BLIS | Bachelor of Library and Information Science",
        "BSCS | BS in Computer Science",
        "BSEMC | BS in Entertainment and Multimedia Computing",
        "BSIS | BS in Information Systems",
        "BSIT | BS in Information Technology",
        // COC
        "BAB | BA in Broadcasting",
        "BAJ | BA in Journalism",
        "BSDC | BS in Development Communication",
        // COD
        "DMD | Doctor of Dental Medicine",
        // COE
        "BECED | Bachelor of Early Childhood Education",
        "BEED | Bachelor of Elementary Education",
        "BSEDENG | Bachelor of Secondary Education major in English",
        "BSEDFIL | Bachelor of Secondary Education major in Filipino",
        "BSEDMATH | Bachelor of Secondary Education major in Mathematics",
        "BSEDSCI | Bachelor of Secondary Education major in Science",
        "BSEDSOCSTUD | Bachelor of Secondary Education major in Social Studies",
        "BSEDVALED | Bachelor of Secondary Education major in Values Education",
        "BSNEDELEM | Bachelor of Special Needs Education major in Elementary School Teaching",
        "BSNEDTDHL | Bachelor of Special Needs Education major in Teaching Deaf and Hard-of-Hearing Learners",
        // COL
        "JD | Juris Doctor",
        // COM
        "MD | Doctor of Medicine",
        // CON
        "BSN | BS in Nursing",
        // PESCAR
        "BCAED | Bachelor of Culture and Arts Education",
        "BPEA | Bachelor of Performing Arts",
        "BPED | Bachelor of Physical Education",
        "BSESSFSC | BS in Exercise and Sports Sciences major in Fitness and Sports Coaching"
    };
    const QStringList $years = {"", "1", "2", "3", "4"};
    const QStringList $sections = {"", "A", "B", "C", "D", "E", "F"};
    const QStringList $isRegular = {"", "Regular", "Irregular"};


    const QMap<QString, QStringList> $offeredPrograms = {
        {
            "CAS | College of Arts & Sciences",
            {
                "",
                "ABELS | BA in English Language Studies",
                "ABFL | BA in Foreign Languages",
                "ABPOLSCI | BA in Political Science",
                "BSAM | BS in Applied Mathematics",
                "BSBIO | BS in Biology",
                "BSCHEM | BS in Chemistry"
            }
        },
        {
            "CBM | College of Business & Management",
            {
                "",
                "BSBAFM | BS in Business Administration major in Financial Management",
                "BSBAOM | BS in Business Administration major in Operation Management",
                "BSCM | BS in Cooperatives Management",
                "BSHM | BS in Hospitality Management",
                "BSTM | BS in Tourism Management"
            }
        },
        {
            "CICT | College of Information and Communications Technology",
            {
                "",
                "BLIS | Bachelor of Library and Information Science",
                "BSCS | BS in Computer Science",
                "BSEMC | BS in Entertainment and Multimedia Computing",
                "BSIS | BS in Information Systems",
                "BSIT | BS in Information Technology"
            }
        },
        {
            "COC | College of Communication",
            {
                "",
                "BAB | BA in Broadcasting",
                "BAJ | BA in Journalism",
                "BSDC | BS in Development Communication"
            }
        },
        {
            "COD | College of Dentistry",
            {
                "",
                "DMD | Doctor of Dental Medicine"
            }
        },
        {
            "COE | College of Education",
            {
                "",
                "BECED | Bachelor of Early Childhood Education",
                "BEED | Bachelor of Elementary Education",
                "BSEDENG | Bachelor of Secondary Education major in English",
                "BSEDFIL | Bachelor of Secondary Education major in Filipino",
                "BSEDMATH | Bachelor of Secondary Education major in Mathematics",
                "BSEDSCI | Bachelor of Secondary Education major in Science",
                "BSEDSOCSTUD | Bachelor of Secondary Education major in Social Studies",
                "BSEDVALED | Bachelor of Secondary Education major in Values Education",
                "BSNEDELEM | Bachelor of Special Needs Education major in Elementary School Teaching",
                "BSNEDTDHL | Bachelor of Special Needs Education major in Teaching Deaf and Hard-of-Hearing Learners"
            }
        },
        {
            "COL | College of Law",
            {
                "",
                "JD | Juris Doctor"
            }
        },
        {
            "COM | College of Medicine",
            {
                "",
                "MD | Doctor of Medicine"
            }
        },
        {
            "CON | College of Nursing",
            {
                "",
                "BSN | BS in Nursing"
            }
        },
        {
            "PESCAR | College of Physical Education, Sports, Culture, Arts, and Recreation",
            {
                "",
                "BCAED | Bachelor of Culture and Arts Education",
                "BPEA | Bachelor of Performing Arts",
                "BPED | Bachelor of Physical Education",
                "BSESSFSC | BS in Exercise and Sports Sciences major in Fitness and Sports Coaching"
            }
        }
    };
}


// Global schedules
namespace Schedules
{
    const QStringList $schoolYears = {"", "2023-2024"};
    const QStringList $semesters = {"", "1", "2"};
    const QStringList $dayOptions = {"", "M", "T", "W", "TH", "F", "S"};
    const QStringList $timeOptions = generateTimeOptions();

    QStringList generateTimeOptions()
    {
        QStringList times;
        QTime time(7, 0);

        // Generate and add time options from 0, 0 with 15 minutes interval
        while (time < QTime(19, 15))
        {
            times.append(time.toString("h:mmAP"));
            time = time.addSecs(15 * 60);
        }

        return times;
    }
}


// Global timer
QTimer* GlobalTimer::m_timer = new QTimer();
void GlobalTimer::displayTextForDuration(QObject* object, const QString& text, int duration)
{
    QLabel* label = qobject_cast<QLabel*>(object);
    if (label) {
        label->setText(text);

        // Stop the existing timer if it's running
        if (m_timer->isActive()) {
            m_timer->stop();
        }

        // Start a new timer with the specified duration
        m_timer->singleShot(duration, label, [=]() {
            label->clear();
        });
    }
}


// Convert school year to short year and vice versa
QString FilteringManager::convertSchoolYear(const QString &schoolYear)
{
    // Check if the input year has a dash, if yes, convert to short year
    if (schoolYear.contains("-"))
    {
        QString firstPart = schoolYear.mid(2, 2);
        QString secondPart = schoolYear.mid(7, 2);
        return firstPart + secondPart;
    }
    // If the input year is already a short year, convert to full school year
    else
    {
        QString firstPart = schoolYear.mid(0, 2);
        QString lastPart = schoolYear.mid(2, 2);
        return "20" + firstPart + "-" + "20" + lastPart;
    }
}


// Increment or decrement page
void FilteringManager::incrementPage(QObject *object, const int &index)
{
    // Get the sender object: lineedit
    QLineEdit *lineEdit = qobject_cast<QLineEdit*>(object);
    if (!lineEdit)
        return;

    // Fetch the current page and increment with index
    int currentPage = lineEdit->text().toInt();
    currentPage += index;

    // Check if exceeds maximum page or below minimum page
    if (currentPage <= $minPage) { currentPage = $minPage; }
    if (currentPage >= $maxPage) { currentPage = $maxPage; }

    lineEdit->setText(QString::number(currentPage));
}


// Validate the page textbox that user edited
void FilteringManager::validatePageEdit(QObject *object)
{
    // Get the sender object: lineedit
    QLineEdit *lineEdit = qobject_cast<QLineEdit*>(object);
    if (!lineEdit)
        return;

    // Get the text from lineedit
    QString text = lineEdit->text();
    bool isNum;
    int pageNumber = text.toInt(&isNum);

    // Return if the user is still editing the page number
    if (text.isEmpty() && lineEdit->hasFocus())
        return;

    // Check if conversion to integer was successful and within range
    if (!(isNum && pageNumber >= $minPage && pageNumber <= $maxPage))
    {
        // Revert back to the last valid page number
        lineEdit->setText(QString::number($minPage));
        return;
    }

    // Update the text to remove leading zeroes
    lineEdit->setText(QString::number(pageNumber));
}


// Select an option in a combobox based on the text
void FilteringManager::selectOptionByText(QComboBox *comboBox, const QString &textToSelect)
{
    // Iterate through each option in the combobox
    for (int i = 0; i < comboBox->count(); ++i)
    {
        QString optionText = comboBox->itemText(i);
        QString extractedText = optionText.split('|').first().trimmed();

        // Compare the text and set the current index to select the option if true
        if (extractedText == textToSelect)
        {
            comboBox->setCurrentIndex(i);
            break;
        }
    }
}


// Check selected option from combobox
void FilteringManager::checkComboboxIndex(QObject* object)
{
    // Get the sender object: combobox
    QComboBox *comboBox = qobject_cast<QComboBox*>(object);
    if (!comboBox)
        return;

    // Deselect the combobox if the current option has no string
    QString selectedText = comboBox->currentText();
    if (selectedText.isEmpty()) {
        comboBox->setCurrentIndex(-1);
    }
}


// Disable combobox if the previous one has not been selected
void FilteringManager::setupComboboxDependency(const QVector<QComboBox *> &comboBoxes)
{
    // Connect each combobox to the next one in the list
    for (int i = 0; i < comboBoxes.size() - 1; ++i) {
        QComboBox *currentComboBox = comboBoxes[i];
        QComboBox *nextComboBox = comboBoxes[i + 1];

        QObject::connect(currentComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
            // Enable the next combobox if an item is selected in the current combobox, otherwise disable it
            nextComboBox->setCurrentIndex(-1);
            nextComboBox->setEnabled(index != -1);
        });

        // Initialize the state of the next combobox based on the initial selection of the current combobox
        bool enableNextComboBox = (currentComboBox->currentIndex() != -1);
        nextComboBox->setEnabled(enableNextComboBox);
    }
}


// Window position manager
QPoint WindowPositionManager::loadWindowPosition()
{
    // Return value of "windowPosition" from HKEY_CURRENT_USER\Software\$appCompany\$appName
    QSettings settings($appCompany, $appName);
    return settings.value("windowPosition", QPoint(0, 0)).toPoint();
}

void WindowPositionManager::saveWindowPosition(const QPoint &position)
{
    // Saves value of "windowPosition" to HKEY_CURRENT_USER\Software\$appCompany\$appName
    QSettings settings($appCompany, $appName);
    settings.setValue("windowPosition", position);
}
