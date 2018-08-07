#include "mainwindow.h"
#include "igo.h"
#include "myserver.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QPainter>
#include <QAction>
#include <QPainter>
#include <QPaintEvent>
#include <QPalette>
#include <cmath>
#include <algorithm>
#include <QPushButton>
#include <QTime>
#include <QLCDNumber>
#include <QTimer>
#include <QListWidget>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QApplication>
#include <QStatusBar>
#include <QInputDialog>
#include <QMessageBox>
#include <QDebug>
//#include <QInputDialog>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),igowidget(new Igo)
{
    setCentralWidget(igowidget);
    createActions();
    createMenus();
    createStatusBar();
    server=new MyServer(this);
//    tcpClient=new QTcpSocket(this);
//    receivedSocket=new QTcpSocket(this);
//    igowidget->setNoAccept();
}

void MainWindow::paintEvent(QPaintEvent */*event*/)
{
    QPainter painter(this);
    QPalette palette;
    palette.setColor(QPalette::Background,QColor(192,121,51,255));
    this->setPalette(palette);
}

void MainWindow::closeEvent(QCloseEvent */*event*/)
{
    if(isItOnLine)
    {
        offLine();
    }
}


void MainWindow::showHistory()
{
    igowidget->listWidget->show();
}

bool MainWindow::save()
{
    QString fileName=QFileDialog::getSaveFileName(this,tr("Save igo"),".",tr("igo files(*.igo)"));
    if(fileName.isEmpty())
    {
        statusBar()->showMessage(tr("Save fail"),3000);
        return false;
    }
    if( igowidget->saveFile(fileName))
    {
        statusBar()->showMessage(tr("Save success"),3000);
        return true;
    }
    statusBar()->showMessage(tr("Save fail"),3000);
    return false;
}

bool MainWindow::load()
{
    igowidget->newgame();
    QString fileName=QFileDialog::getOpenFileName(this,tr("Load igo"),".",tr("igo files(*.igo)"));
    if(fileName.isEmpty())
    {
        statusBar()->showMessage(tr("Load fail"),3000);
        return false;
    }
    if(igowidget->loadFile(fileName))
    {
        statusBar()->showMessage(tr("Load success"),3000);
        return true;
    }
    statusBar()->showMessage(tr("Load fail"),3000);
    return false;
}

void MainWindow::setmessage(QString str)
{
    message->setText(str);
}

void MainWindow::asServer()
{
    if(isItOnLine)
    {
        return;
    }
//    offLine();
    bool isItOk;
    int port=QInputDialog::getInt(this,"Enter port","port: ",this->port,1,65535,1,&isItOk);
    if(!isItOk)
        return;
//    isItOnLine=true;
//    back->setEnabled(false);
//    front->setEnabled(false);

    this->port=port;
    server->listen(QHostAddress::Any,port);
    tcpClient=0;
//    connect(tcpClient,SIGNAL(disconnected()),tcpClient,SLOT(deleteLater()));
//    connect(tcpClient,SIGNAL(disconnected()),this,SLOT(disconnectDialog()));
//    connect(server,SIGNAL(newConnection()),this,SLOT(acceptConnection()));
    connect(server,SIGNAL(acceptConnection(qintptr)),this,SLOT(acceptConnection(qintptr)));
    isItOnLine=true;
    back->setEnabled(false);
    front->setEnabled(false);
    newAction->setEnabled(false);
    loadAction->setEnabled(false);
    asServerAction->setEnabled(false);
    asClientAction->setEnabled(false);
}

void MainWindow::asClient()
{
//    offLine();
    if(isItOnLine)
    {
        return;
    }
    bool isItOk1,isItOk2,isItOk3;
    QString hostIp=QInputDialog::getText(this,"Enter host ip","host ip: ",QLineEdit::Normal,this->ipAddress,&isItOk1);
    int port=QInputDialog::getInt(this,"Enter port","port: ",this->port,1,65535,1,&isItOk2);
    if(!(isItOk1&&isItOk2))
        return;
    this->port=port;
    this->ipAddress=hostIp;
    tcpClient=new QTcpSocket(this);
    connect(tcpClient,SIGNAL(disconnected()),tcpClient,SLOT(deleteLater()));
    tcpClient->connectToHost(hostIp,port);
    isItOk3=tcpClient->waitForConnected(3000);
    if(!isItOk3)
    {
        tcpClient->close();
        tcpClient->waitForDisconnected();
        return;
    }
    connect(tcpClient,SIGNAL(disconnected()),this,SLOT(disconnectDialog()));
    connect(tcpClient,SIGNAL(readyRead()),this,SLOT(readClient()));
    connect(igowidget,SIGNAL(addPieceSignal(int,int)),this,SLOT(send(int,int)));
    igowidget->setNoAccept();
    isItOnLine=true;
    back->setEnabled(false);
    front->setEnabled(false);
    newAction->setEnabled(false);
    loadAction->setEnabled(false);
    asServerAction->setEnabled(false);
    asClientAction->setEnabled(false);
}

