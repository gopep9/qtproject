#include "form.h"
#include "ui_form.h"
#include <QFileDialog>
#include <QDialog>
#include <QCryptographicHash>
#include <QDateTime>
#include <QTextStream>
#include <QMimeData>
#include <QDebug>
#include <iostream>
#include <thread>
#include "md5.h"
#include "calculatethread.h"


static ifstream g_stream;
static MD5 md5object;

static Form *g_form=NULL;

using std::ifstream;
Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    setWindowTitle("Hash");
    filedialog=new QFileDialog(this);
    setAcceptDrops(true);
    ui->plainTextEdit->setAcceptDrops(false);
    ui->plainTextEdit->setReadOnly(true);
//    关联CalculateThread和Form的信号槽
    CalculateThread *calculateThread=CalculateThread::sharedInstance();
    connect(calculateThread,SIGNAL(sendMessage(QString)),this,SLOT(printMessage(QString)));
    connect(calculateThread,SIGNAL(sendFileProgressBar(int)),this,SLOT(setFileProgressBar(int)));
    connect(calculateThread,SIGNAL(sendTotalProgressBar(int)),this,SLOT(setTotalProgressBar(int)));
    connect(this,SIGNAL(stopCalculate()),calculateThread,SLOT(stopCalculate()));
    connect(calculateThread,SIGNAL(finished()),this,SLOT(disableStopButton()));
    connect(calculateThread,SIGNAL(started()),this,SLOT(enableStopButton()));

}

Form *Form::sharedInstance()
{
    if(NULL==g_form)
    {
        g_form=new Form();
    }
    return g_form;
}

Form::~Form()
{
    delete ui;
}

bool Form::isItShowVersion()
{
    itemLock.lockForRead();
    bool temp=showVersion;
    itemLock.unlock();
    return temp;
}

bool Form::isItShowDate()
{
    itemLock.lockForRead();
    bool temp=showDate;
    itemLock.unlock();
    return temp;
}

bool Form::isItShowMD5()
{
    itemLock.lockForRead();
    bool temp=showMD5;
    itemLock.unlock();
    return temp;
}

bool Form::isItShowSHA1()
{
    itemLock.lockForRead();
    bool temp=showSHA1;
    itemLock.unlock();
    return temp;
}

bool Form::isItShowCRC32()
{
    itemLock.lockForRead();
    bool temp=showCRC32;
    itemLock.unlock();
    return temp;
}

void Form::enableStopButton()
{
    ui->stopButton->setEnabled(true);
}

void Form::disableStopButton()
{
    ui->stopButton->setEnabled(false);
}
//打开文件
void Form::on_browseButton_clicked()
{
    if(filedialog->exec()==QFileDialog::Accepted)
    {
        QStringList fileNameList=filedialog->selectedFiles();
        calculateHash(fileNameList);
    }
}
//清理输出板
void Form::on_cleanButton_clicked()
{
    ui->plainTextEdit->clear();
}
//复制到剪贴板
void Form::on_copyButton_clicked()
{
    ui->plainTextEdit->selectAll();
    ui->plainTextEdit->copy();
}
//导出到文件
void Form::on_saveButton_clicked()
{
    if(filedialog->exec()==filedialog->Accepted)
    {
        QStringList fileNameList=filedialog->selectedFiles();
        QString fileName0=fileNameList[0];
        QFile file(fileName0);
        file.open(QIODevice::ReadWrite|QIODevice::Text);
        QTextStream textStream(&file);
        QString text=ui->plainTextEdit->toPlainText();
        QStringList textList=text.split('\n');
        for(QString i:textList)
            textStream<<i<<QString("\n");
        file.close();
    }
}
//停止线程计算
void Form::on_stopButton_clicked()
{
    stopCalculate();
}

void Form::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
}

void Form::dragMoveEvent(QDragMoveEvent *event)
{
    event->accept();
}

void Form::dropEvent(QDropEvent *event)
{
    QList<QUrl>urls=event->mimeData()->urls();
    QStringList fileNameList;
    for(QUrl i:urls)
    {
        QFile file(i.toLocalFile());
        if(file.open(QIODevice::ReadOnly))
            fileNameList.push_back(i.toLocalFile());
        file.close();
    }
    calculateHash(fileNameList);
    event->accept();
}
//开始计算hash值
void Form::calculateHash(const QStringList &fileNameList)
{
    itemLock.lockForWrite();
    //设置要计算哪些值
    showVersion=ui->versionBox->isChecked();
    showDate=ui->dateBox->isChecked();
    showMD5=ui->md5Box->isChecked();
    showSHA1=ui->sha1Box->isChecked();
    showCRC32=ui->Crc32Box->isChecked();
    itemLock.unlock();

    CalculateThread *calculateThread=CalculateThread::sharedInstance();
//    if(上一次还没有计算完成)
//    {
//        停止上一次计算
//    }
//    设置各种参数
//    开始计算
    calculateThread->setFileNameList(fileNameList);
    calculateThread->start();
}

void Form::printMessage(QString msg)
{
    ui->plainTextEdit->appendPlainText(msg);
}

void Form::setFileProgressBar(int length)
{
    ui->fileProgressBar->setValue(length);
    repaint();
}

void Form::setTotalProgressBar(int length)
{
    ui->totalProgressBar->setValue(length);
    repaint();
}


