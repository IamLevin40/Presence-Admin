#ifndef ADMIN_CLASSES_LIST_H
#define ADMIN_CLASSES_LIST_H

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
class Admin_Login_Form;
class Admin_Students_List;
class Admin_Lecturers_List;
class Admin_Classes_Create;
class Admin_Classes_Update;
class Admin_Classes_Students_List;


namespace Ui {
    class Admin_Classes_List;
}

class Admin_Classes_List : public QMainWindow
{
    Q_OBJECT

    public:
        explicit Admin_Classes_List(QWidget *parent = nullptr);
        ~Admin_Classes_List();

    protected:
        void moveEvent(QMoveEvent *event) override;

    private:
        Ui::Admin_Classes_List *ui;
        QSqlDatabase database;
        QList<QGroupBox*> groupBoxList;

        // Reference the class files here
        Admin_Login_Form *admin_login_form;
        Admin_Students_List *admin_students_list;
        Admin_Lecturers_List *admin_lecturers_list;
        Admin_Classes_Create *admin_classes_create;
        Admin_Classes_Update *admin_classes_update;
        Admin_Classes_Students_List *admin_classes_students_list;

        // All custom functions() should be recognized upon here
        void filterSearchCall();
        void selectClassInfo(const int &pageNumber, const QString &schoolYear, const QString &semester,
                             const QString &program, const QString &year, const QString &section,
                             const QString &searchText);
        void displayClassInfo(const QList<QStringList> &dataList);

        void deleteClassInfo(const QStringList &keys_classInfo);

        void populateCombobox();
        QStringList getLecturerInfo(const QString &lecturerId);

        void switchWindow_AdminLoginForm();
        void switchWindow_AdminStudentsList();
        void switchWindow_AdminLecturersList();
        void switchWindow_AdminClassesCreate();
        void switchWindow_AdminClassesUpdate(const QStringList &updateKeys_ClassInfo);
        void switchWindow_AdminClassesStudentsList(const QStringList &selectKeys_ClassInfo);
};

#endif // ADMIN_CLASSES_LIST_H
