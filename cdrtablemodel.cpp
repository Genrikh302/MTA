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
    if (!index.isValid())
        return QVariant();



    if (role == Qt::TextColorRole) { // расскрасим
        QString intype =  QSqlTableModel::data(QSqlTableModel::index(index.row(), 0), Qt::DisplayRole).toString();
        QString outtype =  QSqlTableModel::data(QSqlTableModel::index(index.row(), 6), Qt::DisplayRole).toString();
        if (intype == "C" && outtype == "C")
            return QBrush(QColor(100, 100, 200));
        if (intype == "C" && outtype == "A")
            return QBrush(QColor(50, 255, 50));
        if (intype == "A" && outtype == "C")
            return QBrush(QColor(100, 200, 100));
    }


    if (role == Qt::DisplayRole) {
        if (index.column() == 0) {
            //rec = QSqlTableModel::record(index.row());

            QString strtype =  QSqlTableModel::data(QSqlTableModel::index(index.row(), index.column() + 0), role).toString();//query.value(rec.indexOf("intype")).toString();
            QString str1 =  QSqlTableModel::data(QSqlTableModel::index(index.row(), index.column() + 1), role).toString();
            QString str2 = QString("%1").arg(QSqlTableModel::data(QSqlTableModel::index(index.row(), index.column() + 2), role).toInt(), 3, 10, QLatin1Char('0'));
            QString str3 =  QString("%1").arg(QSqlTableModel::data(QSqlTableModel::index(index.row(), index.column() + 3), role).toInt(), 3, 10, QLatin1Char('0'));

            QString val = QString("%1%2%3%4").arg(strtype).arg(str1).arg(str2).arg(str3);//допилить нули
            return val;
        }
    }

    return QSqlTableModel::data(index, role);
}
