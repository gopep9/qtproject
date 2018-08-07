#include "igo.h"
#include<QPainter>
#include<QPaintEvent>
#include<QPalette>
#include<cmath>
#include<algorithm>
#include<QPushButton>
#include<QTime>
#include<QLCDNumber>
#include<QTimer>
#include<QListWidget>
#include<QLabel>
#include<QApplication>
#include<QBoxLayout>
Igo::Igo(QWidget *parent) :
    QWidget(parent),step(0),/*currentcursor(0,0),*/front(0),
    totalheap(0)
{
    setMinimumSize(windowWidth,windowHeight);
    setMaximumSize(windowWidth,windowHeight);
//    setNoAccept();
    this->setFocusPolicy(Qt::StrongFocus);
    this->setWindowTitle("myigo");

    time=new QTime(0,0,0,0);

    timer=new QTimer(this);
    timer->setInterval(1000);
    timer->start();
    connect(timer,SIGNAL(timeout()),this,SLOT(onTimerOut()));

    onehandtime=new QTime(0,0);
    onehandtimer=new QTimer(this);
    onehandtimer->setInterval(1000);
    onehandtimer->start();
    connect(onehandtimer,SIGNAL(timeout()),this,SLOT(oneHandOnTimerOut()));

    lcdtime=new QLCDNumber(this);
    lcdtime->setDigitCount(10);
    lcdtime->setMode(QLCDNumber::Dec);
    lcdtime->setSegmentStyle(QLCDNumber::Flat);

    oneHandLCD=new QLCDNumber(this);
    oneHandLCD->setDigitCount(10);
    oneHandLCD->setMode(QLCDNumber::Dec);
    oneHandLCD->setSegmentStyle(QLCDNumber::Flat);

    killblackcount=new QLCDNumber(this);
    killblackcount->setDigitCount(10);
    killblackcount->setMode(QLCDNumber::Dec);
    killblackcount->setSegmentStyle(QLCDNumber::Flat);

    killwhitecount=new QLCDNumber(this);
    killwhitecount->setDigitCount(10);
    killwhitecount->setMode(QLCDNumber::Dec);
    killwhitecount->setSegmentStyle(QLCDNumber::Flat);

    QLabel *totallabel=new QLabel(this);
    totallabel->setText("Total time:");

    QLabel *thishandlabel=new QLabel(this);
    thishandlabel->setText("This hand time:");

    QLabel *killblackcountlabel=new QLabel(this);
    killblackcountlabel->setText("Kill black count:");

    QLabel *killwhitecountlabel=new QLabel(this);
    killwhitecountlabel->setText("Kill white count:");

    QLabel *currentpiecelabel=new QLabel(this);
    currentpiecelabel->setText("Next piece:");


    listWidget=new QListWidget();
    listWidget->setGeometry(windowWidth-150,y0+200,300,200);

    currentpiececolorwidget=new QWidget();
    currentpiececolorwidget->setMaximumSize(150,150);
    currentpiececolorwidget->setMinimumSize(150,150);


    QGridLayout *gridlayout=new QGridLayout();
    gridlayout->addWidget(totallabel,0,0,1,1);
    gridlayout->addWidget(lcdtime,1,0,1,1);
    gridlayout->addWidget(thishandlabel,2,0,1,1);
    gridlayout->addWidget(oneHandLCD,3,0,1,1);
    gridlayout->addWidget(killblackcountlabel,4,0,1,1);
    gridlayout->addWidget(killblackcount,5,0,1,1);
    gridlayout->addWidget(killwhitecountlabel,6,0,1,1);
    gridlayout->addWidget(killwhitecount,7,0,1,1);
    gridlayout->addWidget(currentpiecelabel,8,0,1,1);
    gridlayout->addWidget(currentpiececolorwidget,9,0,1,1);
    right=new QWidget(this);

    right->setGeometry(width+2*x0,0,(windowWidth-width-2*x0),windowHeight);

    right->setLayout(gridlayout);
    newgame();
}

