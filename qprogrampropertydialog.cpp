#include "qprogrampropertydialog.h"
#include "ui_qprogrampropertydialog.h"

#include <QRegExpValidator>

QProgramPropertyDialog::QProgramPropertyDialog(QSqlTableModel *national, QSqlTableModel *international, QSqlTableModel *directionName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QProgramPropertyDialog),
    national(national),
    international(international),
    directionName(directionName)
{
    ui->setupUi(this);

    ui->listViewNationalCode->setModel(national);
    ui->listViewInternationalCode->setModel(international);
    ui->listViewDirectionName->setModel(directionName);

//    QRegExp regExp = QRegExp("(([C,c][0-9]{9,9})|([C,c](([*]{1,1})|([0-9]{3,3}[*]{1,1})|([0-9]{6,6}[*]{1,1})))|([A,a][0-9]{1,10}))");
//    QRegExpValidator *validator = new QRegExpValidator(regExp, this);

//    ui->lineEditFrom->setValidator(validator);
//    ui->lineEditBy->setValidator(validator);

    ui->listViewDirectionName->setColumnHidden(0, true); // id
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

    national->select();
    ui->listViewNationalCode->reset();
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

    international->select();
    ui->listViewInternationalCode->reset();
}


void QProgramPropertyDialog::on_pushButtonAddName_clicked()
{
    QSqlQuery query;
    query.prepare("INSERT INTO  DirectionName (name) "
                               "VALUES (:name)");
    query.bindValue(":name", QString("unknown"));
    if (!query.exec())
        qDebug() << "Unable to insert value" << query.lastError();

    directionName->select();

    int row = directionName->rowCount();
    ui->listViewDirectionName->reset();

    ui->listViewDirectionName->edit(directionName->index(row - 1, 1));

}

void QProgramPropertyDialog::on_pushButtonDeleteName_clicked()
{
    QItemSelectionModel *selModel = ui->listViewDirectionName->selectionModel();

    if (!selModel)
        return;

    QModelIndexList indexes = selModel->selectedRows();

    if (indexes.isEmpty())
        return;

    foreach (auto i, indexes)
        directionName->removeRow(i.row());
    directionName->submitAll();

    directionName->select();
    ui->listViewDirectionName->reset();

}

void QProgramPropertyDialog::on_listViewDirectionName_activated(const QModelIndex &index)
{
    qDebug() << "listViewDirectionName_activated";
}

void QProgramPropertyDialog::on_listViewDirectionName_clicked(const QModelIndex &index)
{
    qDebug() << "listViewDirectionName_clicked";

}
