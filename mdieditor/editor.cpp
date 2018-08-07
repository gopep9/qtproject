#include<QtGui>
#include<QtWidgets>

#include"editor.h"
#include"finddialog.h"
#include"gotodialog.h"
#include"replacedialog.h"
Editor::Editor(QWidget*parent)
    :QPlainTextEdit(parent)
{
    action=new QAction(this);
    action->setCheckable(true);
    connect(action,SIGNAL(triggered()),this,SLOT(show()));
    connect(action,SIGNAL(triggered()),this,SLOT(setFocus()));
    isUntitled=true;
    connect(document(),SIGNAL(contentsChanged()),
            this,SLOT(documentWasModified()));
    setWindowIcon(QPixmap(":/images/document.png"));
    setWindowTitle("[*]");
    setAttribute(Qt::WA_DeleteOnClose);

    finddialog=0;
    gotodialog=0;
    replacedialog=0;

}
void Editor::newFile()
{
    static int documentNumber=1;
    curFile=tr("document%1.txt").arg(documentNumber);
    setWindowTitle(curFile+"[*]");
    action->setText(curFile);
    isUntitled=true;
    ++documentNumber;
}
bool Editor::save()
{
    if(isUntitled){
        return saveAs();
    }else{
        return saveFile(curFile);
    }
}
bool Editor::saveAs()
{
    QString fileName=
            QFileDialog::getSaveFileName(this,tr("Save As"),curFile);
    if(fileName.isEmpty())
        return false;
    return saveFile(fileName);
}
QSize Editor::sizeHint()const
{
    return QSize(72*fontMetrics().width('x'),
                 25*fontMetrics().lineSpacing());
}
Editor*Editor::open(QWidget*parent)
{
    QString fileName=
            QFileDialog::getOpenFileName(parent,tr("Open"),".");
    if(fileName.isEmpty())
        return 0;
    return openFile(fileName,parent);
}
Editor*Editor::openFile(const QString&fileName,QWidget*parent)
{
    Editor*editor=new Editor(parent);
    if(editor->readFile(fileName)){
        editor->setCurrentFile(fileName);
        return editor;
    }else{
        delete editor;
        return 0;
    }
}

void Editor::printf()
{
//    QPrinter printer;
//    QPrintDialog printDialog(&printer,this);
//    if(printDialog.exec()){
//        QTextDocument textDocument;
//        textDocument.setHtml(this->toPlainText());
//        textDocument.print(&printer);
//    }
}

void Editor::find()
{
    if(!finddialog)
    {
        finddialog=new findDialog(this);
        connect(finddialog,SIGNAL(findNext(const QString&,Qt::CaseSensitivity)),
                this,SLOT(findNextOne(const QString&,Qt::CaseSensitivity)));
        connect(finddialog,SIGNAL(findPrevious(const QString&,Qt::CaseSensitivity)),
                this,SLOT(findPreviousOne(const QString&,Qt::CaseSensitivity)));
    }
    finddialog->show();
    finddialog->raise();
    finddialog->activateWindow();
}

void Editor::findNext()
{
    if(!finddialog)
    {
        this->find();
    }
    else{
        if(isItFindNext)
        {
            findNextOne(findStr,findcs);
        }
        else
        {
            findPreviousOne(findStr,findcs);
        }
    }
}


void Editor::date()
{
    QDateTime datetime=QDateTime::currentDateTime();
    QString stdDate;
    stdDate=datetime.toString("hh:mm yyyy/MM/dd");
    this->textCursor().insertText(stdDate);
}

void Editor::replace()
{
    if(!replacedialog)
    {
        replacedialog=new replaceDialog(this);
        connect(replacedialog,SIGNAL(findNext(const QString&,Qt::CaseSensitivity)),
                this,SLOT(findNextOne(const QString&,Qt::CaseSensitivity)));
        connect(replacedialog,SIGNAL(replaceNext(const QString&,Qt::CaseSensitivity)),
                this,SLOT(replaceNext(const QString&,Qt::CaseSensitivity)));
        connect(replacedialog,SIGNAL(replaceAll(const QString&,Qt::CaseSensitivity)),
                this,SLOT(replaceAll(const QString&,Qt::CaseSensitivity)));
    }
    replacedialog->show();
    replacedialog->raise();
    replacedialog->activateWindow();
}

void Editor::jump()
{
    if(!gotodialog)
    {
        gotodialog=new gotoDialog(this);
        connect(gotodialog,SIGNAL(gotoLine(int)),this,SLOT(gotoLine(int)));

    }
    gotodialog->show();
    gotodialog->raise();
    gotodialog->activateWindow();
}

