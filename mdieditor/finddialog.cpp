#include "finddialog.h"
#include "ui_finddialog.h"

findDialog::findDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::findDialog)
{
    ui->setupUi(this);
    connect(ui->lineEdit,SIGNAL(textChanged(QString)),this,SLOT(enableFindButton(QString)));
}

findDialog::~findDialog()
{
    delete ui;
}

void findDialog::on_pushButton_clicked()
{
    QString text=ui->lineEdit->text();
    Qt::CaseSensitivity cs=
            ui->checkBox_case_sensitive->isChecked()?Qt::CaseSensitive
                                                   :Qt::CaseInsensitive;
    if(ui->radioButton_FindDown->isChecked()){
        emit findNext(text,cs);
    }
    else{
        emit findPrevious(text,cs);
    }
}

void findDialog::enableFindButton(const QString &text)
{
    ui->pushButton->setEnabled(!text.isEmpty());
}


