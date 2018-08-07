#include "minesweeping.h"
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QtGlobal>
#include<QTime>
mineSweeping::mineSweeping(QWidget*parent)
    :QWidget(parent),area(new block[maxlabel][maxlabel]),
      column(9),row(9),mineCount(10)
{
//    area=new Area;
//    area=new block [maxlabel][maxlabel];
//    column=10;
//    row=10;
//    mineCount=10;
//    newgame();
}

void mineSweeping::newgame()
{
    QVBoxLayout *vLayout=new QVBoxLayout;
    for(int i=0;i<row;i++)
    {
        QHBoxLayout *hLayout=new QHBoxLayout;
        for(int j=0;j<column;j++)
        {
            area[i][j].reset();
            area[i][j].setCoordinate(j,i);
            hLayout->addWidget(&area[i][j]);
            connect(area[i]+j,SIGNAL(bePress(int,int)),
                    this,SLOT(pressBlock(int,int)));
            connect(area[i]+j,SIGNAL(click()),this,SIGNAL(click()));
        }
        vLayout->addLayout(hLayout);
    }
    setLayout(vLayout);
    setMine();
//    emit area[1][2].bePress(1,2);
//    pressBlock(1,2);
}
void mineSweeping::setColumn(int c)
{
    column=c;
}

void mineSweeping::setRow(int r)
{
    row=r;
}

void mineSweeping::setMineCount(int m)
{
    mineCount=m;
}

void mineSweeping::setMine()
{
    if((row+1)*(column+1)<=mineCount)
    {
    }
    else
    {
        qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
        for(int i=0;i<mineCount;)
        {
            int x=qrand()%column;
            int y=qrand()%row;
            if(!area[y][x].haveMine())
            {
                i++;
                area[y][x].setMine();
            }
        }
    }
}

int mineSweeping::signCount()
{
    int count=0;
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<column;++j)
        {
            if(area[i][j].Sign()==10)
            {
                count++;
            }
        }
    }
    return count;
}

void mineSweeping::pressBlock(int c, int r)
{
    if(area[r][c].haveMine())
        boom();
    else
    {
        diedaifa(c,r);
    }
    int count=0;
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<column;j++)
        {
            if(area[i][j].openOrClose())
                count++;
        }
    }
    if(count==column*row-mineCount)
        emit win();
//    emit click();
}

//void mineSweeping::mousePressEvent(QMouseEvent *)
//{
//    newgame();
//}

void mineSweeping::diedaifa(int c, int r)
{
    if(!area[r][c].haveClean())
    {
        area[r][c].setClean();
        int m=0;
        for(int i=r-1;i<=r+1;i++)
        {
            for(int j=c-1;j<=c+1;j++)
            {
                if(i>=0&&j>=0&&i<row&&j<column)
                    if(area[i][j].haveMine())
                        m++;
            }
        }
        if(m>0)
        {
            area[r][c].setNumber(m);
        }
        else
        {
            area[r][c].setNumber(0);
            for(int i=r-1;i<=r+1;i++)
            {
                for(int j=c-1;j<=c+1;j++)
                {
                    if(i>=0&&j>=0&&i<row&&j<column)
                        diedaifa(j,i);
                }
            }
        }
    }
}

void mineSweeping::boom()
{
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<column;++j)
        {
            if(area[i][j].haveMine())
                area[i][j].setNumber(9);
        }
    }
    emit lose();
}
