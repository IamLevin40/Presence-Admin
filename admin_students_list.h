#ifndef ADMIN_STUDENTS_LIST_H
#define ADMIN_STUDENTS_LIST_H

#include <QMainWindow>
#include <QMoveEvent>


// Forward declaration of class files
class Admin_Login_Form;
class Admin_Classes_List;
class Admin_Lecturers_List;
class Admin_Students_Enroll;


namespace Ui {
    class Admin_Students_List;
}

class Admin_Students_List : public QMainWindow
{
    Q_OBJECT

    public:
        explicit Admin_Students_List(QWidget *parent = nullptr);
        ~Admin_Students_List();

    protected:
        void moveEvent(QMoveEvent *event) override;

    private:
        Ui::Admin_Students_List *ui;

        // Reference the class files here
        Admin_Login_Form *admin_login_form;
        Admin_Classes_List *admin_classes_list;
        Admin_Lecturers_List *admin_lecturers_list;
        Admin_Students_Enroll *admin_students_enroll;

        // All custom functions() should be recognized upon here
        void switchWindow_AdminLoginForm();
        void switchWindow_AdminClassesList();
        void switchWindow_AdminLecturersList();
        void switchWindow_AdminStudentsEnroll();
};

#endif // ADMIN_STUDENTS_LIST_H
