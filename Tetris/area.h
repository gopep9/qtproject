#ifndef AREA_H
#define AREA_H

#include <QWidget>
#include "block.h"
#include "position.h"
#include "shape.h"
class QGridLayout;
class QTimer;
class Area : public QWidget
{
    Q_OBJECT
    //enum{width=10,height=20};
    const static int width=10,height=20;
public:
    explicit Area(QWidget *parent = 0);
    ~Area();
    bool crash();
signals:

public slots:
    void nextStep();
protected slots:
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);
private:
    void addToData();
    void updateImage();
    void delBlock();

    //QVector<QVector<Block> > *vec;
    Block vec[height][width];
    QGridLayout *layout;
    QTimer *timer;
    Shape shape;
    int areaData[height][width]{};
};

#endif // AREA_H
