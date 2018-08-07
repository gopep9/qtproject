#include"igo.h"
#include<QApplication>
#include<mainwindow.h>
#include <QMessageBox>
int main(int argc,char **argv)
{
    QApplication app(argc,argv);
//    QMessageBox::warning(0,"","");
    MainWindow *w=new MainWindow;
    w->show();
    return app.exec();
}
