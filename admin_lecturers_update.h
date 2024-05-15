#ifndef ADMIN_LECTURERS_UPDATE_H
#define ADMIN_LECTURERS_UPDATE_H

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


namespace Ui {
    class Admin_Lecturers_Update;
}

class Admin_Lecturers_Update : public QMainWindow
{
    Q_OBJECT

    public:
        explicit Admin_Lecturers_Update(QWidget *parent = nullptr);
        ~Admin_Lecturers_Update();

    protected:
        void moveEvent(QMoveEvent *event) override;

    private:
        Ui::Admin_Lecturers_Update *ui;
        QSqlDatabase database;

        // Reference the class files here
        Admin_Students_List *admin_students_list;
        Admin_Classes_List *admin_classes_list;
        Admin_Lecturers_List *admin_lecturers_list;

        // All custom functions() should be recognized upon here
        void selectLecturerInfo(const QString &key_lecturerId);
        void updateField(const QStringList &dataList);

        void lecturerUpdateCall();
        QString verifyLecturerUpdate(const QString &lastName, const QString &firstName, const QString &college, const QString &pin);
        void updateLecturerInfo(const QString &lastName, const QString &firstName, const QString &college, const QString &pin);

        void populateCombobox();

        void switchWindow_AdminStudentsList();
        void switchWindow_AdminClassesList();
        void switchWindow_AdminLecturersList();
};

#endif // ADMIN_LECTURERS_UPDATE_H


/*
 *   Copyright (c) 2024 - WVSU-CICT BSCS 1B: Batch 2027
 *   All rights reserved.
 */
