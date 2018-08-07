#include "customdialog.h"
#include "ui_customdialog.h"

customDialog::customDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::customDialog)
{
    ui->setupUi(this);
}

customDialog::~customDialog()
{
    delete ui;
}

int customDialog::row()
{
    return ui->highLineEdit->text().toInt();
}

int customDialog::column()
{
    return ui->widthLineEdit->text().toInt();
}

int customDialog::mine()
{
    return ui->mineLineEdit->text().toInt();
}
