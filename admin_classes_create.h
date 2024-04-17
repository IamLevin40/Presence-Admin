#ifndef ADMIN_CLASSES_CREATE_H
#define ADMIN_CLASSES_CREATE_H

#include <QMainWindow>

namespace Ui {
class Admin_Classes_Create;
}

class Admin_Classes_Create : public QMainWindow
{
    Q_OBJECT

public:
    explicit Admin_Classes_Create(QWidget *parent = nullptr);
    ~Admin_Classes_Create();

private:
    Ui::Admin_Classes_Create *ui;
};

#endif // ADMIN_CLASSES_CREATE_H
