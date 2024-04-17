#ifndef ADMIN_LOGIN_FORM_H
#define ADMIN_LOGIN_FORM_H

#include <QMainWindow>

namespace Ui {
class Admin_Login_Form;
}

class Admin_Login_Form : public QMainWindow
{
    Q_OBJECT

public:
    explicit Admin_Login_Form(QWidget *parent = nullptr);
    ~Admin_Login_Form();

private:
    Ui::Admin_Login_Form *ui;
};

#endif // ADMIN_LOGIN_FORM_H
