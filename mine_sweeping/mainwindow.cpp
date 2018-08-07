#include "mainwindow.h"
#include "minesweeping.h"
#include "customdialog.h"
#include<QWidget>
#include<QAction>
#include<QMenu>
#include<QMenuBar>
#include <QLCDNumber>
#include <QPushButton>
#include <QVBoxLayout>
#include<QHBoxLayout>
#include<QApplication>
#include<QTime>
#include<QTimer>
#include<QStatusBar>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    mineSweepingWidget=new mineSweeping(this);
    mainWidget=new QWidget;
    leaveMineCountLcd=new QLCDNumber;
    timeLcd=new QLCDNumber;
    restart=new QPushButton;
    restart->setIcon(QPixmap("://images/mine.png"));
    time=new QTime(0,0,0,0);
    timer=new QTimer(this);

    restart->setMaximumSize(50,50);
    restart->setMinimumSize(50,50);

    leaveMineCountLcd->setMaximumSize(100,50);
    leaveMineCountLcd->setMinimumSize(100,50);
    leaveMineCountLcd->setDigitCount(10);
    leaveMineCountLcd->setMode(QLCDNumber::Dec);
    leaveMineCountLcd->setSegmentStyle(QLCDNumber::Flat);

    timeLcd->setMaximumSize(100,50);
    timeLcd->setMinimumSize(100,50);
    timeLcd->setDigitCount(10);
    timeLcd->setMode(QLCDNumber::Dec);
    timeLcd->setSegmentStyle(QLCDNumber::Flat);

    hLayout=new QHBoxLayout;
    vLayout=new QVBoxLayout;
    hLayout->addWidget(leaveMineCountLcd,0);
    hLayout->addSpacing(10);
    hLayout->addWidget(restart,0);
    hLayout->addSpacing(10);
    hLayout->addWidget(timeLcd,0);
    vLayout->addLayout(hLayout);
    vLayout->addWidget(mineSweepingWidget);
    mainWidget->setLayout(vLayout);
    this->setCentralWidget(mainWidget);

    createActions();
    createMenus();

    connect(timer,SIGNAL(timeout()),this,SLOT(timeout()));
    connect(restart,SIGNAL(clicked()),this,SLOT(newgame()));

    easy();
}

void MainWindow::newgame()
{
    vLayout->removeWidget(mineSweepingWidget);
    delete mineSweepingWidget;
    setFixedSize(0,0);
    mineSweepingWidget=new mineSweeping(this);
    mineSweepingWidget->setColumn(column);
    mineSweepingWidget->setRow(row);
    mineSweepingWidget->setMineCount(mine);

    connect(mineSweepingWidget,SIGNAL(click()),this,SLOT(click()));
    connect(mineSweepingWidget,SIGNAL(win()),this,SLOT(win()));
    connect(mineSweepingWidget,SIGNAL(lose()),this,SLOT(lose()));

    mineSweepingWidget->newgame();
    vLayout->addWidget(mineSweepingWidget);

    (*time)=QTime(0,0);
    timer->setInterval(1000);
    timer->start();
    timeLcd->display(time->toString("mm:ss"));

    click();
//    update();

}

void MainWindow::easy()
{
    mineSweepingWidget->setColumn(9);
    mineSweepingWidget->setRow(9);
    mineSweepingWidget->setMineCount(10);
    column=9,row=9,mine=10;
    newgame();
}

void MainWindow::normal()
{
    mineSweepingWidget->setColumn(16);
    mineSweepingWidget->setRow(16);
    mineSweepingWidget->setMineCount(40);
    column=16,row=16,mine=40;
    newgame();
}

void MainWindow::hard()
{
    mineSweepingWidget->setColumn(30);
    mineSweepingWidget->setRow(16);
    mineSweepingWidget->setMineCount(99);
    column=30,row=16,mine=99;
    newgame();
}

void MainWindow::custom()
{
    customDialog dialog;
    if(dialog.exec())
    {
        if(dialog.row()*dialog.column()<dialog.mine())
        {
            return;
        }
        else
        {
            mineSweepingWidget->setColumn(dialog.column());
            mineSweepingWidget->setRow(dialog.row());
            mineSweepingWidget->setMineCount(dialog.mine());
            column=dialog.column();
            row=dialog.row();
            mine=dialog.mine();

            newgame();
        }
    }
    return;
}

void MainWindow::timeout()
{
    *time=time->addSecs(1);
    timeLcd->display(time->toString("mm:ss"));
    update();
}

void MainWindow::win()
{
    statusBar()->showMessage(tr("You win"),3000);
    timer->stop();\
}

void MainWindow::lose()
{
    statusBar()->showMessage(tr("You lose"),3000);
    timer->stop();
}

void MainWindow::click()
{
    leaveMineCountLcd->display(mine-mineSweepingWidget->signCount());
}

void MainWindow::createActions()
{
    newAction=new QAction(tr("&New"),this);
    newAction->setShortcut(tr("F2"));
    connect(newAction,SIGNAL(triggered()),this,SLOT(newgame()));

    easyAction=new QAction(tr("&Easy"),this);
    connect(easyAction,SIGNAL(triggered()),this,SLOT(easy()));

    normalAction=new QAction(tr("Nor&mal"),this);
    connect(normalAction,SIGNAL(triggered()),this,SLOT(normal()));

    hardAction=new QAction(tr("&Hard"),this);
    connect(hardAction,SIGNAL(triggered()),this,SLOT(hard()));

    customAction=new QAction(tr("&Custom"),this);
    connect(customAction,SIGNAL(triggered()),this,SLOT(custom()));

    closeAction=new QAction(tr("E&xit"),this);
    connect(closeAction,SIGNAL(triggered()),this,SLOT(close()));

    aboutQtAction=new QAction(tr("&About Qt"),this);
    connect(aboutQtAction,SIGNAL(triggered()),qApp,SLOT(aboutQt()));
}

void MainWindow::createMenus()
{
    fileMenu=menuBar()->addMenu("&Game");
    fileMenu->addAction(newAction);
    fileMenu->addAction(easyAction);
    fileMenu->addAction(normalAction);
    fileMenu->addAction(hardAction);
    fileMenu->addAction(customAction);
    fileMenu->addAction(closeAction);

    helpMenu=menuBar()->addMenu("&Help");
    helpMenu->addAction(aboutQtAction);
}
