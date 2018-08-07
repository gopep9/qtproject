#ifndef IGO_H
#define IGO_H

#include <QWidget>
#include <vector>
class QPushButton;
class QTime;
class QTimer;
class QLCDNumber;
class QListWidget;
class QLabel;
class MainWindow;
class Igo : public QWidget
{
    Q_OBJECT
public:
    explicit Igo(QWidget *parent = 0);
    bool addpiece(int ,int );
    bool addPieceFromNet(int,int);
    void setAcceptAble(){acceptAbel=true;}
    void setNoAccept(){acceptAbel=false;}
protected:
//public:
    void paintEvent(QPaintEvent *);
    void paintline(QPainter&);
    void painthosi(QPainter&);
    void paintnumalpha(QPainter&);
    void paintpiece(QPainter&);
    void paintcursor(QPainter&);
    void paintlist(QPainter&);
    void paintcurrentpiece(QPainter&);
    void paintmessage();
    void mousePressEvent(QMouseEvent *);
    void keyPressEvent(QKeyEvent *);

signals:
    void messagechange(QString);
    void addPieceSignal(int x,int y);
public slots:
    void onTimerOut();
    void oneHandOnTimerOut();
    void back();
    void frontstep();
    void newgame();
    bool saveFile(const QString&fileName);
    bool loadFile(const QString&fileName);
private:
    enum{         windowWidth=800,windowHeight=600,
        height=486,width=height,
        y0=(windowHeight-height)/2,x0=y0,
                hosir=3,pitch=height/18};

    struct hand{
        hand(int x=0,int y=0)
        {
            this->x=x;
            this->y=y;
        }
        bool operator==(const hand &r)
        {
            return x==r.x&&y==r.y;
        }
        int x;
        int y;
    };
    struct Piece
    {
        enum pieceType{white=0,black=1,nothing=2};
        pieceType type;
        int heapnum;
        bool clear;
        Piece(pieceType T=nothing,int h=-1)
        {
            type=T;
            heapnum=h;
            clear=false;
        }
    };
    struct Heap
    {
        std::vector<struct hand> p;
        bool live;
    };
    QPoint coordinateToWindow(int x,int y)
    {
        return QPoint(x0+x*pitch,y0+y*pitch);
    }
    template<typename T>
    T absolute(T a,T b);
    void change();
    bool huisufa(int x,int y,int heapn);
    void calculateheap();
    void take();
    bool check(int x,int y);
    int currentpiececolor();
    typedef std::vector<struct hand> hands;
    std::vector<struct hand>step;
    std::vector<struct hand>front;
    struct Piece piece[19][19];
    struct Heap heap[19*19];
    int totalheap;
    hand currentcursor;
    QTime *time;
    QTimer *timer;
    QTime *onehandtime;
    QTimer *onehandtimer;
    QLCDNumber* lcdtime;
    QLCDNumber *oneHandLCD;
    QLCDNumber *killblackcount;
    QLCDNumber *killwhitecount;
    QWidget* right;
    QWidget* currentpiececolorwidget;
    int lifeheapnum;
    bool acceptAbel=true;
public:
    QListWidget *listWidget;
};

#endif
