#ifndef ADMIN_LECTURERS_ADD_H
#define ADMIN_LECTURERS_ADD_H

#include <QMainWindow>

namespace Ui {
class Admin_Lecturers_Add;
}

class Admin_Lecturers_Add : public QMainWindow
{
    Q_OBJECT

public:
    explicit Admin_Lecturers_Add(QWidget *parent = nullptr);
    ~Admin_Lecturers_Add();

private:
    Ui::Admin_Lecturers_Add *ui;
};

#endif // ADMIN_LECTURERS_ADD_H
