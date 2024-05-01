#ifndef ADMIN_CLASSES_STUDENTS_LIST_H
#define ADMIN_CLASSES_STUDENTS_LIST_H

#include <QMainWindow>
#include <QMoveEvent>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLayout>
#include <QLayoutItem>


// Forward declaration of class files
class Admin_Students_List;
class Admin_Classes_List;
class Admin_Lecturers_List;
class Admin_Classes_Students_Add;


namespace Ui {
    class Admin_Classes_Students_List;
}

class Admin_Classes_Students_List : public QMainWindow
{
    Q_OBJECT

    public:
        explicit Admin_Classes_Students_List(QWidget *parent = nullptr);
        ~Admin_Classes_Students_List();

    protected:
        void moveEvent(QMoveEvent *event) override;

    private:
        Ui::Admin_Classes_Students_List *ui;
        QSqlDatabase database;
        QList<QGroupBox*> groupBoxList;

        // Reference the class files here
        Admin_Students_List *admin_students_list;
        Admin_Classes_List *admin_classes_list;
        Admin_Lecturers_List *admin_lecturers_list;
        Admin_Classes_Students_Add *admin_classes_students_add;

        // All custom functions() should be recognized upon here
        void selectClassInfo(const QStringList &keys_classInfo);
        void displayClassInfo(const QStringList &dataList);
        QStringList getLecturerInfo(const QString &lecturerId);

        void deleteUnknownStudents(const QStringList &keys_classInfo);

        void filterSearchCall();
        void selectEnlistedStudents(const int &pageNumber, const QStringList &keys_classInfo);
        void displayEnlistedStudents(const QList<QStringList> &dataList, const QList<QStringList> &recordList);

        void deleteStudentFromClass(const QString &studentId);

        void switchWindow_AdminStudentsList();
        void switchWindow_AdminClassesList();
        void switchWindow_AdminLecturersList();
        void switchWindow_AdminClassesStudentsAdd();
};

#endif // ADMIN_CLASSES_STUDENTS_LIST_H
