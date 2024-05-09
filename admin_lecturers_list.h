#ifndef ADMIN_LECTURERS_LIST_H
#define ADMIN_LECTURERS_LIST_H

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
class Admin_Classes_List;
class Admin_Lecturers_Add;
class Admin_Lecturers_Update;
class Admin_Lecturers_Send_Email;


namespace Ui {
    class Admin_Lecturers_List;
}

class Admin_Lecturers_List : public QMainWindow
{
    Q_OBJECT

    public:
        explicit Admin_Lecturers_List(QWidget *parent = nullptr);
        ~Admin_Lecturers_List();

    protected:
        void moveEvent(QMoveEvent *event) override;

    private:
        Ui::Admin_Lecturers_List *ui;
        QSqlDatabase database;
        QList<QGroupBox*> groupBoxList;

        // Reference the class files here
        Admin_Login_Form *admin_login_form;
        Admin_Students_List *admin_students_list;
        Admin_Classes_List *admin_classes_list;
        Admin_Lecturers_Add *admin_lecturers_add;
        Admin_Lecturers_Update *admin_lecturers_update;
        Admin_Lecturers_Send_Email *admin_lecturers_send_email;

        // All custom functions() should be recognized upon here
        void filterSearchCall();
        void selectLecturerInfo(const int &pageNumber, const QString &college, const QString &searchText);
        void displayLecturerInfo(const QList<QStringList> &dataList);

        void displayDeleteWarning(const QString &key_lecturerId);
        void deleteLecturerInfo(const QString &key_lecturerId);
        void disregardDelete();

        void populateCombobox();

        void switchWindow_AdminLoginForm();
        void switchWindow_AdminStudentsList();
        void switchWindow_AdminClassesList();
        void switchWindow_AdminLecturersAdd();
        void switchWindow_AdminLecturersUpdate(const QString &key_lecturerId);
        void switchWindow_AdminLecturersSendEmail(const QString &key_lecturerId);
};

#endif // ADMIN_LECTURERS_LIST_H
