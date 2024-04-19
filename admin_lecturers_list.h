#ifndef ADMIN_LECTURERS_LIST_H
#define ADMIN_LECTURERS_LIST_H

#include <QMainWindow>
#include <QMoveEvent>


// Forward declaration of class files
class Admin_Login_Form;
class Admin_Students_List;
class Admin_Classes_List;
class Admin_Lecturers_Add;


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

        // Reference the class files here
        Admin_Login_Form *admin_login_form;
        Admin_Students_List *admin_students_list;
        Admin_Classes_List *admin_classes_list;
        Admin_Lecturers_Add *admin_lecturers_add;

        // All custom functions() should be recognized upon here
        void switchWindow_AdminLoginForm();
        void switchWindow_AdminStudentsList();
        void switchWindow_AdminClassesList();
        void switchWindow_AdminLecturersAdd();
};

#endif // ADMIN_LECTURERS_LIST_H
