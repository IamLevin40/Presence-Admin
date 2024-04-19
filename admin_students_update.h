#ifndef ADMIN_STUDENTS_UPDATE_H
#define ADMIN_STUDENTS_UPDATE_H

#include <QMainWindow>
#include <QMoveEvent>


// Forward declaration of class files
class Admin_Students_List;
class Admin_Classes_List;
class Admin_Lecturers_List;


namespace Ui {
    class Admin_Students_Update;
}

class Admin_Students_Update : public QMainWindow
{
    Q_OBJECT

    public:
        explicit Admin_Students_Update(QWidget *parent = nullptr);
        ~Admin_Students_Update();

    protected:
        void moveEvent(QMoveEvent *event) override;

    private:
        Ui::Admin_Students_Update *ui;

        // Reference the class files here
        Admin_Students_List *admin_students_List;
        Admin_Classes_List *admin_classes_list;
        Admin_Lecturers_List *admin_lecturers_list;

        // All custom functions() should be recognized upon here
        void switchWindow_AdminStudentsList();
        void switchWindow_AdminClassesList();
        void switchWindow_AdminLecturersList();
};

#endif // ADMIN_STUDENTS_UPDATE_H
