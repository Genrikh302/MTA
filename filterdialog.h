#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

#include <QDialog>

namespace Ui {
class FilterDialog;
}

class FilterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FilterDialog(QWidget *parent = 0);
    QString ab1;
    void getfil(QString &abfd1);
    ~FilterDialog();

private slots:

private:
    Ui::FilterDialog *ui;
};

#endif // FILTERDIALOG_H
