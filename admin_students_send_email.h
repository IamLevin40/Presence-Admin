#ifndef ADMIN_STUDENTS_SEND_EMAIL_H
#define ADMIN_STUDENTS_SEND_EMAIL_H

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
    class Admin_Students_Send_Email;
}

class Admin_Students_Send_Email : public QMainWindow
{
    Q_OBJECT

    public:
        explicit Admin_Students_Send_Email(QWidget *parent = nullptr);
        ~Admin_Students_Send_Email();

    protected:
        void moveEvent(QMoveEvent *event) override;

    private:
        Ui::Admin_Students_Send_Email *ui;
        QSqlDatabase database;

        // Reference the class files here
        Admin_Students_List *admin_students_list;
        Admin_Classes_List *admin_classes_list;
        Admin_Lecturers_List *admin_lecturers_list;

        // All custom functions() should be recognized upon here
        void selectStudentInfo(const QString &key_studentId);
        void pregenerateEmail(const QStringList &dataList);

        void sendEmailCall();
        QString verifyEmail(const QString &adminGmail, const QString &adminPassword,
                            const QString &recipient, const QString &subject, const QString &body);

        void switchWindow_AdminStudentsList();
        void switchWindow_AdminClassesList();
        void switchWindow_AdminLecturersList();
};

#endif // ADMIN_STUDENTS_SEND_EMAIL_H
