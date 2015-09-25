#ifndef QSSHSELECTDIALOG_H
#define QSSHSELECTDIALOG_H

#include <QDialog>
#include <QStringList>

namespace Ui {
class QSSHSelectDialog;
}

class QSSHSelectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QSSHSelectDialog(const QStringList &v, QWidget *parent = 0);
    ~QSSHSelectDialog();

    QStringList getSelectedFiles();
private:
    Ui::QSSHSelectDialog *ui;
};

#endif // QSSHSELECTDIALOG_H