bool Igo::addpiece(int x, int y)
{
    if(check(x,y)&&acceptAbel)
    {
        if(currentpiececolor())        {
            piece[x][y].type=Piece::black;
        }
        else        {
            piece[x][y].type=Piece::white;
        }
        step.push_back(hand(x,y));
        if(lifeheapnum>=0)
        {
            heap[lifeheapnum].live=true;
            lifeheapnum=-1;
        }
        change();
        take();
        emit addPieceSignal(x,y);
        return true;
    }
    return false;
}

bool Igo::addPieceFromNet(int x, int y)
{
    if(check(x,y))
    {
        if(currentpiececolor())        {
            piece[x][y].type=Piece::black;
        }
        else        {
            piece[x][y].type=Piece::white;
        }
        step.push_back(hand(x,y));
        if(lifeheapnum>=0)
        {
            heap[lifeheapnum].live=true;
            lifeheapnum=-1;
        }
        change();
        take();
        return true;
    }
    return false;
}
void Igo::paintEvent(QPaintEvent */*event*/)
{
    QPainter painter(this);

    painter.setWindow(0,0,windowWidth,windowHeight);
    QPalette palette;
    palette.setColor(QPalette::Background,QColor(192,121,51,255));
    this->setPalette(palette);

    paintline(painter);
    painthosi(painter);
    paintnumalpha(painter);

    paintpiece(painter);
    paintcursor(painter);
    paintlist(painter);
    paintcurrentpiece(painter);

    paintmessage();
}

void Igo::paintline(QPainter &painter)
{
    painter.drawRect(x0,y0,width,height);
    for(int i=0;i<19-2;i++)
    {
        painter.drawLine(x0,y0+(i+1)*pitch,x0+width,y0+(i+1)*pitch);
        painter.drawLine(x0+(i+1)*pitch,y0,x0+(i+1)*pitch,y0+height);
    }
}

void Igo::painthosi(QPainter &painter)
{
    painter.setBrush(QBrush(Qt::black));
        for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            painter.drawEllipse(x0+width/2-hosir+(i-1)*6*pitch,y0+width/2-hosir+(j-1)*6*pitch,2*hosir,2*hosir);
        }
    }
    painter.setBrush(QBrush());
}
void Igo::paintnumalpha(QPainter&painter)
{
    for(int i=0;i<19;i++)
    {
        painter.drawText(x0+i*pitch,y0-20,QString()+char(('A'+i>='I')?('A'+i+1):('A'+i)));
        painter.drawText(x0+i*pitch,y0+height+30,QString()+char(('A'+i>='I')?('A'+i+1):('A'+i)));
        painter.drawText(x0-30,y0+i*pitch+5,QString::number(19-i,10));
        painter.drawText(x0+width+25,y0+i*pitch+5,QString::number(19-i,10));
    }
}

void Igo::paintpiece(QPainter&painter)
{
    for(int i=0;i<19;i++)
    {
        for(int j=0;j<19;j++)
        {
            if(piece[i][j].type==Piece::black)
            {
                painter.setBrush(QBrush(Qt::black));
                painter.drawEllipse(coordinateToWindow(i,j),11,11);
            }
            else if(piece[i][j].type==Piece::white)
            {
                painter.setBrush(QBrush(Qt::white));
                painter.drawEllipse(coordinateToWindow(i,j),11,11);
            }
        }
    }
}

void Igo::paintcursor(QPainter &painter)
{
    painter.setBrush(QBrush(Qt::red));
    painter.drawRect(coordinateToWindow(currentcursor.x,currentcursor.y).x()-5,
                     coordinateToWindow(currentcursor.x,currentcursor.y).y()-5,
            10,10);
    painter.setBrush(QBrush());
}

void Igo::paintlist(QPainter &)
{
    listWidget->clear();
    std::vector<struct hand>::reverse_iterator it=step.rbegin();

    while(it!=step.rend())
    {
        char alpha[20]="ABCDEFGHJKLMNOPQRST";
        if((it-step.rend()+1)%2)
            listWidget->addItem(QString("white:(")+alpha[(*it).x]+","+QString::number(19-(*it).y)+")");
        else
            listWidget->addItem(QString("black:(")+alpha[(*it).x]+","+QString::number(19-(*it).y)+")");
        it++;
    }
}

