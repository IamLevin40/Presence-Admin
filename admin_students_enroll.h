#ifndef ADMIN_STUDENTS_ENROLL_H
#define ADMIN_STUDENTS_ENROLL_H

#include <QMainWindow>

namespace Ui {
class Admin_Students_Enroll;
}

class Admin_Students_Enroll : public QMainWindow
{
    Q_OBJECT

public:
    explicit Admin_Students_Enroll(QWidget *parent = nullptr);
    ~Admin_Students_Enroll();

private:
    Ui::Admin_Students_Enroll *ui;
};

#endif // ADMIN_STUDENTS_ENROLL_H
