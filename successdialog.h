#ifndef SUCCESSDIALOG_H
#define SUCCESSDIALOG_H

#include <QDialog>
#include "graph.h"

namespace Ui {
class SuccessDialog;
}

class SuccessDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SuccessDialog(QWidget *parent = 0);
    QDate getdate(void);
    ~SuccessDialog();

private slots:
    void on_SuccessDialog_accepted();

private:
    Ui::SuccessDialog *ui;
    QDate date;
};

#endif // SUCCESSDIALOG_H
