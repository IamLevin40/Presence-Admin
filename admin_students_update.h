#ifndef ADMIN_STUDENTS_UPDATE_H
#define ADMIN_STUDENTS_UPDATE_H

#include <QMainWindow>

namespace Ui {
class Admin_Students_Update;
}

class Admin_Students_Update : public QMainWindow
{
    Q_OBJECT

public:
    explicit Admin_Students_Update(QWidget *parent = nullptr);
    ~Admin_Students_Update();

private:
    Ui::Admin_Students_Update *ui;
};

#endif // ADMIN_STUDENTS_UPDATE_H
