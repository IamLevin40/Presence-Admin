#ifndef ADMIN_CLASSES_STUDENTS_LIST_H
#define ADMIN_CLASSES_STUDENTS_LIST_H

#include <QMainWindow>

namespace Ui {
class Admin_Classes_Students_List;
}

class Admin_Classes_Students_List : public QMainWindow
{
    Q_OBJECT

public:
    explicit Admin_Classes_Students_List(QWidget *parent = nullptr);
    ~Admin_Classes_Students_List();

private:
    Ui::Admin_Classes_Students_List *ui;
};

#endif // ADMIN_CLASSES_STUDENTS_LIST_H
