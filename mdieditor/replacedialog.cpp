#include "replacedialog.h"
#include "ui_replacedialog.h"

replaceDialog::replaceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::replaceDialog)
{
    ui->setupUi(this);
}

replaceDialog::~replaceDialog()
{
    delete ui;
}

QString replaceDialog::getReplaceText()
{
    return ui->lineEdit_Replace->text();
}

void replaceDialog::on_pushButton_FindNext_clicked()
{
    QString text=ui->lineEdit_Find->text();
    Qt::CaseSensitivity cs=
            ui->checkBox_case_sensitive->isChecked()?Qt::CaseSensitive
                                                   :Qt::CaseInsensitive;
    emit findNext(text,cs);
}

void replaceDialog::on_pushButton_Replace_clicked()
{
    QString text=ui->lineEdit_Find->text();
    Qt::CaseSensitivity cs=
            ui->checkBox_case_sensitive->isChecked()?Qt::CaseSensitive
                                                   :Qt::CaseInsensitive;
    emit replaceNext(text,cs);
}

void replaceDialog::on_pushButton_ReplaceAll_clicked()
{
    QString text=ui->lineEdit_Find->text();
    Qt::CaseSensitivity cs=
            ui->checkBox_case_sensitive->isChecked()?Qt::CaseSensitive
                                                   :Qt::CaseInsensitive;
    emit replaceAll(text,cs);
}
