#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QtXml>
#include <QDragMoveEvent>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->parsePushButton,&QPushButton::clicked,this,&MainWindow::OnParseButtonClick);
    ui->xmlTreeWidget->setAcceptDrops(false);
    this->setAcceptDrops(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnParseButtonClick()
{
    ui->xmlTreeWidget->clear();
    QString path=ui->pathLineEdit->text();
    QDomDocument doc("mydoc");
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
    {
        return;
    }
    if(!doc.setContent(&file))
    {
        file.close();
        return;
    }

    QDomElement root=doc.documentElement();
    QString rootName=root.nodeName();

    addItemInTree(root,ui->xmlTreeWidget);
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
}

void MainWindow::dragMoveEvent(QDragMoveEvent *event)
{
    event->accept();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QList<QUrl>urls=event->mimeData()->urls();
    QStringList fileNameList;
    for(QUrl i:urls)
    {
        QFile file(i.toLocalFile());
        if(file.open(QIODevice::ReadOnly))
            fileNameList.push_back(i.toLocalFile());
        file.close();
    }
    ui->pathLineEdit->setText(fileNameList.at(0));
    OnParseButtonClick();
    event->accept();
}
//使用template，兼容qtreewidgetitem和qtreewidget
template<typename T>
void MainWindow::addItemInTree(QDomNode node, T *item)//item是当前node的parent item
{
    QDomNode::NodeType nodeType=node.nodeType();

    switch (nodeType) {
    case QDomNode::ElementNode://element节点属性和下面的文本
    {
        QString nodeName=node.nodeName();
        QTreeWidgetItem *treeWidgetItem=new QTreeWidgetItem(item,QStringList(nodeName));
        for(int i=0;i<node.childNodes().size();i++)
        {
            addItemInTree(node.childNodes().at(i),treeWidgetItem);
        }
        QDomElement element=node.toElement();
        QDomNamedNodeMap attrMap=element.attributes();
        for(int i=0;i<attrMap.size();i++)
        {
            addItemInTree(attrMap.item(i),treeWidgetItem);
        }

        QDomNode text=element.toText();
        if(text.isText())//有text的时候才显示
            addItemInTree(text,treeWidgetItem);
    }
        break;
    case QDomNode::AttributeNode://attr节点，
    {
        //attr节点，考虑使用右键显示attr
        QString nodeName=node.nodeName();
        QString nodeValue=node.nodeValue();
        QStringList strList;
        strList.append(nodeName);
        strList.append(nodeValue);
        QTreeWidgetItem *treeWidgetItem=new QTreeWidgetItem(item,strList);
    }
        break;
    case QDomNode::TextNode://text节点
    {
        //改为和attr节点一样
        QString nodeName=node.nodeName();
        QString nodeValue=node.nodeValue();
        QStringList strList;
        strList.append(nodeName);
        strList.append(nodeValue);
        QTreeWidgetItem *treeWidgetItem=new QTreeWidgetItem(item,strList);
    }
        break;
    default:
        break;
    }


//    QString nodeName=node.nodeName();
//    QTreeWidgetItem *treeWidgetItem=new QTreeWidgetItem(item,QStringList(nodeName));
//    for(int i=0;i<node.childNodes().size();i++)
//    {
//        addItemInTree(node.childNodes().at(i),treeWidgetItem);
//    }
}
