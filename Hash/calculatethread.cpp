#include "calculatethread.h"
#include "sha1.h"

#include <fstream>
#include <iostream>
#include <QFile>
#include <QFileInfo>
#include <QDateTime>
#include <QCryptographicHash>
#include "md5.h"
#include "form.h"
#include "crc32.h"
using namespace std;
static CalculateThread *g_calculateThread=NULL;
CalculateThread::CalculateThread(QObject *parent)
{

}

CalculateThread *CalculateThread::sharedInstance()
{
    if(NULL==g_calculateThread)
    {
        g_calculateThread=new CalculateThread();
    }
    return g_calculateThread;
}

void CalculateThread::run()
{
    setTotalProgressBar(0);
    int index=0;
    int totalFile=fileNameList.size();
    needStop=false;
    for(QString fileName:fileNameList)
    {
        QFile file(fileName);
        file.open(QIODevice::ReadOnly);
        if(file.isOpen()==false)
        {
            sendMessage(QString("")+"can't open file "+fileName);
            continue;
        }
        sendMessage(QString("文件:")+fileName);
        QFileInfo fileInfo(file);
        quint64 curSize=file.size();
        QString unit=" bytes";
        sendMessage(QString("大小:")+QString::number(curSize)+unit);
        Form *form=Form::sharedInstance();
        if(form->isItShowDate()){
//            sendMessage(QString("创建时间:")+fileInfo.created().toString());
            sendMessage(QString("修改时间:")+fileInfo.lastModified().toString());
//            sendMessage(QString("最后读取时间:")+fileInfo.lastRead().toString());
        }
        if(form->isItShowVersion())
        {//改为显示各种信息
            QString user=fileInfo.owner();
            QString group=fileInfo.group();
            QFile::Permissions permission=fileInfo.permissions();
            QString strpermission=permissionToQString(permission);
            sendMessage(QString("用户:")+user);
            sendMessage(QString("组:")+group);
            sendMessage(QString("权限:")+strpermission);
        }

        if(form->isItShowMD5())
            calculateMD5(fileName);
        //假如是需要停止的话停止
        if(needStop)
            break;
        if(form->isItShowSHA1())
            calculateSHA1(fileName);
        //假如是需要停止的话停止
        if(needStop)
            break;
        if(form->isItShowCRC32())
            calculateCRC32(fileName);
        //假如是需要停止的话停止
        if(needStop)
            break;
        sendMessage("");
        index++;
        sendTotalProgressBar(100.0*index/totalFile);
    }
//    假如是中途停止的话要添加一行
    if(needStop)
        sendMessage("");
    sendFileProgressBar(100);
    sendTotalProgressBar(100);
}

void CalculateThread::setFileNameList(QStringList fileNameList)
{
    this->fileNameList=fileNameList;
}
//计算每个文件的md5
void CalculateThread::calculateMD5(QString fileName)
{
    ifstream stream1(fileName.toStdString());
    if(!stream1.is_open())
    {
        sendMessage(QString("can't open file"));
        return;
    }
    MD5 md5object(stream1);
    if(needStop)
        return;
    QString md5(md5object.toString().c_str());
    md5=md5.toUpper();
    sendMessage(QString("MD5:")+md5);
}

void CalculateThread::calculateSHA1(QString fileName)
{
    ifstream in(fileName.toStdString());
    if(!in){
        return;
    }
    enum { BUFFER_SIZE = 1024 };
    char buffer[BUFFER_SIZE];
    std::streamsize length;
    streampos pos=in.tellg();
    in.seekg(0,in.end);
    long long total=in.tellg();
    in.seekg(pos);
    long long currentLength=0;
    int lastPercent=0;
    int currentPercent=0;
    setFileProgressBar(0);
    SHA1 sha1;

    while(!in.eof()){

        if(stopRun())
            break;
        in.read(buffer,BUFFER_SIZE);
        length=in.gcount();
        if(length>0){
            sha1.PushData(buffer,length);
        }
        currentLength+=length;
        if(total==0)
            currentPercent=100;
        else
            currentPercent=(int)100.0*currentLength/total;
        if(currentPercent>lastPercent)
        {
            lastPercent=currentPercent;
            setFileProgressBar(currentPercent);
        }
    }
    setFileProgressBar(100);
    in.close();
    char tmp[50]={};
    sha1.GetHexString(tmp);
    sendMessage(QString("SHA1:")+QString(tmp));
}

void CalculateThread::calculateCRC32(QString fileName)
{
    ifstream in(fileName.toStdString());
    if(!in){
        return;
    }
    enum { BUFFER_SIZE = 1024 };
    char buffer[BUFFER_SIZE];
    std::streamsize length;
    streampos pos=in.tellg();
    in.seekg(0,in.end);
    long long total=in.tellg();
    in.seekg(pos);
    long long currentLength=0;
    int lastPercent=0;
    int currentPercent=0;
    setFileProgressBar(0);
    Crc32 crc32;

    while(!in.eof()){

        if(stopRun())
            break;
        in.read(buffer,BUFFER_SIZE);
        length=in.gcount();
        if(length>0){
            crc32.update(buffer,length);
        }
        currentLength+=length;
        if(total==0)
            currentPercent=100;
        else
            currentPercent=(int)100.0*currentLength/total;
        if(currentPercent>lastPercent)
        {
            lastPercent=currentPercent;
            setFileProgressBar(currentPercent);
        }
    }
    setFileProgressBar(100);
    in.close();
    char tmp[50]={};
    unsigned int crc32num = crc32.getresult();
    sprintf(tmp,"%x",crc32num);
    QString crc32str(tmp);
    crc32str=crc32str.toUpper();
    sendMessage(QString("CRC32:")+crc32str);

}

QString CalculateThread::permissionToQString(QFileDevice::Permissions permission)
{
    QString strPermission;
    int totalPermission[3][3]={QFileDevice::ReadOwner,QFileDevice::WriteOwner,QFileDevice::ExeOwner,
//                              QFileDevice::ReadUser,QFileDevice::WriteUser,QFileDevice::ExeUser,
                              QFileDevice::ReadGroup,QFileDevice::WriteGroup,QFileDevice::ExeGroup,
                              QFileDevice::ReadOther,QFileDevice::WriteOther,QFileDevice::ExeOther};
    for(auto &array:totalPermission)
    {
        int i=0;
        for(auto &numPermission:array)
        {
            if(0==i&&(numPermission&permission))
            {
                strPermission+="r";
            }
            else if(1==i&&(numPermission&permission))
            {
                strPermission+="w";
            }
            else if(2==i&&(numPermission&permission))
            {
                strPermission+="x";
            }
            else
            {
                strPermission+="-";
            }
            i++;
        }
    }
    return strPermission;
/*
 * ReadOwner = 0x4000, WriteOwner = 0x2000, ExeOwner = 0x1000,
        ReadUser  = 0x0400, WriteUser  = 0x0200, ExeUser  = 0x0100,
        ReadGroup = 0x0040, WriteGroup = 0x0020, ExeGroup = 0x0010,
        ReadOther = 0x0004, WriteOther = 0x0002, ExeOther = 0x0001
*/
}

bool CalculateThread::stopRun()
{
    return needStop;
}

void CalculateThread::setFileProgressBar(int length)
{
    emit sendFileProgressBar(length);
}

void CalculateThread::setTotalProgressBar(int length)
{
    emit sendTotalProgressBar(length);
}

void CalculateThread::stopCalculate()
{
    needStop=true;
}
