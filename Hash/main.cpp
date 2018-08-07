#include"form.h"
#include<QApplication>

int main(int argc,char*argv[])
{
    QApplication *app=new QApplication(argc,argv);
    Form *w=Form::sharedInstance();
    w->show();
    return app->exec();
}
