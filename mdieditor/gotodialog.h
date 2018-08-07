#ifndef GOTODIALOG_H
#define GOTODIALOG_H

#include <QDialog>

namespace Ui {
class gotoDialog;
}

class gotoDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit gotoDialog(QWidget *parent = 0);
    ~gotoDialog();
    
private slots:
    void on_pushButton_Ok_clicked();

private:
    Ui::gotoDialog *ui;
signals:
    void gotoLine(int);

};

#endif // GOTODIALOG_H
