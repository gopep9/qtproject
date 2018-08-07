#include "area.h"
//#include <random>
#include <QLayout>
#include <QTimer>
#include<time.h>
#include<stdlib.h>
#include<QDebug>
#include<QKeyEvent>
#include<QMessageBox>
Area::Area(QWidget *parent) : QWidget(parent),shape(1)
{
    //vec=new QVector<QVector<Block> >(height);
//    std::uniform_int_distribution<int>u(0,6);
    srand((unsigned)time(0));
    shape=Shape(rand());
    layout=new QGridLayout(this);
    layout->setSpacing(1);
    timer=new QTimer(this);
    timer->start();
    timer->setInterval(300);
    connect(timer,&QTimer::timeout,this,&Area::nextStep);
    for(int i=0;i<height;i++)
    {
        //(*vec)[i]=QVector<Block>(width);
        for(int j=0;j<width;j++)
        {

            //(*vec)[i].push_back(QVector<QLabel>);
        }
    }
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            layout->addWidget(&vec[i][j],i,j,1,1);
        }
    }
}

Area::~Area()
{
//    delete layout;
//    delete timer;
}

bool Area::crash()
{
    PositionVector tmp=shape.currentPos();
    for(PositionVector::iterator it=tmp.begin();it!=tmp.end();it++)
    {
        for(int i=0;i<height;i++)
        {
            for(int j=0;j<width;j++)
            {
                if((*it).x==j&&(*it).y==i&&areaData[i][j]!=0)
                {
                    return true;
                }
                if((*it).y>19)
                    return true;
            }
        }
    }

    return false;
}

void Area::nextStep()
{
    shape.down();
    if(crash())
    {
        shape.up();
        addToData();
        delBlock();
        shape=Shape(rand());
        if(crash()){
            timer->stop();
            QMessageBox::information(this,"You lose","You lose",QMessageBox::Ok);
            return;
        }
        //addToData();
    }
    else
    {
        //down();
    }
//    update();
//    repaint();
    updateImage();
}

void Area::paintEvent(QPaintEvent *event)
{
}

void Area::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_Left)
    {
        bool b=true;
        PositionVector tmp=shape.currentPos();
        for(PositionVector::iterator it=tmp.begin();it!=tmp.end();it++)
        {
            if((*it).x==0)
                b=false;
        }
        if(b)
        shape.left();
        if(crash())
            shape.right();
        //return;
    }
    else if(event->key()==Qt::Key_Right)
    {
        bool b=true;
        PositionVector tmp=shape.currentPos();
        for(PositionVector::iterator it=tmp.begin();it!=tmp.end();it++)
        {
            if((*it).x==width-1)
                b=false;
        }
        if(b)
        shape.right();
        if(crash())
            shape.left();
        //return;
    }
    else if(event->key()==Qt::Key_Up)
    {
        shape.rotate();
        if(crash())
        {
            shape.rotate();
            shape.rotate();
            shape.rotate();
        }
        bool b=true;
        PositionVector tmp=shape.currentPos();
        for(PositionVector::iterator it=tmp.begin();it!=tmp.end();it++)
        {
            if((*it).x<0||(*it).x>9)
                b=false;
        }
        if(!b)
        {
            shape.rotate();
            shape.rotate();
            shape.rotate();
        }
        //return;
    }
    else if(event->key()==Qt::Key_Down)
    {
        while(!crash())
        {
            shape.down();
        }
        shape.up();
    }
    updateImage();
    QWidget::keyPressEvent(event);
}

void Area::addToData()
{
    PositionVector tmp=shape.currentPos();
    for(PositionVector::iterator it=tmp.begin();it!=tmp.end();it++)
    {
        int x=(*it).x;
        int y=(*it).y;
//        QDebug()<<static_cast<int>(shape.currentType());
        Shape::ShapeType t=shape.currentType();
        areaData[y][x]=shape.currentType();
    }
}

void Area::updateImage()
{
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            vec[i][j].setcolor(areaData[i][j]);
        }
    }
    PositionVector tmp=shape.currentPos();
    for(PositionVector::iterator it=tmp.begin();it!=tmp.end();it++)
    {
        int a=(*it).x;
        int b=(*it).y;
        Shape::ShapeType ty=shape.currentType();
        vec[(*it).y][(*it).x].setcolor(shape.currentType());
    }
    repaint();
}

void Area::delBlock()
{
    for(int i=0;i<height;i++)
    {
        bool d=true;
        for(int j=0;j<width;j++)
        {
            if(areaData[i][j]==0)
            {
                d=false;
            }
        }
        if(d==true)
        {
            for(int k=i;k>0;k--)
            {
                for(int m=0;m<width;m++)
                {
                    areaData[k][m]=areaData[k-1][m];
                }
            }
        }
    }
}
