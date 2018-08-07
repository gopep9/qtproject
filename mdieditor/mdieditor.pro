QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


HEADERS += \
    editor.h \
    mainwindow.h \
    replacedialog.h \
    gotodialog.h \
    finddialog.h

SOURCES += \
    editor.cpp \
    mainwindow.cpp \
    main.cpp \
    replacedialog.cpp \
    gotodialog.cpp \
    finddialog.cpp

RESOURCES += \
    mdieditor.qrc

FORMS += \
    replacedialog.ui \
    gotodialog.ui \
    finddialog.ui
