#ifndef BLOCK_H
#define BLOCK_H

#include <QWidget>
#include <QLabel>
class Block : public QLabel
{

public:
    Block(QWidget*parent=0);
    void setcolor(int i);
    ~Block();
private:
    QPixmap *pix;
};

#endif // BLOCK_H