void Igo::paintcurrentpiece(QPainter &painter)
{
    if(currentpiececolor())
        painter.setBrush(QBrush(Qt::black));
    else
        painter.setBrush(QBrush(Qt::white));
    painter.drawEllipse(QPoint(windowWidth-100,windowHeight-100),50,50);
    painter.setBrush(QBrush());
    int currentblackcount=0;
    int currentwhitecount=0;
    for(int i=0;i<19;i++)
    {
        for(int j=0;j<19;j++)
        {
            if(piece[i][j].type==Piece::black)
                currentblackcount++;
            if(piece[i][j].type==Piece::white)
                currentwhitecount++;
        }
    }
    killblackcount->display(int(step.end()-step.begin()+1)/2-currentblackcount);
    killwhitecount->display(int(step.end()-step.begin())/2-currentwhitecount);
}

void Igo::paintmessage()
{
    if(!step.empty())
        emit messagechange(QString("Last hand ")+listWidget->item(0)->text()+

                       " now is "+QString::number(step.end()-step.begin())+" hand.");
//    else
//        emit messagechange("hello.");
}

void Igo::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons()&Qt::LeftButton)
    {
        QPoint pos=event->pos();
        for(int i=0;i<19;i++)
        {
            for(int j=0;j<19;j++)
            {
                QPoint currentpos=coordinateToWindow(i,j);
                if(absolute(pos.x(),currentpos.x())<10&&absolute(pos.y(),currentpos.y())<10)
                {
                    if(i!=currentcursor.x||j!=currentcursor.y)
                    {
                        currentcursor.x=i,currentcursor.y=j;
                    }
                    else{
                        addpiece(i,j);
                        front.clear();
                    }
                }
            }
        }
    }
    update();
}

void Igo::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Up:
        if(currentcursor.y>0)
        currentcursor.y--;
        break;
    case Qt::Key_Down:
        if(currentcursor.y<18)
            currentcursor.y++;
        break;
    case Qt::Key_Left:
        if(currentcursor.x>0)
            currentcursor.x--;
        break;
    case Qt::Key_Right:
        if(currentcursor.x<18)
            currentcursor.x++;
        break;
    case Qt::Key_Enter:
    case Qt::Key_Space:
        addpiece(currentcursor.x,currentcursor.y);
        break;
    }
    update();

}

void Igo::onTimerOut()
{
    *time=time->addSecs(1);
    lcdtime->display(time->toString("hh:mm:ss"));
    update();
}

void Igo::oneHandOnTimerOut()
{
    *onehandtime=onehandtime->addSecs(1);
    oneHandLCD->display(onehandtime->toString("hh:mm:ss"));
    update();
}

void Igo::back()
{
    if(!step.empty()){
    front.push_back(step.back());
    step.pop_back();
    hands temp=step;
    hands tempfront=front;
    hand tempcurrentcursor=currentcursor;
    QTime temptime=*time;
    QTimer*temptimer=timer;
    timer=new QTimer;
    newgame();
    front=tempfront;
    hands::iterator it=temp.begin();
    currentcursor=tempcurrentcursor;
    while(it!=temp.end())
    {
        addpiece((*it).x,(*it).y);
        it++;
    }
    *time=temptime;
    delete timer;
    timer=temptimer;
    lcdtime->display(time->toString("hh:mm:ss"));
    update();
    }
}

void Igo::frontstep()
{
    if(!front.empty()){
        addpiece(front.back().x,front.back().y);
    front.pop_back();
    update();
    }
}

void Igo::newgame()
{
    step.clear();
    front.clear();
    currentcursor=Igo::hand(0,0);
    time->setHMS(0,0,0);
    timer->start();
    onehandtime->setHMS(0,0,0);
    onehandtimer->start();
    lcdtime->display(time->toString("hh:mm:ss"));
    oneHandLCD->display(onehandtime->toString("hh:mm:ss"));
    for(int i=0;i<19;i++)
        for(int j=0;j<19;j++)
        {
            piece[i][j].type=Piece::nothing;
            piece[i][j].heapnum=-1;
            piece[i][j].clear=false;
        }
    update();
}

bool Igo::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly)){
        return false;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_4_8);
    std::vector<Igo::hand>::const_iterator it=step.begin();
    while(it!=step.end())
    {
        out<<quint16((*it).x)<<quint16((*it).y);
        it++;
    }
    return true;
}

