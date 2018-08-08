#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QtXml>
#include <QDragMoveEvent>
#include <QFileDialog>
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

//        QString nodeName=node.nodeName();
        QString nodeValue=node.nodeValue();
        QString type="ELEMENT";
        QStringList strList;
        strList.append(nodeName);
        strList.append(nodeValue);
        strList.append(type);
        QTreeWidgetItem *treeWidgetItem=new QTreeWidgetItem(item,strList);
        ui->xmlTreeWidget->openPersistentEditor(treeWidgetItem);
        ui->xmlTreeWidget->openPersistentEditor(treeWidgetItem,1);
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
        QString type="ATTR";
        QStringList strList;
        strList.append(nodeName);
        strList.append(nodeValue);
        strList.append(type);
        QTreeWidgetItem *treeWidgetItem=new QTreeWidgetItem(item,strList);
        ui->xmlTreeWidget->openPersistentEditor(treeWidgetItem);
        ui->xmlTreeWidget->openPersistentEditor(treeWidgetItem,1);
    }
        break;
    case QDomNode::TextNode://text节点
    {
        //改为和attr节点一样
        QString nodeName=node.nodeName();
        QString nodeValue=node.nodeValue();
        QString type="TEXT";
        QStringList strList;
        strList.append(nodeName);
        strList.append(nodeValue);
        strList.append(type);
        QTreeWidgetItem *treeWidgetItem=new QTreeWidgetItem(item,strList);
        ui->xmlTreeWidget->openPersistentEditor(treeWidgetItem);
        ui->xmlTreeWidget->openPersistentEditor(treeWidgetItem,1);
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

void MainWindow::buildDoc(QTreeWidgetItem *item,QDomNode*node,QDomDocument *doc)
{
    int childCount=item->childCount();
    QString type=item->text(2);
    if(type==QString("ELEMENT"))
    {
        QDomElement element=doc->createElement(item->text(0));
        if(NULL==node)
        {
            doc->appendChild(element);
        }else
        {
            node->appendChild(element);
        }
        for(int i=0;i<childCount;i++)
        {
            buildDoc(item->child(i),&element,doc);
        }
    }else if(type==QString("ATTR"))
    {
        QDomAttr attr=doc->createAttribute(item->text(0));
        attr.setValue(item->text(1));
        QDomElement *element=static_cast<QDomElement*>(node);
        element->setAttributeNode(attr);
    }else if(type==QString("TEXT"))
    {
        QDomText text=doc->createTextNode(item->text(1));
        node->appendChild(text);
    }
}


void MainWindow::on_savePushButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save XML File"), ".", tr("XML Files (*.xml)"));
    if(fileName.isEmpty())
        return;
    QFile db(fileName);
    if (!db.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return;

    QTreeWidgetItem *item=ui->xmlTreeWidget->itemAt(0,0);
    QDomDocument doc;
    QDomProcessingInstruction instruction;
    instruction=doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);

    buildDoc(item,NULL,&doc);
    QTextStream out(&db);
    out.setCodec("UTF-8");
    doc.save(out,4);
    db.close();
}
