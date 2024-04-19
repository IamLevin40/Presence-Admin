#ifndef GLOBAL_MAIN_H
#define GLOBAL_MAIN_H

#include <QString>
#include <QPoint>
#include <QStringList>
#include <QTime>


// Global variables
extern QString $appCompany;
extern QString $appName;

extern QString $adminId;
extern QString $adminPin;
extern short $studentIdLength;
extern short $lecturerIdLength;
extern short $pinLength;


// Global database
extern QString $sql_Driver;
extern QString $db_Hostname;
extern QString $db_Username;
extern QString $db_Password;
extern QString $db_Database;
extern int $db_Port;


// Global colleges and its corresponding programs
namespace Colleges
{
    // Define colleges, programs, years, and sections
    extern const QStringList $colleges;
    extern const QStringList $programs;
    extern const QStringList $years;
    extern const QStringList $sections;

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

    inline QString emptyAdminId() { return "Admin number is required."; }
    inline QString invalidAdminId() { return "Admin number is invalid."; }
    inline QString emptyAdminPin() { return "Pin is required."; }
    inline QString invalidAdminPin() { return "Pin is invalid."; }
    inline QString loginSuccessful() { return "Successfully logged in."; }

    inline QString emptyStudentId() { return "Student number is required."; }
    inline QString incompleteLengthStudentId() { return "Student number should be exactly 9 characters long."; }
    inline QString invalidStudentId() { return "Student number is invalid."; }
    inline QString alreadyExistStudentId() { return "Student number already exist in database."; }
    inline QString emptyLecturerId() { return "Lecturer number is required."; }
    inline QString incompleteLengthLecturerId() { return "Lecturer number should be exactly 4 characters long."; }
    inline QString invalidLecturerId() { return "Lecturer number is invalid."; }
    inline QString alreadyExistLecturerId() { return "Lecturer number already exist in database."; }
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
}


// Window position manager
class WindowPositionManager
{
    public:
        static QPoint loadWindowPosition();
        static void saveWindowPosition(const QPoint &position);
};


#endif // GLOBAL_MAIN_H
