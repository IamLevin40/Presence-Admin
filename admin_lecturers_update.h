#ifndef ADMIN_LECTURERS_UPDATE_H
#define ADMIN_LECTURERS_UPDATE_H

#include <QMainWindow>

namespace Ui {
class Admin_Lecturers_Update;
}

class Admin_Lecturers_Update : public QMainWindow
{
    Q_OBJECT

public:
    explicit Admin_Lecturers_Update(QWidget *parent = nullptr);
    ~Admin_Lecturers_Update();

private:
    Ui::Admin_Lecturers_Update *ui;
};

#endif // ADMIN_LECTURERS_UPDATE_H
