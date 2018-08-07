#include "calculatewidget.h"
#include <QPushButton>
#include <QLayout>
#include <QPlainTextEdit>
#include <QMessageBox>
#include <cmath>
CalculateWidget::CalculateWidget(QWidget *parent) : QWidget(parent)
{
    initial();
}

void CalculateWidget::equality()
{
    QString expression=textEdit->toPlainText();
    double sum=0.0;
    sum=plusOrSubtract(expression);
    QString string=QString::number(sum,'g',10);
    textEdit->setPlainText(string);
    textEdit->setFocus();
    int i=string.size();
    QTextCursor cursor=textEdit->textCursor();
    cursor.setPosition(string.size());
    textEdit->setTextCursor(std::move(cursor));
}

void CalculateWidget::num0Click()
{
    textEdit->insertPlainText("0");
}

void CalculateWidget::num1Click()
{
    textEdit->insertPlainText("1");
}

void CalculateWidget::num2Click()
{
    textEdit->insertPlainText("2");
}

void CalculateWidget::num3Click()
{
    textEdit->insertPlainText("3");
}

void CalculateWidget::num4Click()
{
    textEdit->insertPlainText("4");
}

void CalculateWidget::num5Click()
{
    textEdit->insertPlainText("5");
}

void CalculateWidget::num6Click()
{
    textEdit->insertPlainText("6");
}

void CalculateWidget::num7Click()
{
    textEdit->insertPlainText("7");
}

void CalculateWidget::num8Click()
{
    textEdit->insertPlainText("8");
}

void CalculateWidget::num9Click()
{
    textEdit->insertPlainText("9");
}

void CalculateWidget::pointClick()
{
    textEdit->insertPlainText(".");
}

void CalculateWidget::additionClick()
{
    textEdit->insertPlainText("+");
}

void CalculateWidget::subtractionClick()
{
    textEdit->insertPlainText("-");
}

void CalculateWidget::multiplicationClick()
{
    textEdit->insertPlainText("*");
}

void CalculateWidget::divisionClick()
{
    textEdit->insertPlainText("/");
}

void CalculateWidget::modClick()
{

}

void CalculateWidget::reciprocalClick()
{
    //    textEdit->
    QTextCursor cursor=textEdit->textCursor();
    QTextCursor tmp=cursor;
    cursor.setPosition(0);
    textEdit->setTextCursor(cursor);
    textEdit->insertPlainText("1/(");
    tmp.setPosition(tmp.position());
    textEdit->setTextCursor(tmp);
    textEdit->insertPlainText(")");
}

void CalculateWidget::backClick()
{
    textEdit->textCursor().deletePreviousChar();
}

void CalculateWidget::ceClick()
{
    textEdit->clear();
}

void CalculateWidget::cClick()
{
    textEdit->clear();
}

void CalculateWidget::oppositeClick()
{
    QTextCursor cursor=textEdit->textCursor();
    QTextCursor tmp=cursor;
    cursor.setPosition(0);
    textEdit->setTextCursor(cursor);
    textEdit->insertPlainText("-(");
    tmp.setPosition(tmp.position());
    textEdit->setTextCursor(tmp);
    textEdit->insertPlainText(")");
}

void CalculateWidget::sqrtClick()
{
    textEdit->insertPlainText("sqrt(");
}

void CalculateWidget::leftParenthesesClick()
{
    textEdit->insertPlainText("(");
}

void CalculateWidget::rightParenthesesClick()
{
    textEdit->insertPlainText(")");
}



double CalculateWidget::plusOrSubtract(QString &expression)
{
    double sum=multiplyOrDivide(expression);
    if(!expression.isEmpty())
    {
        QChar ch=expression[0];
        while(ch==QChar('+')||ch==QChar('-'))
        {
            if(ch==QChar('+'))
            {
                expression.remove(0,1);
                sum+=multiplyOrDivide(expression);
            }
            else if(ch==QChar('-'))
            {
                expression.remove(0,1);
                sum-=multiplyOrDivide(expression);
            }
            if(expression.isEmpty())
                break;
            ch=expression[0];
        }
    }
    return sum;
}

