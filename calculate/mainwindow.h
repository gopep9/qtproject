#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class CalculateWidget;
class QAction;
class QMenu;
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

signals:

public slots:

private:
    void createActions();
    void createMenus();
//    void createContextMenu();
    QAction *about;
    QMenu *helpMenu;
    CalculateWidget *calculateWidget;

};

#endif // MAINWINDOW_H
