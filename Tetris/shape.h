#ifndef SHAPE_H
#define SHAPE_H

#include"position.h"
class Shape
{
public:
    enum ShapeType{OShape=1,LShape=2,
                   SShape=3,ZShape=4,JShape=5,IShape=6,
                  TShape=7};

    Shape(int i);
    PositionVector currentPos();
    void rotate();
    ShapeType currentType(){
        return type;
    }

    void down()
    {
        currenty++;
    }
    void up()
    {
        currenty--;
    }
    void left()
    {
        currentx--;
    }
    void right()
    {
        currentx++;
    }

private:
    ShapeType type;
    int currentx;
    int currenty;
    bool shapeArea[4][4];
};

//class
#endif // SHAPE_H
