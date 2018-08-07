#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>

namespace Ui {
class findDialog;
}
//class MainWindow;
class findDialog : public QDialog
{
    Q_OBJECT
//    friend class MainWindow;
public:
    explicit findDialog(QWidget *parent = 0);
    ~findDialog();
    
private slots:
    void on_pushButton_clicked();
    void enableFindButton(const QString&text);

private:
    Ui::findDialog *ui;
signals:
    void findNext(const QString&str,Qt::CaseSensitivity);
    void findPrevious(const QString&str,Qt::CaseSensitivity);
};

#endif // FINDDIALOG_H
