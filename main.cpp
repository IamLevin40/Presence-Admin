#include "global_main.h"
#include "admin_classes_create.h"
#include "admin_classes_list.h"
#include "admin_classes_students_add.h"
#include "admin_classes_students_list.h"
#include "admin_classes_update.h"
#include "admin_lecturers_add.h"
#include "admin_lecturers_list.h"
#include "admin_lecturers_update.h"
#include "admin_login_form.h"
#include "admin_students_enroll.h"
#include "admin_students_list.h"
#include "admin_students_update.h"
#include "ui_design_area.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Admin_Login_Form w;
    w.show();

    return a.exec();
}


/*
 *   Copyright (c) 2024 - WVSU-CICT BSCS 1B: Batch 2027
 *   All rights reserved.
 */
