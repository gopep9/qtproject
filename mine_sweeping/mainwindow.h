#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class mineSweeping;
class QLCDNumber;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QTime;
class QTimer;
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    
signals:
    
public slots:
    void newgame();
    void easy();
    void normal();
    void hard();
    void custom();
    void timeout();
    void win();
    void lose();
    void click();
protected:
//    void timerEvent(QTimerEvent *);
private:
    void createActions();
    void createMenus();

    QTime *time;
    QTimer *timer;
    mineSweeping *mineSweepingWidget;
    QWidget *mainWidget;
    QLCDNumber *leaveMineCountLcd;
    QLCDNumber *timeLcd;
    QPushButton *restart;
    QHBoxLayout *hLayout;
    QVBoxLayout *vLayout;

    QAction *newAction;
    QAction *easyAction;
    QAction *normalAction;
    QAction *hardAction;
    QAction *customAction;
    QAction *closeAction;
    QAction *aboutQtAction;

    QMenu *fileMenu;
    QMenu *helpMenu;

    int mine;
    int row;
    int column;
};

#endif // MAINWINDOW_H
