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
    ui->xmlTreeView->setAcceptDrops(false);
    this->setAcceptDrops(true);
    model=new QStandardItemModel(this);
    ui->xmlTreeView->setModel(model);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnParseButtonClick()
{
    model->clear();
    model->setHorizontalHeaderLabels(QStringList()<<"key"<<"value"<<"type");
    ui->xmlTreeView->setColumnWidth(0,200);
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
    addItemInTree(root,model);
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

template<typename T>
void MainWindow::addItemInTree(QDomNode node, T *parentItem)
{
    QDomNode::NodeType nodeType=node.nodeType();

    switch (nodeType) {
    case QDomNode::ElementNode://element节点属性和下面的文本
    {
        QString nodeName=node.nodeName();
        //添加item
        QStandardItem* childItem=new QStandardItem(nodeName);
        QStandardItem* secondChildItem=new QStandardItem("");
        QStandardItem* thirdChildItem=new QStandardItem("ELEMENT");
        QList<QStandardItem*> listItem;
        listItem.append(childItem);
        listItem.append(secondChildItem);
        listItem.append(thirdChildItem);
        parentItem->appendRow(listItem);

        ui->xmlTreeView->closePersistentEditor(thirdChildItem->index());

        for(int i=0;i<node.childNodes().size();i++)
        {
            addItemInTree(node.childNodes().at(i),childItem);
        }
        QDomElement element=node.toElement();
        QDomNamedNodeMap attrMap=element.attributes();
        for(int i=0;i<attrMap.size();i++)
        {
            addItemInTree(attrMap.item(i),childItem);
        }

        QDomNode text=element.toText();
        if(text.isText())//有text的时候才显示
            addItemInTree(text,childItem);
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
        //添加item
        QStandardItem* childItem=new QStandardItem(nodeName);
        QStandardItem* secondChildItem=new QStandardItem(nodeValue);
        QStandardItem* thirdChildItem=new QStandardItem("ATTR");
        QList<QStandardItem*> listItem;
        listItem.append(childItem);
        listItem.append(secondChildItem);
        listItem.append(thirdChildItem);
        parentItem->appendRow(listItem);

        ui->xmlTreeView->closePersistentEditor(thirdChildItem->index());

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
        //添加item
        QStandardItem* childItem=new QStandardItem(nodeName);
        QStandardItem* secondChildItem=new QStandardItem(nodeValue);
        QStandardItem* thirdChildItem=new QStandardItem("TEXT");
        QList<QStandardItem*> listItem;
        listItem.append(childItem);
        listItem.append(secondChildItem);
        listItem.append(thirdChildItem);
        parentItem->appendRow(listItem);

        ui->xmlTreeView->closePersistentEditor(thirdChildItem->index());

    }
        break;
    default:
        break;
    }
}

void MainWindow::ergodicItemChild(QStandardItem *item)
{
    for(int i=0;true;i++)
    {
        if(item->child(i)==NULL)
            break;
        QString type=item->child(i,2)->text();
        if(type==QString("ELEMENT"))
        {

        }else if(type==QString("ATTR"))
        {

        }else if(type==QString("TEXT"))
        {

        }
    }
}

void MainWindow::on_savePushButton_clicked()
{
    QStandardItem *item=model->item(0);
    ergodicItemChild(item);
//    for(int i=0;i<item)
//    int a=1;
}
