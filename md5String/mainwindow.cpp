#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCryptographicHash>
#include <QByteArray>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->lineEdit,SIGNAL(returnPressed()),this,SLOT(on_calculatePushButton_clicked()));
//    connect(ui->lineEdit,SIGNA)
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_calculatePushButton_clicked()
{
    QString text=ui->lineEdit->text();
    QByteArray ba=QCryptographicHash::hash(text.toUtf8(),QCryptographicHash::Md5);
    QString md5str=ba.toHex().constData();
    ui->textBrowser->clear();
    ui->textBrowser->append(QString("源字符串:")+text);
    ui->textBrowser->append(QString("32位 小写:")+md5str);
    ui->textBrowser->append(QString("32位 大写:")+md5str.toUpper());
}
