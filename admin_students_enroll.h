#ifndef ADMIN_STUDENTS_ENROLL_H
#define ADMIN_STUDENTS_ENROLL_H

#include <QMainWindow>
#include <QMoveEvent>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QRandomGenerator>


// Forward declaration of class files
class Admin_Students_List;
class Admin_Classes_List;
class Admin_Lecturers_List;


namespace Ui {
    class Admin_Students_Enroll;
}

class Admin_Students_Enroll : public QMainWindow
{
    Q_OBJECT

    public:
        explicit Admin_Students_Enroll(QWidget *parent = nullptr);
        ~Admin_Students_Enroll();

        QString isRegular;

    protected:
        void moveEvent(QMoveEvent *event) override;

    private slots:
        void collegeIndexChanged(int index);

    private:
        Ui::Admin_Students_Enroll *ui;
        QSqlDatabase database;

        // Reference the class files here
        Admin_Students_List *admin_students_List;
        Admin_Classes_List *admin_classes_list;
        Admin_Lecturers_List *admin_lecturers_list;

        // All custom functions() should be recognized upon here
        void studentEnrollCall();
        QString verifyStudentEnroll(const QString &studentId, const QString &lastName, const QString &firstName,
                                    const QString &college, const QString &program,
                                    const QString &year, const QString &section);
        void connectToDatabase(const QString &studentId, const QString &lastName, const QString &firstName,
                               const QString &college, const QString &program,
                               const QString &year, const QString &section);
        void handleIsRegularRadio(const QString &option);
        void populateCombobox();
        void switchWindow_AdminStudentsList();
        void switchWindow_AdminClassesList();
        void switchWindow_AdminLecturersList();
};

#endif // ADMIN_STUDENTS_ENROLL_H
