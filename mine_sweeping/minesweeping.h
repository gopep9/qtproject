#ifndef MINESWEEPING_H
#define MINESWEEPING_H

#include<QWidget>
#include"block.h"

class mineSweeping:public QWidget
{
    Q_OBJECT
public:
    mineSweeping(QWidget*parent=0);
public slots:
    void newgame();
    void setColumn(int c);
    void setRow(int r);
    void setMineCount(int m);
    void setMine();
    int signCount();
private slots:
    void pressBlock(int c,int r);
signals:
    void lose();
    void win();
    void click();
protected:
private:
    void diedaifa(int c,int r);
    void boom();
    enum{maxlabel=99};
    block (*area)[maxlabel];
    int column;
    int row;
    int mineCount;
};

#endif // MINE_SWEEPING_H
