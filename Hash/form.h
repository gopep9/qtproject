#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QReadWriteLock>
namespace Ui {
class Form;
}

class QFileDialog;

class Form : public QWidget
{
    Q_OBJECT

//public:
private:
    explicit Form(QWidget *parent = 0);
public:
    //单例
    static Form *sharedInstance();
    ~Form();


private slots:
    void on_browseButton_clicked();

    void on_cleanButton_clicked();

    void on_copyButton_clicked();

    void on_saveButton_clicked();

    void on_stopButton_clicked();



protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
private:
    void calculateHash(const QStringList&fileNameList);
    Ui::Form *ui;
    QFileDialog *filedialog;
    int totalLength=10000;

    QReadWriteLock itemLock;//下面3个变量的锁

    bool showVersion;
    bool showDate;
    bool showMD5;
    bool showSHA1;
    bool showCRC32;

signals:
    void stopCalculate();

public slots:
    void printMessage(QString msg);
    void setFileProgressBar(int length);
    void setTotalProgressBar(int length);

    bool isItShowVersion();
    bool isItShowDate();
    bool isItShowMD5();
    bool isItShowSHA1();
    bool isItShowCRC32();

    void enableStopButton();
    void disableStopButton();
};

#endif // FORM_H
