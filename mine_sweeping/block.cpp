#include "block.h"
#include<QMouseEvent>
block::block(QWidget *parent) :
    QLabel(parent)
{
    reset();
}

void block::setCoordinate(int c, int r)
{
    column=c;
    row=r;
}

void block::setMine()
{
    mine=true;
}

bool block::haveMine()
{
    return mine;
}

void block::setNumber(int i)
{
    switch(i)
    {
    case -1:
        this->setPixmap(QPixmap("://images/normal.png"));
        sign=-1;
        break;
    case 0:
        this->setPixmap(QPixmap("://images/mine_0.png"));
        sign=0;
        break;
    case 1:
        this->setPixmap(QPixmap("://images/mine_1.png"));
        sign=1;
        break;
    case 2:
        this->setPixmap(QPixmap("://images/mine_2.png"));
        sign=2;
        break;
    case 3:
        this->setPixmap(QPixmap("://images/mine_3.png"));
        sign=3;
        break;
    case 4:
        this->setPixmap(QPixmap("://images/mine_4.png"));
        sign=4;
        break;
    case 5:
        this->setPixmap(QPixmap("://images/mine_5.png"));
        sign=5;
        break;
    case 6:
        this->setPixmap(QPixmap("://images/mine_6.png"));
        sign=6;
        break;
    case 7:
        this->setPixmap(QPixmap("://images/mine_7.png"));
        sign=7;
        break;
    case 8:
        this->setPixmap(QPixmap("://images/mine_8.png"));
        sign=8;
        break;
    case 9:
        this->setPixmap(QPixmap("://images/mine.png"));
        sign=9;
        break;
    case 10:
        this->setPixmap(QPixmap("://images/flag.png"));
        sign=10;
        break;
    case 11:
        this->setPixmap(QPixmap("://images/mine_QuestionMark.png"));
        sign=11;
        break;
    default:
        this->setText("error");
        break;
    }
}

//void block::setMineSign()
//{
//    sign=9;
//}

//void block::setQuestionMark()
//{
//    sign=2;
//}

//bool block::haveSign()
//{
//    return sign!=0;
//}

void block::reset()
{
    setNumber(-1);
    mine=false;
    clean=false;
}

bool block::haveClean()
{
    return clean;
}

void block::setClean()
{
    clean=true;
}

bool block::openOrClose()
{
    return sign>=0&&sign<9;
}

void block::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        if(sign==-1)
            emit bePress(column,row);
    }
    else
    {
        if(sign==-1)
        {
            setNumber(10);
        }
        else if(sign==10)
        {
            setNumber(11);
        }
        else if(sign==11)
        {
            setNumber(-1);
        }
    }
    emit click();
}
