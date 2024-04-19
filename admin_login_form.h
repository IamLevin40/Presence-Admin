#ifndef ADMIN_LOGIN_FORM_H
#define ADMIN_LOGIN_FORM_H

#include <QMainWindow>
#include <QMoveEvent>


// Forward declaration of class files
class Admin_Students_List;


namespace Ui {
    class Admin_Login_Form;
}

class Admin_Login_Form : public QMainWindow
{
    Q_OBJECT

    public:
        explicit Admin_Login_Form(QWidget *parent = nullptr);
        ~Admin_Login_Form();

    protected:
        void moveEvent(QMoveEvent *event) override;

    private:
        Ui::Admin_Login_Form *ui;

        // Reference the class files here
        Admin_Students_List *admin_students_list;

        // All custom functions() should be recognized upon here
        void loginCall();
        QString authenticateAdmin(const QString &adminId, const QString &adminPin);
        void switchWindow_AdminStudentsList();
};

#endif // ADMIN_LOGIN_FORM_H
