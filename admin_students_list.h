#ifndef ADMIN_STUDENTS_LIST_H
#define ADMIN_STUDENTS_LIST_H

#include <QMainWindow>

namespace Ui {
class Admin_Students_List;
}

class Admin_Students_List : public QMainWindow
{
    Q_OBJECT

public:
    explicit Admin_Students_List(QWidget *parent = nullptr);
    ~Admin_Students_List();

private:
    Ui::Admin_Students_List *ui;
};

#endif // ADMIN_STUDENTS_LIST_H
