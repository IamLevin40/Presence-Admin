#ifndef ADMIN_CLASSES_LIST_H
#define ADMIN_CLASSES_LIST_H

#include <QMainWindow>
#include <QMoveEvent>


// Forward declaration of class files
class Admin_Login_Form;
class Admin_Students_List;
class Admin_Lecturers_List;
class Admin_Classes_Create;


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

        // Reference the class files here
        Admin_Login_Form *admin_login_form;
        Admin_Students_List *admin_students_list;
        Admin_Lecturers_List *admin_lecturers_list;
        Admin_Classes_Create *admin_classes_create;

        // All custom functions() should be recognized upon here
        void switchWindow_AdminLoginForm();
        void switchWindow_AdminStudentsList();
        void switchWindow_AdminLecturersList();
        void switchWindow_AdminClassesCreate();
};

#endif // ADMIN_CLASSES_LIST_H
