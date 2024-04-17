#ifndef ADMIN_CLASSES_UPDATE_H
#define ADMIN_CLASSES_UPDATE_H

#include <QMainWindow>

namespace Ui {
class Admin_Classes_Update;
}

class Admin_Classes_Update : public QMainWindow
{
    Q_OBJECT

public:
    explicit Admin_Classes_Update(QWidget *parent = nullptr);
    ~Admin_Classes_Update();

private:
    Ui::Admin_Classes_Update *ui;
};

#endif // ADMIN_CLASSES_UPDATE_H
