#include "block.h"
#include <QPixmap>


Block::Block(QWidget *parent):QLabel(parent)
{
//    QPixmap pix(20,20);
    pix=new QPixmap(30,30);
    pix->fill();
//    pix->fill(Qt::red);
    this->setPixmap(*pix);
}

void Block::setcolor(int i)
{
    switch (i) {
    case 0:
        pix->fill(Qt::white);
        break;
    case 1:
        pix->fill(Qt::red);
        break;
    case 2:
        pix->fill(Qt::green);
        break;
    case 3:
        pix->fill(Qt::blue);
        break;
    case 4:
        pix->fill(Qt::cyan);
        break;
    case 5:
        pix->fill(Qt::magenta);
        break;
    case 6:
        pix->fill(Qt::yellow);
        break;
    case 7:
        pix->fill(Qt::gray);
        break;
    default:
        break;
    }
    this->setPixmap(*pix);
}

Block::~Block()
{
    delete pix;
}