void MainWindow::acceptConnection(qintptr socketDescriptor)
{
//    delete tcpClient;
//    tcpClient->close();
    tcpClient=new QTcpSocket(this);
    tcpClient->setSocketDescriptor(socketDescriptor);
    int button=QMessageBox::question(this,"accept connect",
        QString("accept connect? from:")+tcpClient->peerAddress().toString(),QMessageBox::Ok,QMessageBox::No);
    if(button==QMessageBox::Ok)
    {
        igowidget->setNoAccept();
        button=QMessageBox::question(this,"select close","select close","black","white");
//        if(button==0)
//        {
//            send(-2,-2);
//        }
//        else
//        {
//            send(-2,-3);
//        }
    }
    else
    {
        tcpClient->close();
        return;
    }
    connect(tcpClient,SIGNAL(disconnected()),tcpClient,SLOT(deleteLater()));
    connect(tcpClient,SIGNAL(disconnected()),this,SLOT(disconnectDialog()));
    connect(tcpClient,SIGNAL(readyRead()),this,SLOT(readClient()));
    connect(igowidget,SIGNAL(addPieceSignal(int,int)),this,SLOT(send(int,int)));
    if(button==0)
    {
        send(-2,-2);
        setmessage("another player is ready,you first");
        igowidget->newgame();
        igowidget->setAcceptAble();
    }
    else
    {
        send(-2,-3);
        setmessage("another player is ready,you secode");
        igowidget->newgame();
    }
}

void MainWindow::send(int x,int y)
{
    if(tcpClient->isValid()){
        QByteArray outBlock;
        QDataStream out(&outBlock,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);
        out<<x<<y;
        tcpClient->write(outBlock);
        tcpClient->waitForBytesWritten();
        igowidget->setNoAccept();
    }
}

void MainWindow::readClient()
{
    QDataStream in(tcpClient);
    in.setVersion(QDataStream::Qt_4_8);
    int x,y;
    in>>x>>y;
    if(x==-2)
    {
        igowidget->newgame();
        if(y==-3)
        {
            igowidget->setAcceptAble();
            setmessage("another player is ready,you first");
        }
        else if(y==-2)
        {
            igowidget->setNoAccept();
            setmessage("another player is ready,you second");
        }
//        QMessageBox::information(this,"connect","connect");
        return;
    }
    else if(x==-1)
    {
        offLine();
        QMessageBox::warning(this,"warning","Other player offLine");
        return;
    }
    else if(x==-3)
    {
        QMessageBox::information(this,"another is lose","another is lose");
        igowidget->setNoAccept();
        return;
    }
    igowidget->addPieceFromNet(x,y);
    igowidget->setAcceptAble();
}

void MainWindow::onLine()
{
//    offLine();
//    igowidget->setNoAccept();
//    server=new QTcpServer(this);
//    tcpClient=new QTcpSocket(this);
//    receivedSocket=new QTcpSocket(this);
//    isItOnLine=true;
//    igowidget->newgame();
//    QString hostIp=QInputDialog::getText(this,"Enter host ip","host ip: ",QLineEdit::Normal,ipAddress);
//    int port=QInputDialog::getInt(this,"Enter port","port: ",this->port);
//    this->port=port;
//    ipAddress=hostIp;

//    back->setEnabled(false);
//    front->setEnabled(false);
//    newAction->setEnabled(false);
//    loadAction->setEnabled(false);
//    server->listen(QHostAddress::Any,port);
//    connect(server,SIGNAL(newConnection()),this,SLOT(acceptConnection()));

//    tcpClient->connectToHost(hostIp,port);
//    connect(igowidget,SIGNAL(addPieceSignal(int,int)),this,SLOT(send(int,int)));
}

