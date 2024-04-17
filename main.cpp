#include "admin_login_form.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Admin_Login_Form w;
    w.show();
    return a.exec();
}
