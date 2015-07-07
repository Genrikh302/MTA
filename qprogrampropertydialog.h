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
    explicit QProgramPropertyDialog(QSqlTableModel *national, QSqlTableModel *international, QSqlTableModel *directionName, QWidget *parent = 0);
    ~QProgramPropertyDialog();

private slots:
    void on_pushAddNationalCode_clicked();

    void on_pushRemoveNationalCode_clicked();

    void on_pushClose_clicked();

    void on_pushAddInternationalCode_clicked();

    void on_pushRemoveInternationalCode_clicked();

    void on_pushButtonAddName_clicked();

    void on_pushButtonDeleteName_clicked();

    void on_listViewDirectionName_activated(const QModelIndex &index);

    void on_listViewDirectionName_clicked(const QModelIndex &index);

private:
    Ui::QProgramPropertyDialog *ui;
    QSqlTableModel *national;
    QSqlTableModel *international;
    QSqlTableModel *directionName;
};

#endif // QPROGRAMPROPERTYDIALOG_H
