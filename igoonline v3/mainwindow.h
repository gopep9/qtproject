#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QTcpSocket>
#include <QTcpServer>
//typedef int qintptr;
class Igo;
class QPushButton;
class QLabel;
class MyServer;
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
protected:
    void paintEvent(QPaintEvent *);
    void closeEvent(QCloseEvent *event);
//    void keyPressEvent(QKeyEvent *);
signals:

public slots:
    void showHistory();
    bool save();
    bool load();
    void setmessage(QString);
    void asServer();
    void asClient();
    void acceptConnection(qintptr);
    void send(int x,int y);
    void readClient();
    void onLine();
    void offLine();
    void disconnectDialog();
    void capitulate();
//    connect(server, SIGNAL(newConnection()), this, SLOT(acceptConnection()));

//    void about();
private:
    static void sleep(unsigned int msec);
    void createActions();
    void createMenus();
    void createStatusBar();
    QLabel *message;
    bool isItOnLine=false;
    Igo *igowidget;
    QMenu*fileMenu;
    QMenu*editMenu;
    QMenu*helpMenu;
    QAction*newAction;
    QAction*saveAction;
    QAction*loadAction;
    QAction*exit;
    QAction*back;
    QAction*front;
    QAction*history;
    QAction*aboutAction;
    QAction *asServerAction;
    QAction *asClientAction;
    QAction *onLineAction;
    QAction *offLineAction;
    QAction *capitulateAction;
    MyServer*server=0;
//    QTcpSocket*receivedSocket;
    QTcpSocket*tcpClient=0;
    QString ipAddress="192.168.225.1";
    int port=6667;
//    bool isAsClient=false;
};

#endif
