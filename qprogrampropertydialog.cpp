#include "qprogrampropertydialog.h"
#include "ui_qprogrampropertydialog.h"

#include <QRegExpValidator>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

//TODO Сделать выбор цвтов для расскрашивания типов вызовов
//TODO Сделать работу по старому формату xx-xx-xx, а не xxx-xxx-xxx

QProgramPropertyDialog::QProgramPropertyDialog(QSqlTableModel *national, QSqlTableModel *international, QSqlTableModel *directionName, QSqlTableModel *directionChannel, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QProgramPropertyDialog),
    national(national),
    international(international),
    directionName(directionName),
    directionChannel(directionChannel)
{
    ui->setupUi(this);


    ui->listViewNationalCode->setModel(national);
    ui->listViewInternationalCode->setModel(international);
    ui->listViewDirectionName->setModel(directionName);

    directionChannel->setFilter("key = -1");
    ui->tableViewDirectionChannel->setModel(directionChannel);

//    QRegExp regExp = QRegExp("(([C,c][0-9]{9,9})|([C,c](([*]{1,1})|([0-9]{3,3}[*]{1,1})|([0-9]{6,6}[*]{1,1})))|([A,a][0-9]{1,10}))");
//    QRegExpValidator *validator = new QRegExpValidator(regExp, this);

//    ui->lineEditFrom->setValidator(validator);
//    ui->lineEditBy->setValidator(validator);

    ui->listViewDirectionName->setColumnHidden(0, true); // id
    ui->tableViewDirectionChannel->setColumnHidden(0, true); // key


    ui->tableViewDirectionChannel->setItemDelegate(new QChannelDelegate());
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
    query.prepare("insert into DirectionName (name) "
                               "values (:name)");
    query.bindValue(":name", QString("unknown"));
    if (!query.exec())
        qDebug() << "Unable to insert value" << query.lastError();

    directionName->select();

    int row = directionName->rowCount();
    ui->listViewDirectionName->reset();

    ui->listViewDirectionName->edit(directionName->index(row - 1, 1));
    ui->listViewDirectionName->selectRow(row - 1);

    directionChannel->setFilter(QString("key = -1"));
}

void QProgramPropertyDialog::on_pushButtonDeleteName_clicked()
{
    QItemSelectionModel *selModel = ui->listViewDirectionName->selectionModel();

    if (!selModel)
        return;

    QModelIndexList indexes = selModel->selectedRows();

    if (indexes.isEmpty())
        return;

    foreach (auto i, indexes) {
        int id = directionName->data(directionName->index(i.row(), 0)).toInt();
        // нужно удалить все каналы связанные с этим именем
        QSqlQuery query;
        if (query.exec(QString("delete from DirectionChannel where key = %1").arg(id)))
            qDebug() << "Unable to insert value" << query.lastError();

        directionName->removeRow(i.row());
    }
    directionName->submitAll();

    directionName->select();
    ui->listViewDirectionName->reset();

    directionChannel->select();
    ui->tableViewDirectionChannel->reset();
}

void QProgramPropertyDialog::on_listViewDirectionName_clicked(const QModelIndex &idx)
{
    if (!idx.isValid())
        return;

    QItemSelectionModel *selModel = ui->listViewDirectionName->selectionModel();

    if (!selModel)
        return;

    QModelIndexList indexes = selModel->selectedRows();

    if (indexes.count() != 1) {
        directionChannel->setFilter(QString("key = -1"));
        return;
    }


    int id = directionName->data(directionName->index(idx.row(), 0)).toInt();


    directionChannel->setFilter(QString("key = %1").arg(id));
}


void QProgramPropertyDialog::on_pushButtonAddChannel_clicked()
{
    QItemSelectionModel *selModel = ui->listViewDirectionName->selectionModel();

    if (!selModel)
        return;

    QModelIndexList indexes = selModel->selectedRows();

    if (indexes.count() != 1)
        return;

    int id = directionName->data(directionName->index(indexes[0].row(), 0)).toInt();

    int row = directionChannel->rowCount();
    if (!directionChannel->insertRow(row))
        qDebug() << "insertRow QProgramPropertyDialog::directionChannel" << directionChannel->lastError().text();

    directionChannel->setData(directionChannel->index(row, 0), id);
    directionChannel->setData(directionChannel->index(row, 1), 0);
    directionChannel->setData(directionChannel->index(row, 2), 0);

    ui->tableViewDirectionChannel->selectRow(row);
    ui->tableViewDirectionChannel->edit(directionChannel->index(row, 1));
}

void QProgramPropertyDialog::on_pushButtonDeleteChannel_clicked()
{
    QItemSelectionModel *selModel = ui->tableViewDirectionChannel->selectionModel();

    if (!selModel)
        return;

    QModelIndexList indexes = selModel->selectedRows();

    foreach (auto i, indexes)
        directionChannel->removeRow(i.row());
    directionChannel->submitAll();

    directionChannel->select();
    ui->tableViewDirectionChannel->reset();
}


QChannelDelegate::QChannelDelegate(QObject *parent)
    : QItemDelegate(parent) {}


QWidget *QChannelDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const
{
    QLineEdit *editor = new QLineEdit(parent);

    QRegExp regExp = QRegExp("(([C,c][0-9]{9,9})|([C,c](([*]{1,1})|([0-9]{3,3}[*]{1,1})|([0-9]{6,6}[*]{1,1}))))");
    QRegExpValidator *validator = new QRegExpValidator(regExp);
    editor->setValidator(validator);

    return editor;
}

void QChannelDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QLineEdit *edit = qobject_cast<QLineEdit*>(editor);
    if (edit) {         
        edit->setText(index.model()->data(index, Qt::DisplayRole).toString());
        return;
     }
}

void QChannelDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit *edit = qobject_cast<QLineEdit *>(editor);

    if (edit) {
        QString value = edit->text();
        QRegularExpression r("([C,c](\\d{3,3})(\\d{3,3})(\\d{3,3}))");
        QRegularExpressionMatch m = r.match(value);
//        QString sa1 = m.captured(2);
//        QString sa2 = m.captured(3);
//        QString sa3 = m.captured(4);
//        qint64 a1 = m.captured(2).toLongLong();
//        qint64 a2 = m.captured(3).toLongLong();
//        qint64 a3 = m.captured(4).toLongLong();
        if (m.hasMatch())
            model->setData(index, (m.captured(2).toLongLong() << 32) | (m.captured(3).toLongLong() << 16) | m.captured(4).toLongLong());

    }
}
