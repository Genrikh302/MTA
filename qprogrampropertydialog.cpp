#include "qprogrampropertydialog.h"
#include "ui_qprogrampropertydialog.h"

QProgramPropertyDialog::QProgramPropertyDialog(QSqlTableModel *national, QSqlTableModel *international, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QProgramPropertyDialog),
    national(national),
    international(international)
{
    ui->setupUi(this);

    ui->listViewNationalCode->setModel(national);
    ui->listViewInternationalCode->setModel(international);

}

QProgramPropertyDialog::~QProgramPropertyDialog()
{
    delete ui;
}

void QProgramPropertyDialog::on_pushAddNationalCode_clicked()
{
    QString prefix = ui->lineEditNewPrefixNational->text();
    if (prefix.isEmpty())
        return;

    int row = national->rowCount();
    if (!national->insertRow(row))
        qDebug() << "insertRow QProgramPropertyDialog::national" << national->lastError().text();

    national->setData(national->index(row, 0), prefix);
    national->submitAll();
}

void QProgramPropertyDialog::on_pushRemoveNationalCode_clicked()
{

    QItemSelectionModel *selModel = ui->listViewNationalCode->selectionModel();
    if (!selModel)
        return;
    QModelIndexList indexes = selModel->selectedRows();

    if (indexes.isEmpty())
        return;

    foreach (auto i, indexes)
        national->removeRow(i.row());
    national->submitAll();
}

void QProgramPropertyDialog::on_pushClose_clicked()
{
    close();
}

void QProgramPropertyDialog::on_pushAddInternationalCode_clicked()
{
    QString prefix = ui->lineEditNewPrefixInternational->text();
    if (prefix.isEmpty())
        return;

    int row = international->rowCount();
    if (!international->insertRow(row))
        qDebug() << "insertRow QProgramPropertyDialog::international" << international->lastError().text();

    international->setData(international->index(row, 0), prefix);
    international->submitAll();
}

void QProgramPropertyDialog::on_pushRemoveInternationalCode_clicked()
{
    QItemSelectionModel *selModel = ui->listViewInternationalCode->selectionModel();

    if (!selModel)
        return;

    QModelIndexList indexes = selModel->selectedRows();

    if (indexes.isEmpty())
        return;

    foreach (auto i, indexes)
        international->removeRow(i.row());
    international->submitAll();
}
