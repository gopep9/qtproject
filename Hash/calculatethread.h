#ifndef CALCULATETHREAD_H
#define CALCULATETHREAD_H
#include <QThread>
#include <QFile>
class CalculateThread:public QThread
{
    Q_OBJECT
private:
    explicit CalculateThread(QObject *parent=0);
public:
    //单例
    static CalculateThread *sharedInstance();
    void run();
    void setFileNameList(QStringList fileNameList);
//    判断是否已经运行完成
    bool stopRun();
    void setFileProgressBar(int length);
    void setTotalProgressBar(int length);
    bool needStop=false;
private:
    //    计算每个md5
    void calculateMD5(QString fileName);
    void calculateSHA1(QString fileName);
    void calculateCRC32(QString fileName);
    QString permissionToQString(QFile::Permissions);

    QStringList fileNameList;//需要计算的文件列表

signals:
    void sendMessage(QString msg);
    void sendFileProgressBar(int length);
    void sendTotalProgressBar(int length);

public slots:
    void stopCalculate();
};

#endif // CALCULATETHREAD_H
