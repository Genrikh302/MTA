#include "qsshselectdialog.h"
#include "ui_qsshselectdialog.h"

#include <QStringListModel>

QSSHSelectDialog::QSSHSelectDialog(const QStringList &v, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QSSHSelectDialog)
{
    ui->setupUi(this);

    ui->listViewSSHFileName->setModel(new  QStringListModel(v));

}

QSSHSelectDialog::~QSSHSelectDialog()
{
    delete ui;
}

QStringList QSSHSelectDialog::getSelectedFiles()
{
    QStringList list;
    for(const QModelIndex &index: ui->listViewSSHFileName->selectionModel()->selectedIndexes())
        list.append(ui->listViewSSHFileName->model()->data(index).toString());

    return list;

}
