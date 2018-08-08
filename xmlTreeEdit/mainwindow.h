#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtXml>
#include <QTreeWidget>
#include <QDomDocument>
#include <QFile>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void OnParseButtonClick();
    void on_savePushButton_clicked();

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
private:
    template<typename T>
    void addItemInTree(QDomNode,T*);
    void buildDoc(QTreeWidgetItem *item,QDomNode*node,QDomDocument*doc);
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
