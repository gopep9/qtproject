#include "gotodialog.h"
#include "ui_gotodialog.h"

gotoDialog::gotoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::gotoDialog)
{
    ui->setupUi(this);
}

gotoDialog::~gotoDialog()
{
    delete ui;
}

void gotoDialog::on_pushButton_Ok_clicked()
{
    emit gotoLine(ui->lineEdit->text().toInt());
    accept();
}
