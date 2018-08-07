CONFIG += C++11
Qt += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TEMPLATE = app
SOURCES += \
    main.cpp \
    mainwindow.cpp \
    calculatewidget.cpp

HEADERS += \
    mainwindow.h \
    calculatewidget.h

FORMS +=
