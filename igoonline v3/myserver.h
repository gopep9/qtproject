#ifndef MYSERVER_H
#define MYSERVER_H

#include <QTcpServer>
//typedef int qintptr;
class MyServer : public QTcpServer
{
    Q_OBJECT
public:
    MyServer(QObject *parent=0);
    void incomingConnection(qintptr socketDescriptor)override;
signals:
    void acceptConnection(qintptr);
};

#endif // MYSERVER_H
