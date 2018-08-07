#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <QObject>
#include <QTcpServer>
class GameServer : public QTcpServer
{
public:
    GameServer();
};

#endif // GAMESERVER_H