void Editor::autoLinefeed()
{
    if(this->lineWrapMode()==this->NoWrap)
    {
        this->setLineWrapMode(this->WidgetWidth);
    }
    else
    {
        this->setLineWrapMode(this->NoWrap);
    }
}

bool Editor::findNextOne(const QString &str, Qt::CaseSensitivity cs)
{
    bool isItFind;
    if(cs==Qt::CaseSensitive)
        isItFind=this->QPlainTextEdit::find(str,QTextDocument::FindCaseSensitively);
    else
        isItFind=this->QPlainTextEdit::find(str);
    findStr=str;
    findcs=cs;
    isItFindNext=true;
    return isItFind;
}

bool Editor::findPreviousOne(const QString &str, Qt::CaseSensitivity cs)
{
    bool isItFind;
    if(cs==Qt::CaseSensitive)
        isItFind=this->QPlainTextEdit::find(str,
                                       QTextDocument::FindBackward|QTextDocument::FindCaseSensitively);
    else
        isItFind=this->QPlainTextEdit::find(str,QTextDocument::FindBackward);
    findStr=str;
    findcs=cs;
    isItFindNext=false;
        return isItFind;
}

bool Editor::replaceNext(const QString&str, Qt::CaseSensitivity cs)
{
    QTextCursor cursor=this->textCursor();
    bool isItReplace;
    if(cursor.selectedText().isEmpty()||cursor.selectedText()!=str)
    {
        isItReplace=findNextOne(str,cs);
    }
    else
    {
        replaceText(replacedialog->getReplaceText());
        isItReplace=findNextOne(str,cs);
    }
    return isItReplace;
}

bool Editor::replaceAll(const QString&str, Qt::CaseSensitivity cs)
{
    QTextCursor cursor=this->textCursor();
    cursor.setPosition(0);
    this->setTextCursor(cursor);
    bool isItSuccess=true;
    isItSuccess=findNextOne(str,cs);
    while(isItSuccess)
    {
        replaceText(replacedialog->getReplaceText());
        isItSuccess=findNextOne(str,cs);
    }
    return true;
}

bool Editor::gotoLine(int line)
{
    QTextCursor cursor=this->textCursor();
    cursor.setPosition(lineToposition(line));
    this->setTextCursor(cursor);
}


void Editor::closeEvent(QCloseEvent*event)
{
    if(okToContinue()){
        event->accept();
    }
    else{
        event->ignore();
    }
}
void Editor::documentWasModified()
{
    setWindowModified(true);
}
bool Editor::okToContinue()
{
    if(document()->isModified()){
        int r=QMessageBox::warning(this,tr("MDI Editor"),
                                   tr("File %1 has been modified.\n"
                                      "Do you want to save your changes?")
                                   .arg(strippedName(curFile)),
                                   QMessageBox::Yes|QMessageBox::No
                                   |QMessageBox::Cancel);
        if(r==QMessageBox::Yes){
            return save();
        }
        else if(r==QMessageBox::Cancel){
            return false;
        }
    }
    return true;
}
bool Editor::saveFile(const QString &fileName)
{
    if(writeFile(fileName)){
        setCurrentFile(fileName);
        return true;
    }else{
        return false;
    }
}
void Editor::setCurrentFile(const QString &fileName)
{
    curFile=fileName;
    isUntitled=false;
    action->setText(strippedName(curFile));
    document()->setModified(false);
    setWindowTitle(strippedName(curFile)+"[*]");
    setWindowModified(false);
}
bool Editor::readFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text)){
        QMessageBox::warning(this,tr("MDI Editor"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(file.fileName())
                             .arg(file.errorString()));
        return false;
        }
    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    setPlainText(in.readAll());
    QApplication::restoreOverrideCursor();
    return true;
}
bool Editor::writeFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text)){
        QMessageBox::warning(this,tr("MDI Editor"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(file.fileName())
                             .arg(file.errorString()));
        return false;
    }
    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out<<toPlainText();
    QApplication::restoreOverrideCursor();
    return true;
}
QString Editor::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void Editor::replaceText(const QString &str)
{
    this->textCursor().deletePreviousChar();
    QTextCursor cursor=this->textCursor();
    cursor.insertText(str);
}

int Editor::lineToposition(int line)
{
    QString str=this->toPlainText();
    QString::ConstIterator it=str.constBegin();
    QTextCursor cursor=this->textCursor();
    line--;
    if(line==0)
    {
        return 0;
    }
    int pos=0;
    while(line&&it!=str.constEnd())
    {
        if(*it=='\n')
        {
            line--;
            if(line>0)
            {
            }
            else
            {
                break;
            }
        }
        else
        {
        }
        it++;
        pos++;
    }
    if(it==str.constEnd())
        return pos;
    else
        return pos+1;
}
