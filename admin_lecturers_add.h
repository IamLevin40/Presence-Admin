#ifndef ADMIN_LECTURERS_ADD_H
#define ADMIN_LECTURERS_ADD_H

#include <QMainWindow>
#include <QMoveEvent>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QRandomGenerator>


// Forward declaration of class files
class Admin_Students_List;
class Admin_Classes_List;
class Admin_Lecturers_List;


namespace Ui {
    class Admin_Lecturers_Add;
}

class Admin_Lecturers_Add : public QMainWindow
{
    Q_OBJECT

    public:
        explicit Admin_Lecturers_Add(QWidget *parent = nullptr);
        ~Admin_Lecturers_Add();

    protected:
        void moveEvent(QMoveEvent *event) override;

    private:
        Ui::Admin_Lecturers_Add *ui;
        QSqlDatabase database;

        // Reference the class files here
        Admin_Students_List *admin_students_list;
        Admin_Classes_List *admin_classes_list;
        Admin_Lecturers_List *admin_lecturers_list;

        // All custom functions() should be recognized upon here
        void lecturerAddCall();
        QString verifyLecturerAdd(const QString &lecturerId, const QString &lastName, const QString &firstName, const QString &college);
        void insertLecturerData(const QString &lecturerId, const QString &lastName, const QString &firstName, const QString &college);

        void populateCombobox();

        void switchWindow_AdminStudentsList();
        void switchWindow_AdminClassesList();
        void switchWindow_AdminLecturersList();
};

#endif // ADMIN_LECTURERS_ADD_H
