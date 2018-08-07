#ifndef EDITOR_H
#define EDITOR_H
#include<QTextEdit>
#include<QString>
#include<QPlainTextEdit>
class findDialog;
class gotoDialog;
class replaceDialog;
class Editor:public QPlainTextEdit
{
    Q_OBJECT
public:
    Editor(QWidget*parent=0);
    void newFile();
    bool save();
    bool saveAs();
    QSize sizeHint()const;
    QAction*windowMenuAction()const {return action;}
    static Editor*open(QWidget*parent=0);
    static Editor*openFile(const QString &fileName,
                           QWidget*parent=0);
//    ~Editor(){

//    }
    void printf();
//    void undo();
    void find();
    void findNext();
    void date();
    void replace();
    void jump();
    void autoLinefeed();
//    void setFont();

//    void open();
//    void save();
//    void saveAs();
//    void printf();
//    void undo();
//    void cut();
//    void copy();
//    void paste();
//    void del();
//    void find();
//    void findNext();
//    void selectAll();
//    void date();
//    void replace();
//    void autoLinefeed();
//    void setFont();
//    void about();
//    void updateActions();
//    void loadFiles();
public slots:
    bool findNextOne(const QString&,Qt::CaseSensitivity);
    bool findPreviousOne(const QString&,Qt::CaseSensitivity);
    bool replaceNext(const QString&,Qt::CaseSensitivity);
    bool replaceAll(const QString&,Qt::CaseSensitivity);
    bool gotoLine(int line);
protected:
    void closeEvent(QCloseEvent*event);
private slots:
    void documentWasModified();
private:
    bool okToContinue();
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    bool readFile(const QString &fileName);
    bool writeFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);
    void replaceText(const QString&str);
    int lineToposition(int line);
    QString curFile;
    bool isUntitled;
    QAction*action;
    findDialog*finddialog;
    gotoDialog*gotodialog;
    replaceDialog*replacedialog;
    QString findStr;
    Qt::CaseSensitivity findcs;
    bool isItFindNext;
};
#endif
