#ifndef ADMIN_CLASSES_CREATE_H
#define ADMIN_CLASSES_CREATE_H

#include <QMainWindow>
#include <QMoveEvent>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVector>
#include <QChar>


// Forward declaration of class files
class Admin_Students_List;
class Admin_Classes_List;
class Admin_Lecturers_List;


namespace Ui {
    class Admin_Classes_Create;
}

class Admin_Classes_Create : public QMainWindow
{
    Q_OBJECT

    public:
        explicit Admin_Classes_Create(QWidget *parent = nullptr);
        ~Admin_Classes_Create();

    protected:
        void moveEvent(QMoveEvent *event) override;

    private:
        Ui::Admin_Classes_Create *ui;
        QSqlDatabase database;

        // Reference the class files here
        Admin_Students_List *admin_students_list;
        Admin_Classes_List *admin_classes_list;
        Admin_Lecturers_List *admin_lecturers_list;

        // All custom functions() should be recognized upon here
        void classCreateCall();
        QString verifyClassCreate(const QString &subjectCode, const QString &subjectDesc,
                                  const QString &schoolYear, const QString &semester, const QString &program,
                                  const QString &year, const QString &section, const QString &room, const QString &lecturerId,
                                  const QString &firstDay, const QString &firstStartTime, const QString &firstEndTime,
                                  const QString &secondDay, const QString &secondStartTime, const QString &secondEndTime,
                                  const QString &thirdDay, const QString &thirdStartTime, const QString &thirdEndTime);

        void insertClassData(const QString &subjectCode, const QString &subjectDesc,
                             const QString &schoolYear, const QString &semester, const QString &program,
                             const QString &year, const QString &section, const QString &room, const QString &lecturerId,
                             const QString &firstDay, const QString &firstStartTime, const QString &firstEndTime,
                             const QString &secondDay, const QString &secondStartTime, const QString &secondEndTime,
                             const QString &thirdDay, const QString &thirdStartTime, const QString &thirdEndTime);
        void addStudentsToClass(const QString &tableName, const QString &program, const QString &year, const QString &section);

        void populateCombobox();

        void switchWindow_AdminStudentsList();
        void switchWindow_AdminClassesList();
        void switchWindow_AdminLecturersList();
};

#endif // ADMIN_CLASSES_CREATE_H


/*
 *   Copyright (c) 2024 - WVSU-CICT BSCS 1B: Batch 2027
 *   All rights reserved.
 */
