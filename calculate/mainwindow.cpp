#include "mainwindow.h"
#include "calculatewidget.h"
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QtWidgets>
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    createActions();
    createMenus();
    calculateWidget=new CalculateWidget(this);
    this->setCentralWidget(calculateWidget);
    this->setGeometry(100,100,300,400);
}

void MainWindow::createActions()
{
    about=new QAction("&AboutQt",this);
    connect(about,SIGNAL(triggered(bool)),qApp,SLOT(aboutQt()));
}

void MainWindow::createMenus()
{
    helpMenu=menuBar()->addMenu("&help");
    helpMenu->addAction(about);
}
