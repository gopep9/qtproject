#ifndef BLOCK_H
#define BLOCK_H

#include <QLabel>

class block : public QLabel
{
    Q_OBJECT
public:
    explicit block(QWidget *parent = 0);

signals:
    void boom();
    void bePress(int c,int r);
    void click();
public slots:
    void setCoordinate(int c, int r);
    void setMine();
    bool haveMine();
    void setNumber(int i);
    void reset();
    bool haveClean();
    void setClean();
    bool openOrClose();
    int Sign()
    {
        return sign;
    }

protected:
    void mousePressEvent(QMouseEvent *event);
private:
    bool mine;
    int sign;
    int column;
    int row;
    bool clean;
};

#endif // BLOCK_H