double CalculateWidget::multiplyOrDivide(QString &expression)
{
    double product=parentheses(expression);
    if(!expression.isEmpty())
    {
        QChar ch=expression[0];
        while(ch==QChar('*')||ch==QChar('/'))
        {
            if(ch==QChar('*'))
            {
                expression.remove(0,1);
                product*=parentheses(expression);
            }
            else if(ch==QChar('/'))
            {
                expression.remove(0,1);
                double tmp=parentheses(expression);
                if(tmp==0.0)
                {
                    QMessageBox::critical(this,"critical","Cannot divide 0",
                            QMessageBox::Ok,QMessageBox::Ok);
                }
                else
                product/=tmp;
            }
            if(expression.isEmpty())
                break;
            ch=expression[0];
        }
    }
    return product;
}

double CalculateWidget::parentheses(QString &expression)
{
    if(!expression.isEmpty())
    {
        QChar ch=expression[0];
        if(ch.isDigit())
        {
            return getANumber(expression);
        }
        else if(ch==QChar('+')||ch==QChar('-'))
        {
            expression.remove(0,1);
            if(ch==QChar('-'))
                return -parentheses(expression);
            return parentheses(expression);
        }
        else if(ch==QChar('('))
        {
            expression.remove(0,1);
            double sum=plusOrSubtract(expression);
            if(expression.isEmpty()||expression[0]!=QChar(')'))
            {
                QMessageBox::critical(this,"critical","Lose parentheses",
                                      QMessageBox::Ok,QMessageBox::Ok);
                return 0.0;
            }
            expression.remove(0,1);
            return sum;
        }
        else if(ch.isLetter())
        {
            if(expression.indexOf(QString("sqrt"))==0)
            {
                expression.remove(0,4);
                double result=parentheses(expression);
                return sqrt(result);
            }
            else
            {
                QMessageBox::critical(this,"critical","I cannot understand want you say",
                                      QMessageBox::Ok,QMessageBox::Ok);
            }
        }
    }
}

double CalculateWidget::getANumber(QString &expression)
{
    QChar ch;
    double num=0;
    bool isDecimal=false;
    int deep=1;
    while(!expression.isEmpty())
    {
        ch=expression[0];
        if(ch.isDigit()||ch==QChar('.'))
        {
            expression.remove(0,1);
            if(ch.isDigit()&&isDecimal==false)
            {
                num*=10;
                num+=ch.toLatin1()-'0';
            }
            else if(ch.isDigit()&&isDecimal)
            {
                double tmp=ch.toLatin1()-'0';
                for(int i=0;i<deep;i++)
                {
                    tmp/=10;
                }
                num+=tmp;
                deep++;
            }
            else if(ch==QChar('.')&&isDecimal==false)
            {
                isDecimal=true;
            }
            else if(ch==QChar('.')&&isDecimal)
            {
                QMessageBox::critical(this,"critical","Have two point",
                                      QMessageBox::Ok,QMessageBox::Ok);
            }
        }
        else
            break;
    }
    return num;
}

