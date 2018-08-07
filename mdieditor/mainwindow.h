#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<QMainWindow>
class QAction;
class QActionGroup;
class QLabel;
class QMdiArea;
class QMenu;
class QToolBar;
class Editor;
class MainWindow:public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();
public slots:
    void newFile();
    void openFile(const QString &fileName);
protected:
    void closeEvent(QCloseEvent*event);
private slots:
    void open();
    void save();
    void saveAs();
    void printf();
    void undo();
    void cut();
    void copy();
    void paste();
    void del();
    void find();
    void findNext();
    void selectAll();
    void date();
    void replace();
    void jump();
    void autoLinefeed();
    void setFont();
    void about();
    void updateActions();
    void loadFiles();
private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void addEditor(Editor*editor);
    Editor*activeEditor();
    QMdiArea *mdiArea;
    QLabel*readyLabel;
    QWidgetList windows;
    QMenu*fileMenu;
    QMenu*editMenu;
    QMenu*formatMenu;
    QMenu*windowMenu;
    QMenu*helpMenu;
    QToolBar*fileToolBar;
    QToolBar*editToolBar;
    QActionGroup*windowActionGroup;
    QAction*newAction;
    QAction*openAction;
    QAction*saveAction;
    QAction*saveAsAction;

    QAction*printAction;

    QAction*undoAction;

    QAction*exitAction;
    QAction*cutAction;
    QAction*copyAction;
    QAction*pasteAction;

    QAction*delAction;

    QAction*findAction;
    QAction*findNextAction;
    QAction*selectAllAction;
    QAction*dateAction;
    QAction*replaceAction;
    QAction*jumpAction;
    QAction*autoLinefeedAction;
    QAction*setFontAction;

    QAction*closeAction;
    QAction*closeAllAction;
    QAction*tileAction;
    QAction*cascadeAction;
    QAction*nextAction;
    QAction*previousAction;
    QAction*separatorAction;
    QAction*aboutAction;
    QAction*aboutQtAction;
};
#endif
