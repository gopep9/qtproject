#ifndef CALCULATEWIDGET_H
#define CALCULATEWIDGET_H

#include <QWidget>
class QPushButton;
class QPlainTextEdit;
class CalculateWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CalculateWidget(QWidget *parent = 0);

signals:

public slots:
    void equality();
//    void addChar(QChar);
    void num0Click();
    void num1Click();
    void num2Click();
    void num3Click();
    void num4Click();
    void num5Click();
    void num6Click();
    void num7Click();
    void num8Click();
    void num9Click();
    void pointClick();
    void additionClick();
    void subtractionClick();
    void multiplicationClick();
    void divisionClick();
    void modClick();
    void reciprocalClick();
    void backClick();
    void ceClick();
    void cClick();
    void oppositeClick();
    void sqrtClick();
    void leftParenthesesClick();
    void rightParenthesesClick();

private:
    double plusOrSubtract(QString&);
    double multiplyOrDivide(QString&);
    double parentheses(QString&);
    double getANumber(QString&);

    void initial();
    QPushButton *numButton[10];
    QPushButton *pointButton=0;
    QPushButton *additionButton;
    QPushButton *subtractionButton;
    QPushButton *multiplicationButton;
    QPushButton *divisionButton;
    QPushButton *modButton;
    QPushButton *reciprocalButton;
    QPushButton *equalityButton;
    QPushButton *backButton;
    QPushButton *ceButton;
    QPushButton *cButton;
    QPushButton *oppositeButton;
    QPushButton *sqrtButton;
    QPushButton *leftParentheses;
    QPushButton *rightParentheses;
    QPlainTextEdit *textEdit;
};

#endif // CALCULATEWIDGET_H
