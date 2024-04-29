#ifndef ADMIN_STUDENTS_UPDATE_H
#define ADMIN_STUDENTS_UPDATE_H

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
    class Admin_Students_Update;
}

class Admin_Students_Update : public QMainWindow
{
    Q_OBJECT

    public:
        explicit Admin_Students_Update(QWidget *parent = nullptr);
        ~Admin_Students_Update();
        QString regularity;

    protected:
        void moveEvent(QMoveEvent *event) override;

    private slots:
        void collegeIndexChanged(int index);

    private:
        Ui::Admin_Students_Update *ui;
        QSqlDatabase database;

        // Reference the class files here
        Admin_Students_List *admin_students_List;
        Admin_Classes_List *admin_classes_list;
        Admin_Lecturers_List *admin_lecturers_list;

        // All custom functions() should be recognized upon here
        void selectDataFromDatabase(const QString &key_studentId);
        void displayDataFromDatabase(const QStringList &dataList);

        void studentUpdateCall();
        QString verifyStudentUpdate(const QString &lastname, const QString &firstName,
                                    const QString &college, const QString &program,
                                    const QString &year, const QString &section, const QString &pin);
        void updateDataFromDatabase(const QString &lastname, const QString &firstName,
                                    const QString &college, const QString &program,
                                    const QString &year, const QString &section, const QString &pin);

        void handleIsRegularRadio(const QString &option);
        void populateCombobox();

        void switchWindow_AdminStudentsList();
        void switchWindow_AdminClassesList();
        void switchWindow_AdminLecturersList();
};

#endif // ADMIN_STUDENTS_UPDATE_H
