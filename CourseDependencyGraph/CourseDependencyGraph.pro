#-------------------------------------------------
#
# Project created by QtCreator 2018-11-21T10:27:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CourseDependencyGraph
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        ui/mainwindow.cpp \
    sources/AVLTree.cpp \
    sources/Semester.cpp \
    sources/Relationship.cpp \
    sources/CourseCode.cpp \
    sources/Course.cpp \
    utilities/CSVLoader.cpp \
    utilities/DependencyManager.cpp \
    sources/AVLTree.tpp \
    ui/CourseInfoWidget.cpp \
    ui/CourseLabel.cpp
    sources/DependencyGraph.cpp

HEADERS += \
        ui/mainwindow.h \
    sources/Course.h \
    sources/CourseCode.h \
    sources/Semester.h \
    sources/AVLTree.h \
    sources/Relationship.h \
    utilities/CSVLoader.h \
    utilities/DependencyManager.h \
    ui/CourseInfoWidget.h \
    ui/CourseLabel.h
    sources/DependencyGraph.h

FORMS += \
        ui/mainwindow.ui \
    ui/CourseInfoWidget.ui \
    ui/CourseLabel.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
