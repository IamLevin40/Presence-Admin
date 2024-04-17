#ifndef ADMIN_LECTURERS_LIST_H
#define ADMIN_LECTURERS_LIST_H

#include <QMainWindow>

namespace Ui {
class Admin_Lecturers_List;
}

class Admin_Lecturers_List : public QMainWindow
{
    Q_OBJECT

public:
    explicit Admin_Lecturers_List(QWidget *parent = nullptr);
    ~Admin_Lecturers_List();

private:
    Ui::Admin_Lecturers_List *ui;
};

#endif // ADMIN_LECTURERS_LIST_H
