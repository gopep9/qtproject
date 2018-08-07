#ifndef REPLACEDIALOG_H
#define REPLACEDIALOG_H

#include <QDialog>

namespace Ui {
class replaceDialog;
}

class replaceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit replaceDialog(QWidget *parent = 0);
    ~replaceDialog();
    QString getReplaceText();
private:
    Ui::replaceDialog *ui;
signals:
    void findNext(const QString&,Qt::CaseSensitivity);
    void replaceNext(const QString&,Qt::CaseSensitivity);
    void replaceAll(const QString&,Qt::CaseSensitivity);
private slots:
    void on_pushButton_FindNext_clicked();
    void on_pushButton_Replace_clicked();
    void on_pushButton_ReplaceAll_clicked();
};

#endif // REPLACEDIALOG_H
