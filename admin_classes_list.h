#ifndef ADMIN_CLASSES_LIST_H
#define ADMIN_CLASSES_LIST_H

#include <QMainWindow>

namespace Ui {
class Admin_Classes_List;
}

class Admin_Classes_List : public QMainWindow
{
    Q_OBJECT

public:
    explicit Admin_Classes_List(QWidget *parent = nullptr);
    ~Admin_Classes_List();

private:
    Ui::Admin_Classes_List *ui;
};

#endif // ADMIN_CLASSES_LIST_H
