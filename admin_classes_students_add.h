#ifndef ADMIN_CLASSES_STUDENTS_ADD_H
#define ADMIN_CLASSES_STUDENTS_ADD_H

#include <QMainWindow>
#include <QMoveEvent>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>


// Forward declaration of class files
class Admin_Students_List;
class Admin_Classes_List;
class Admin_Lecturers_List;
class Admin_Classes_Students_List;


namespace Ui {
    class Admin_Classes_Students_Add;
}

class Admin_Classes_Students_Add : public QMainWindow
{
    Q_OBJECT

    public:
        explicit Admin_Classes_Students_Add(QWidget *parent = nullptr);
        ~Admin_Classes_Students_Add();

    protected:
        void moveEvent(QMoveEvent *event) override;

    private:
        Ui::Admin_Classes_Students_Add *ui;
        QSqlDatabase database;

        // Reference the class files here
        Admin_Students_List *admin_students_list;
        Admin_Classes_List *admin_classes_list;
        Admin_Lecturers_List *admin_lecturers_list;
        Admin_Classes_Students_List *admin_classes_students_list;

        // All custom functions() should be recognized upon here
        void classStudentAddCall();
        QString verifyClassStudentAdd(const QString &studentId);

        void insertStudentToClass(const QString &studentId, const QStringList &keys_classInfo);

        void switchWindow_AdminStudentsList();
        void switchWindow_AdminClassesList();
        void switchWindow_AdminLecturersList();
        void switchWindow_AdminClassesStudentsList();
};

#endif // ADMIN_CLASSES_STUDENTS_ADD_H


/*
 *   Copyright (c) 2024 - WVSU-CICT BSCS 1B: Batch 2027
 *   All rights reserved.
 */
