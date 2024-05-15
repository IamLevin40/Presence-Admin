#ifndef ADMIN_STUDENTS_LIST_H
#define ADMIN_STUDENTS_LIST_H

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
class Admin_Classes_List;
class Admin_Lecturers_List;
class Admin_Students_Enroll;
class Admin_Students_Update;
class Admin_Students_Send_Email;


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

    private slots:
        void collegeIndexChanged(int index);

    private:
        Ui::Admin_Students_List *ui;
        QSqlDatabase database;
        QList<QGroupBox*> groupBoxList;

        // Reference the class files here
        Admin_Login_Form *admin_login_form;
        Admin_Classes_List *admin_classes_list;
        Admin_Lecturers_List *admin_lecturers_list;
        Admin_Students_Enroll *admin_students_enroll;
        Admin_Students_Update *admin_students_update;
        Admin_Students_Send_Email *admin_students_send_email;

        // All custom functions() should be recognized upon here
        void filterSearchCall();
        void selectStudentInfo(const int &pageNumber, const QString &college, const QString &program,
                               const QString &year, const QString &section, const QString &isRegular,
                               const QString &searchText);
        void displayStudentInfo(const QList<QStringList> &dataList);

        void displayDeleteWarning(const QString &key_studentId);
        void deleteStudentInfo(const QString &key_studentId);
        void disregardDelete();

        void populateCombobox();

        void switchWindow_AdminLoginForm();
        void switchWindow_AdminClassesList();
        void switchWindow_AdminLecturersList();
        void switchWindow_AdminStudentsEnroll();
        void switchWindow_AdminStudentsUpdate(const QString &key_studentId);
        void switchWindow_AdminStudentsSendEmail(const QString &key_studentId);
};

#endif // ADMIN_STUDENTS_LIST_H


/*
 *   Copyright (c) 2024 - WVSU-CICT BSCS 1B: Batch 2027
 *   All rights reserved.
 */
