QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    admin_classes_create.cpp \
    admin_classes_list.cpp \
    admin_classes_students_add.cpp \
    admin_classes_students_list.cpp \
    admin_classes_update.cpp \
    admin_lecturers_add.cpp \
    admin_lecturers_list.cpp \
    admin_lecturers_update.cpp \
    admin_login_form.cpp \
    admin_students_enroll.cpp \
    admin_students_list.cpp \
    admin_students_update.cpp \
    global_variables.cpp \
    main.cpp

HEADERS += \
    admin_classes_create.h \
    admin_classes_list.h \
    admin_classes_students_add.h \
    admin_classes_students_list.h \
    admin_classes_update.h \
    admin_lecturers_add.h \
    admin_lecturers_list.h \
    admin_lecturers_update.h \
    admin_login_form.h \
    admin_students_enroll.h \
    admin_students_list.h \
    admin_students_update.h \
    global_variables.h

FORMS += \
    admin_classes_create.ui \
    admin_classes_list.ui \
    admin_classes_students_add.ui \
    admin_classes_students_list.ui \
    admin_classes_update.ui \
    admin_lecturers_add.ui \
    admin_lecturers_list.ui \
    admin_lecturers_update.ui \
    admin_login_form.ui \
    admin_students_enroll.ui \
    admin_students_list.ui \
    admin_students_update.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
