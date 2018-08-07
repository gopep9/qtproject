#include "myserver.h"

MyServer::MyServer(QObject *parent):QTcpServer(parent)
{

}

void MyServer::incomingConnection(qintptr socketDescriptor)
{
    emit acceptConnection(socketDescriptor);
}
