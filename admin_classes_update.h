#ifndef ADMIN_CLASSES_UPDATE_H
#define ADMIN_CLASSES_UPDATE_H

#include <QMainWindow>
#include <QMoveEvent>


// Forward declaration of class files
class Admin_Students_List;
class Admin_Classes_List;
class Admin_Lecturers_List;


namespace Ui {
    class Admin_Classes_Update;
}

class Admin_Classes_Update : public QMainWindow
{
    Q_OBJECT

    public:
        explicit Admin_Classes_Update(QWidget *parent = nullptr);
        ~Admin_Classes_Update();

    protected:
        void moveEvent(QMoveEvent *event) override;

    private:
        Ui::Admin_Classes_Update *ui;

        // Reference the class files here
        Admin_Students_List *admin_students_list;
        Admin_Classes_List *admin_classes_list;
        Admin_Lecturers_List *admin_lecturers_list;

        // All custom functions() should be recognized upon here
        void switchWindow_AdminStudentsList();
        void switchWindow_AdminClassesList();
        void switchWindow_AdminLecturersList();
};

#endif // ADMIN_CLASSES_UPDATE_H
