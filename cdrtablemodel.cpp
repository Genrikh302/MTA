#include "cdrtablemodel.h"
#include <QSqlRecord>
#include <QDebug>
#include <QSqlQuery>
#include <QDateTime>

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
        QString intype =  QSqlTableModel::data(QSqlTableModel::index(index.row(), COL_TYPE_IN), Qt::DisplayRole).toString();
        QString outtype =  QSqlTableModel::data(QSqlTableModel::index(index.row(), COL_TYPE_OUT), Qt::DisplayRole).toString();
        if (intype == "C" && outtype == "C")
            return QBrush(QColor(100, 100, 200));
        if (intype == "C" && outtype == "A")
            return QBrush(QColor(50, 255, 50));
        if (intype == "A" && outtype == "C")
            return QBrush(QColor(100, 200, 100));
    }


    if (role == Qt::DisplayRole) {
        if (index.column() == COL_TYPE_IN || index.column() == COL_TYPE_OUT) {
            QString strtype =  QSqlTableModel::data(QSqlTableModel::index(index.row(), index.column() + 0), role).toString();
            QString str1 =  QSqlTableModel::data(QSqlTableModel::index(index.row(), index.column() + 1), role).toString();
            QString str2 = QString("%1").arg(QSqlTableModel::data(QSqlTableModel::index(index.row(), index.column() + 2), role).toInt(), 3, 10, QLatin1Char('0'));
            QString str3 =  QString("%1").arg(QSqlTableModel::data(QSqlTableModel::index(index.row(), index.column() + 3), role).toInt(), 3, 10, QLatin1Char('0'));

            QString val = QString("%1%2%3%4").arg(strtype).arg(str1).arg(strtype == "A" ? "" : str2).arg(strtype == "A" ? "" : str3);//допилить нули
            return val;
        }

        if (index.column() == COL_DATE_OUT) {
            QDate d;
            d.fromJulianDay(QSqlTableModel::data(QSqlTableModel::index(index.row(), index.column() + 0), role).toLongLong());
            //qDebug() << QSqlTableModel::data(QSqlTableModel::index(index.row(), index.column() + 0), role).toLongLong();
            //qDebug() << d.toString("dd-MM-yyyy");
            return d.toString("dd-MM-yyyy");
        }

    }

    return QSqlTableModel::data(index, role);
}