void MainWindow::offLine()
{
    if(isItOnLine)
    {
        isItOnLine=false;
//        send(-1,-1);
//        this->sleep(1000);

        back->setEnabled(true);
        front->setEnabled(true);
        newAction->setEnabled(true);
        loadAction->setEnabled(true);
        asServerAction->setEnabled(true);
        asClientAction->setEnabled(true);
        disconnect(server,SIGNAL(acceptConnection(qintptr)),this,SLOT(acceptConnection(qintptr)));
//        disconnect(server,SIGNAL(newConnection()),this,SLOT(acceptConnection(qintptr)));
        disconnect(igowidget,SIGNAL(addPieceSignal(int,int)),this,SLOT(send(int,int)));
        if(tcpClient)
        disconnect(tcpClient,SIGNAL(readyRead()),this,SLOT(readClient()));
        igowidget->setAcceptAble();
        if(server->isListening())
            server->close();
//        server->disconnect(1);
//        tcpClient->disconnectFromHost();
        if(tcpClient&&tcpClient->isOpen()){
            tcpClient->close();
//            tcpClient->waitForDisconnected();
        }
        tcpClient=0;
//    QTcpServer *tmp1=server;
//    QTcpSocket *tmp2=tcpClient;
//        server=new QTcpServer(this);
//        tcpClient=new QTcpSocket(this);
//    delete tmp1;
//    delete tmp2;
    }
}



void MainWindow::disconnectDialog()
{
    offLine();
    QMessageBox::warning(this,"disconnect","disconnect");
}

void MainWindow::capitulate()
{
    if(!isItOnLine)
        return;
    send(-3,0);
    igowidget->setNoAccept();
}

void MainWindow::sleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void MainWindow::createActions()
{
    newAction=new QAction(tr("&New game"),this);
    newAction->setShortcut(QKeySequence::New);
    connect(newAction,SIGNAL(triggered()),igowidget,SLOT(newgame()));

    exit=new QAction(tr("E&xit"),this);
    exit->setShortcut(tr("Ctrl+Q"));
    connect(exit,SIGNAL(triggered()),this,SLOT(close()));

    back=new QAction(tr("&Back"),this);
    back->setShortcut(tr("Ctrl+Z"));
    connect(back,SIGNAL(triggered()),igowidget,SLOT(back()));

    front=new QAction(tr("&Front"),this);
    front->setShortcut(tr("Ctrl+f"));
    connect(front,SIGNAL(triggered()),igowidget,SLOT(frontstep()));

    history=new QAction(tr("&History"),this);
    history->setShortcut(tr("Ctrl+H"));
    connect(history,SIGNAL(triggered()),this,SLOT(showHistory()));

    saveAction=new QAction(tr("&Save"),this);
    saveAction->setShortcut(tr("Ctrl+S"));
    connect(saveAction,SIGNAL(triggered()),this,SLOT(save()));

    loadAction=new QAction(tr("&Load"),this);
    loadAction->setShortcut(tr("Ctrl+L"));
    connect(loadAction,SIGNAL(triggered()),this,SLOT(load()));

    aboutAction=new QAction(tr("&About Qt"),this);
    aboutAction->setShortcut(tr("F1"));
    connect(aboutAction,SIGNAL(triggered()),qApp,SLOT(aboutQt()));

    asServerAction=new QAction(tr("&Server"),this);
    connect(asServerAction,SIGNAL(triggered(bool)),this,SLOT(asServer()));

    asClientAction=new QAction(tr("&Client"),this);
    connect(asClientAction,SIGNAL(triggered(bool)),this,SLOT(asClient()));

    onLineAction=new QAction(tr("&On line"),this);
    connect(onLineAction,SIGNAL(triggered(bool)),this,SLOT(onLine()));

    offLineAction=new QAction(tr("O&ff line"),this);
    connect(offLineAction,SIGNAL(triggered(bool)),this,SLOT(offLine()));

    capitulateAction=new QAction(tr("&Capitulate"),this);
    connect(capitulateAction,SIGNAL(triggered(bool)),this,SLOT(capitulate()));
}

void MainWindow::createMenus()
{
    fileMenu=menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(loadAction);
    fileMenu->addAction(exit);

    editMenu=menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(back);
    editMenu->addAction(front);
    editMenu->addAction(history);
    editMenu->addAction(asServerAction);
    editMenu->addAction(asClientAction);
//    editMenu->addAction(onLineAction);
    editMenu->addAction(offLineAction);
    editMenu->addAction(capitulateAction);

    helpMenu=menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
}

void MainWindow::createStatusBar()
{
    message=new QLabel(this);
    message->setAlignment(Qt::AlignHCenter);
    message->setMinimumSize(message->sizeHint());
    message->setText("hello");
    connect(igowidget,SIGNAL(messagechange(QString)),this,SLOT(setmessage(QString)));
    statusBar()->addWidget(message);
}
