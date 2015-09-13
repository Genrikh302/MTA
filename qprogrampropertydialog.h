#ifndef QPROGRAMPROPERTYDIALOG_H
#define QPROGRAMPROPERTYDIALOG_H

#include <QDialog>
#include <QSqlTableModel>
#include <QItemDelegate>

namespace Ui {
class QProgramPropertyDialog;
}

class QChannelDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    QChannelDelegate(QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &,
                              const QModelIndex &index) const ;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                          const QModelIndex &index) const;

};

class QProgramPropertyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QProgramPropertyDialog(QSqlTableModel *national, QSqlTableModel *international, QSqlTableModel *directionName, QSqlTableModel *directionChannel, QWidget *parent = 0);
    ~QProgramPropertyDialog();

private slots:
    void on_pushAddNationalCode_clicked();

    void on_pushRemoveNationalCode_clicked();

    void on_pushClose_clicked();

    void on_pushAddInternationalCode_clicked();

    void on_pushRemoveInternationalCode_clicked();

    void on_pushButtonAddName_clicked();

    void on_pushButtonDeleteName_clicked();

    void on_listViewDirectionName_clicked(const QModelIndex &index);

    void on_pushButtonAddChannel_clicked();

    void on_pushButtonDeleteChannel_clicked();

    void on_pushButton_clicked();

private:
    Ui::QProgramPropertyDialog *ui;
    QSqlTableModel *national;
    QSqlTableModel *international;
    QSqlTableModel *directionName;
    QSqlTableModel *directionChannel;
    QSqlTableModel *addedFiles;
};

#endif // QPROGRAMPROPERTYDIALOG_H
