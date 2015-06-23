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
    QSqlRecord rec;
    QSqlQuery query;

    if(index.column() == 0)
    {
        rec = QSqlTableModel::record(index.row());

        QString strtype =  QSqlTableModel::data(QSqlTableModel::index(index.row(), index.column() + 0), role).toString();//query.value(rec.indexOf("intype")).toString();
        QString str1 =  QSqlTableModel::data(QSqlTableModel::index(index.row(), index.column() + 1), role).toString();
        QString str2 =  QSqlTableModel::data(QSqlTableModel::index(index.row(), index.column() + 2), role).toString();
        if(str2.length() == 2)
        {
            str2 = "0" + str2;
        }
        else if(str2.length() == 1)
        {
            str2 = "00" + str2;
        }
        QString str3 =  QSqlTableModel::data(QSqlTableModel::index(index.row(), index.column() + 3), role).toString();
        if(str3.length() == 2)
        {
            str3 = "0" + str3;
        }
        else if(str3.length() == 1)
        {
            str3 = "00" + str3;
        }

        QString val = QString("%1%2%3%4").arg(strtype).arg(str1).arg(str2).arg(str3);//допилить нули
        return val;
    }
    return value;
}
