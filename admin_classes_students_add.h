#ifndef ADMIN_CLASSES_STUDENTS_ADD_H
#define ADMIN_CLASSES_STUDENTS_ADD_H

#include <QMainWindow>

namespace Ui {
class Admin_Classes_Students_Add;
}

class Admin_Classes_Students_Add : public QMainWindow
{
    Q_OBJECT

public:
    explicit Admin_Classes_Students_Add(QWidget *parent = nullptr);
    ~Admin_Classes_Students_Add();

private:
    Ui::Admin_Classes_Students_Add *ui;
};

#endif // ADMIN_CLASSES_STUDENTS_ADD_H
