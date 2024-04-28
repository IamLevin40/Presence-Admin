#ifndef GLOBAL_MAIN_H
#define GLOBAL_MAIN_H

#include <QString>
#include <QVector>
#include <QPoint>
#include <QList>
#include <QStringList>
#include <QDateTime>
#include <QTime>
#include <QTimer>
#include <QSettings>
#include <QObject>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>


// Global variables
extern QString $appCompany;
extern QString $appName;

extern QString $adminId;
extern QString $adminPin;
extern short $studentIdLength;
extern short $lecturerIdLength;
extern short $pinLength;
extern short $minPage;
extern short $maxPage;
extern short $dataLimitPerPage;


// Global database
extern QString $sql_Driver;
extern QString $db_Hostname;
extern QString $db_Username;
extern QString $db_Password;
extern QString $db_Database;
extern int $db_Port;

extern QString $updateKey_StudentInfo;
extern QString $updateKey_LecturerInfo;
extern QStringList $updateKeys_ClassInfo;
extern QStringList $selectKeys_ClassInfo;


// Global colleges and its corresponding programs
namespace Colleges
{
    // Define colleges, programs, years, sections, and regular/irregular
    extern const QStringList $colleges;
    extern const QStringList $programs;
    extern const QStringList $years;
    extern const QStringList $sections;
    extern const QStringList $isRegular;

    // Define colleges and its corresponding programs offered
    extern const QMap<QString, QStringList> $offeredPrograms;
}


// Global schedules
namespace Schedules
{
    // Define school years, semesters, days, and time options
    extern const QStringList $schoolYears;
    extern const QStringList $semesters;
    extern const QStringList $dayOptions;

    extern const QStringList $timeOptions;
    QStringList generateTimeOptions();
}


// Global messages
namespace Messages
{
    inline QString errorConnection() { return "No internet connection. Try again."; }
    inline QString unaccessDatabase() { return "Unable to access database. Try again."; }
    inline QString errorInsertData() { return "Unable to insert data to the database."; }
    inline QString errorSelectData() { return "Unable to select data from the database."; }
    inline QString errorDeleteData() { return "Unable to delete data from the database."; }
    inline QString errorUpdateData() { return "Unable to update data from the database."; }
    inline QString errorCreateTable() { return "Unable to create table in the database."; }
    inline QString errorDeleteTable() { return "Unable to delete table from the database."; }

    inline QString emptyAdminId() { return "Admin number is required."; }
    inline QString invalidAdminId() { return "Admin number is invalid."; }
    inline QString emptyAdminPin() { return "Pin is required."; }
    inline QString invalidAdminPin() { return "Pin is invalid."; }
    inline QString loginSuccessful() { return "Successfully logged in."; }

    inline QString emptyStudentId() { return "Student number is required."; }
    inline QString incompleteLengthStudentId() { return "Student number should be exactly 9 characters long."; }
    inline QString invalidStudentId() { return "Student number is invalid."; }
    inline QString alreadyExistStudentId() { return "Student number already exist in the database."; }
    inline QString alreadyEnrolledStudentId() { return "Student number already enrolled in this class."; }
    inline QString notExistStudentId() { return "Student number does not exist."; }

    inline QString emptyLecturerId() { return "Lecturer number is required."; }
    inline QString incompleteLengthLecturerId() { return "Lecturer number should be exactly 4 characters long."; }
    inline QString invalidLecturerId() { return "Lecturer number is invalid."; }
    inline QString alreadyExistLecturerId() { return "Lecturer number already exist in the database."; }
    inline QString notExistLecturerId() { return "Lecturer number does not exist."; }

    inline QString emptyPin() { return "Pin is required."; }
    inline QString incompleteLengthPin() { return "Pin should be exactly 6 digits long"; }
    inline QString invalidPin() { return "Pin is invalid. Must only consist digits."; }

    inline QString noSelectedIsRegular() { return "No selected option in either regular or irregular."; }
    inline QString noSelectedSchoolYear() { return "No selected school year."; }
    inline QString noSelectedSemester() { return "No selected semester."; }
    inline QString noSelectedCollege() { return "No selected college."; }
    inline QString noSelectedProgram() { return "No selected program."; }
    inline QString noSelectedYear() { return "No selected year."; }
    inline QString noSelectedSection() { return "No selected section."; }

    inline QString emptyLastName() { return "Last name is required."; }
    inline QString emptyFirstName() { return "First name is required."; }
    inline QString emptySubjectCode() { return "Subject code is required."; }
    inline QString invalidSubjectCode() { return "Subject code is invalid. Should have no spaces."; }
    inline QString emptySubjectDesc() { return "Subject description is required."; }
    inline QString emptyRoom() { return "Room is required."; }

    inline QString noSelectedFirstDay() { return "No selected first day."; }
    inline QString noSelectedStartTimeFirstDay() { return "No selected start time on the first day."; }
    inline QString noSelectedEndTimeFirstDay() { return "No selected end time on the first day."; }
    inline QString noSelectedSecondDay() { return "No selected second day."; }
    inline QString noSelectedStartTimeSecondDay() { return "No selected start time on the second day."; }
    inline QString noSelectedEndTimeSecondDay() { return "No selected end time on the second day."; }
    inline QString noSelectedThirdDay() { return "No selected third day."; }
    inline QString noSelectedStartTimeThirdDay() { return "No selected start time on the third day."; }
    inline QString noSelectedEndTimeThirdDay() { return "No selected end time on the third day."; }
}


// Global timer
class GlobalTimer : public QObject
{
    Q_OBJECT

    public:
        static QTimer* timer();
        static void displayTextForDuration(QObject* object, const QString& text, int duration);

    private:
        static QTimer* m_timer;
};


// Date and Time Utilities
class DateTimeUtils
{
    public:
        static void updateDateTimeUtils(QLabel *dateLabel, QLabel *timeLabel);
};


// Filtering manager
class FilteringManager
{
    public:
        static QString convertSchoolYear(const QString &schoolYear);
        static void incrementPage(QObject *object, const int &index);
        static void validatePageEdit(QObject *object);
        static void selectOptionByText(QComboBox *comboBox, const QString &textToSelect);
        static void checkComboboxIndex(QObject *object);
        static void setupComboboxDependency(const QVector<QComboBox *> &comboBoxes);
};


// Window position manager
class WindowPositionManager
{
    public:
        static QPoint loadWindowPosition();
        static void saveWindowPosition(const QPoint &position);
};


#endif // GLOBAL_MAIN_H
