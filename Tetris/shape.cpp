#include "shape.h"


Shape::Shape(int i)
{
    type=ShapeType(i%7+1);
    currentx=5;
    currenty=2;
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
        {
            shapeArea[i][j]=false;
        }
    switch (type) {
    case TShape:
        shapeArea[2][0]=shapeArea[2][1]=shapeArea[2][2]
                =shapeArea[1][1]=true;
        break;
    case OShape:
        shapeArea[1][1]=shapeArea[1][2]=shapeArea[2][1]
                =shapeArea[2][2]=true;
        break;
    case LShape:
        shapeArea[0][1]=shapeArea[1][1]=shapeArea[2][1]
                =shapeArea[2][2]=true;
        break;
    case SShape:
        shapeArea[1][3]=shapeArea[1][2]=shapeArea[2][2]
                =shapeArea[2][1]=true;
        break;
    case ZShape:
        shapeArea[1][0]=shapeArea[1][1]=shapeArea[2][1]
                =shapeArea[2][2]=true;
        break;
    case JShape:
        shapeArea[0][2]=shapeArea[1][2]=shapeArea[2][2]
                =shapeArea[2][1]=true;
        break;
    case IShape:
        shapeArea[0][1]=shapeArea[1][1]=shapeArea[2][1]
                =shapeArea[3][1]=true;
        break;
    default:
        break;
    }
}

PositionVector Shape::currentPos()
{
    PositionVector positionvector(0);
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            if(shapeArea[i][j])
            {
                Position p={
                    j+currentx-2,
                    i+currenty-2};
                positionvector.push_back(p);
            }
        }
    }
    return positionvector;
}

void Shape::rotate()
{
    bool temp[4][4]{};
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            temp[i][j]=shapeArea[i][j];
        }
    }
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            shapeArea[3-j][i]=temp[i][j];
        }
    }
}
