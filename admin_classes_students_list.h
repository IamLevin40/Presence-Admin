#ifndef ADMIN_CLASSES_STUDENTS_LIST_H
#define ADMIN_CLASSES_STUDENTS_LIST_H

#include <QMainWindow>
#include <QMoveEvent>


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

        // Reference the class files here
        Admin_Students_List *admin_students_list;
        Admin_Classes_List *admin_classes_list;
        Admin_Lecturers_List *admin_lecturers_list;
        Admin_Classes_Students_Add *admin_classes_students_add;

        // All custom functions() should be recognized upon here
        void switchWindow_AdminStudentsList();
        void switchWindow_AdminClassesList();
        void switchWindow_AdminLecturersList();
        void switchWindow_AdminClassesStudentsAdd();
};

#endif // ADMIN_CLASSES_STUDENTS_LIST_H