void CalculateWidget::initial()
{
    for(int i=0;i<10;++i)
    {
        numButton[i]=new QPushButton(QString::number(i));
        numButton[i]->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);

    }
    connect(numButton[0],&QPushButton::clicked,this,&CalculateWidget::num0Click);
    connect(numButton[1],&QPushButton::clicked,this,&CalculateWidget::num1Click);
    connect(numButton[2],&QPushButton::clicked,this,&CalculateWidget::num2Click);
    connect(numButton[3],&QPushButton::clicked,this,&CalculateWidget::num3Click);
    connect(numButton[4],&QPushButton::clicked,this,&CalculateWidget::num4Click);
    connect(numButton[5],&QPushButton::clicked,this,&CalculateWidget::num5Click);
    connect(numButton[6],&QPushButton::clicked,this,&CalculateWidget::num6Click);
    connect(numButton[7],&QPushButton::clicked,this,&CalculateWidget::num7Click);
    connect(numButton[8],&QPushButton::clicked,this,&CalculateWidget::num8Click);
    connect(numButton[9],&QPushButton::clicked,this,&CalculateWidget::num9Click);

    pointButton=new QPushButton(".");
    pointButton->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    connect(pointButton,&QPushButton::clicked,this,&CalculateWidget::pointClick);

    additionButton=new QPushButton("+");
    additionButton->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    connect(additionButton,&QPushButton::clicked,this,&CalculateWidget::additionClick);

    subtractionButton=new QPushButton("-");
    subtractionButton->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    connect(subtractionButton,&QPushButton::clicked,this,&CalculateWidget::subtractionClick);

    multiplicationButton=new QPushButton("*");
    multiplicationButton->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    connect(multiplicationButton,&QPushButton::clicked,this,&CalculateWidget::multiplicationClick);

    divisionButton=new QPushButton("/");
    divisionButton->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    connect(divisionButton,&QPushButton::clicked,this,&CalculateWidget::divisionClick);

    modButton=new QPushButton("%");
    modButton->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    connect(modButton,&QPushButton::clicked,this,&CalculateWidget::modClick);

    reciprocalButton=new QPushButton("1/x");
    reciprocalButton->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    connect(reciprocalButton,&QPushButton::clicked,this,&CalculateWidget::reciprocalClick);

    equalityButton=new QPushButton("=");
    equalityButton->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    connect(equalityButton,&QPushButton::clicked,this,&CalculateWidget::equality);

    backButton=new QPushButton("<-");
    backButton->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    connect(backButton,&QPushButton::clicked,this,&CalculateWidget::backClick);

    ceButton=new QPushButton("CE");
    ceButton->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    connect(ceButton,&QPushButton::clicked,this,&CalculateWidget::ceClick);

    cButton=new QPushButton("C");
    cButton->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    connect(cButton,&QPushButton::clicked,this,&CalculateWidget::cClick);

    oppositeButton=new QPushButton("+\n-");
    oppositeButton->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    connect(oppositeButton,&QPushButton::clicked,this,&CalculateWidget::oppositeClick);

    sqrtButton=new QPushButton("sqrt");
    sqrtButton->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    connect(sqrtButton,&QPushButton::clicked,this,&CalculateWidget::sqrtClick);

    leftParentheses=new QPushButton("(");
    leftParentheses->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    connect(leftParentheses,&QPushButton::clicked,this,&CalculateWidget::leftParenthesesClick);

    rightParentheses=new QPushButton(")");
    rightParentheses->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    connect(rightParentheses,&QPushButton::clicked,this,&CalculateWidget::rightParenthesesClick);

    textEdit=new QPlainTextEdit(this);
    textEdit->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed);
    textEdit->setMaximumHeight(60);
    QFont font=textEdit->font();
    font.setPointSize(15);
    textEdit->setFont(std::move(font));
    QGridLayout *mainLayout=new QGridLayout(this);
    mainLayout->addWidget(textEdit,0,0,1,5);

    mainLayout->addWidget(backButton,1,0,1,1);
    mainLayout->addWidget(ceButton,1,1,1,1);
    mainLayout->addWidget(cButton,1,2,1,1);
    mainLayout->addWidget(oppositeButton,1,3,1,1);
    mainLayout->addWidget(sqrtButton,1,4,1,1);

    mainLayout->addWidget(numButton[7],2,0,1,1);
    mainLayout->addWidget(numButton[8],2,1,1,1);
    mainLayout->addWidget(numButton[9],2,2,1,1);
    mainLayout->addWidget(divisionButton,2,3,1,1);
    mainLayout->addWidget(reciprocalButton,2,4,1,1);

    mainLayout->addWidget(numButton[4],3,0,1,1);
    mainLayout->addWidget(numButton[5],3,1,1,1);
    mainLayout->addWidget(numButton[6],3,2,1,1);
    mainLayout->addWidget(multiplicationButton,3,3,1,1);
    mainLayout->addWidget(leftParentheses,3,4,1,1);

    mainLayout->addWidget(numButton[1],4,0,1,1);
    mainLayout->addWidget(numButton[2],4,1,1,1);
    mainLayout->addWidget(numButton[3],4,2,1,1);
    mainLayout->addWidget(subtractionButton,4,3,1,1);
    mainLayout->addWidget(rightParentheses,4,4,1,1);

    mainLayout->addWidget(numButton[0],5,0,1,2);
    mainLayout->addWidget(pointButton,5,2,1,1);
    mainLayout->addWidget(additionButton,5,3,1,1);
    mainLayout->addWidget(equalityButton,5,4,1,1);

    setLayout(mainLayout);

}

