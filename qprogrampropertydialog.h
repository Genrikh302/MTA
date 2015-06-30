#ifndef QPROGRAMPROPERTYDIALOG_H
#define QPROGRAMPROPERTYDIALOG_H

#include <QDialog>
#include <QSqlTableModel>

namespace Ui {
class QProgramPropertyDialog;
}

class QProgramPropertyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QProgramPropertyDialog(QSqlTableModel *national, QSqlTableModel *international, QWidget *parent = 0);
    ~QProgramPropertyDialog();

private slots:
    void on_pushAddNationalCode_clicked();

    void on_pushRemoveNationalCode_clicked();

    void on_pushClose_clicked();

    void on_pushAddInternationalCode_clicked();

    void on_pushRemoveInternationalCode_clicked();

private:
    Ui::QProgramPropertyDialog *ui;
    QSqlTableModel *national;
    QSqlTableModel *international;
};

#endif // QPROGRAMPROPERTYDIALOG_H
