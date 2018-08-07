#include "mainwindow.h"
#include <QApplication>
#include "area.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Area w;
    w.show();

    return a.exec();
}