bool Igo::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly)){
        return false;
    }
    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_4_8);
    quint16 row;
    quint16 column;
    QApplication::setOverrideCursor(Qt::WaitCursor);
    hands temp;
    while(!in.atEnd()){
        in>>row>>column;
        temp.push_back(Igo::hand(row,column));
    }
    newgame();
    hands::iterator it=temp.begin();
    while(it!=temp.end())
    {
        addpiece((*it).x,(*it).y);
        it++;
    }
    QApplication::restoreOverrideCursor();
    update();
    return true;
}

void Igo::change()
{
    onehandtimer->setInterval(1000);
    onehandtimer->start();
    *onehandtime=QTime(0,0);
    update();
    oneHandLCD->display(onehandtime->toString("hh:mm:ss"));
}

bool Igo::huisufa(int x,int y,int heapn)
{
    if(piece[x][y].clear)
        return false;
    if(piece[x][y].type!=Piece::nothing){
        piece[x][y].heapnum=heapn;
        piece[x][y].clear=true;
        heap[heapn].p.push_back(hand(x,y));
        if(x>0)
        {
            if(piece[x-1][y].type==Piece::nothing)
                heap[heapn].live=true;
            if(piece[x][y].type==piece[x-1][y].type)
            huisufa(x-1,y,heapn);
        }
        if(y>0)
        {
            if(piece[x][y-1].type==Piece::nothing)
                heap[heapn].live=true;
            if(piece[x][y].type==piece[x][y-1].type)
            huisufa(x,y-1,heapn);
        }
        if(x<18)
        {
            if(piece[x+1][y].type==Piece::nothing)
                heap[heapn].live=true;
            if(piece[x][y].type==piece[x+1][y].type)
            huisufa(x+1,y,heapn);
        }
        if(y<18)
        {
            if(piece[x][y+1].type==Piece::nothing)
                heap[heapn].live=true;
            if(piece[x][y].type==piece[x][y+1].type)
            huisufa(x,y+1,heapn);
        }
        return true;
    }
    return false;
}

void Igo::calculateheap()
{

    for(int i=0;i<361;i++)
    {
        heap[i].live=false;
        heap[i].p.clear();
    }

    for(int i=0;i<19;i++)
        for(int j=0;j<19;j++)
        {
            piece[i][j].clear=false;
        }

    totalheap=0;
    for(int i=0;i<19;i++)
    {
        for(int j=0;j<19;j++)
        {
            if(huisufa(i,j,totalheap))
            {
                ++totalheap;
            }
        }
    }

}

void Igo::take()
{
    for(int i=0;i<totalheap;i++)
    {
        if(heap[i].live==false)
        {
            hands::iterator it=heap[i].p.begin();
            while(it!=heap[i].p.end())
            {
                piece[(*it).x][(*it).y].heapnum=-1;
                piece[(*it).x][(*it).y].type=Piece::nothing;
                it++;
            }
        }
    }
    update();
}

bool Igo::check(int x, int y)
{
    lifeheapnum=-1;
    if(piece[x][y].type==Piece::nothing)
    {
        if(currentpiececolor())
            piece[x][y].type=Piece::black;
        else
            piece[x][y].type=Piece::white;
        calculateheap();
        if(heap[piece[x][y].heapnum].live==false)
        {
            bool b=false;
            for(int i=0;i<totalheap;i++)
            {
                if(i!=piece[x][y].heapnum&&heap[i].live==false)
                {
                    b=true;
                    break;
                }
            }
            if(b){
                lifeheapnum=piece[x][y].heapnum;
                piece[x][y].type=Piece::nothing;
                return true;
            }
            else
            {
                piece[x][y].type=Piece::nothing;
                return false;
            }
        }
        else{
            piece[x][y].type=Piece::nothing;
            return true;
        }
    }
    return false;
}

int Igo::currentpiececolor(){
    if(!step.empty())
        return (step.end()-step.begin()+1)%2;
    return 1;
}

template<typename T>
T Igo::absolute(T a, T b)
{
    {
        if(a<b)
            return b-a;
        return a-b;
    }
}
