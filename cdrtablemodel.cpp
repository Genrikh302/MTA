#include "cdrtablemodel.h"
#include <QSqlRecord>
#include <QDebug>
#include <QSqlQuery>

QCDRTableModel::QCDRTableModel() : QSqlTableModel()
{

}

QCDRTableModel::~QCDRTableModel()
{

}

QVariant QCDRTableModel::data(const QModelIndex & index, int role) const
{
    if (role != Qt::DisplayRole) return QVariant();
    if (!index.isValid()) return QVariant();

    QVariant value = QSqlTableModel::data(index, role);
//    QSqlRecord rec;
//    QSqlQuery query;

    if(index.column() == 0)
    {
        //rec = QSqlTableModel::record(index.row());

        QString strtype =  QSqlTableModel::data(QSqlTableModel::index(index.row(), index.column() + 0), role).toString();//query.value(rec.indexOf("intype")).toString();
        QString str1 =  QSqlTableModel::data(QSqlTableModel::index(index.row(), index.column() + 1), role).toString();
        QString str2 = QString("%1").arg(QSqlTableModel::data(QSqlTableModel::index(index.row(), index.column() + 2), role).toInt(), 3, 10, QLatin1Char('0'));
        QString str3 =  QString("%1").arg(QSqlTableModel::data(QSqlTableModel::index(index.row(), index.column() + 3), role).toInt(), 3, 10, QLatin1Char('0'));

        QString val = QString("%1%2%3%4").arg(strtype).arg(str1).arg(str2).arg(str3);//допилить нули
        return val;
    }
    return value;
}
