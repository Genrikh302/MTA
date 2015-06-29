#include "cdrtablemodel.h"
#include <QSqlRecord>
#include <QDebug>
#include <QSqlQuery>
#include <QDateTime>

#include "qcallog.h"

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
        int typecall = QSqlTableModel::data(QSqlTableModel::index(index.row(), COL_CALL_TYPE), Qt::DisplayRole).toInt();
        switch (typecall) {
            case Qcallog::TYPE_LOCAL: QBrush(QColor(255, 255, 255));;
            case Qcallog::TYPE_IN_LOCAL: QBrush(QColor(50, 255, 50));
            case Qcallog::TYPE_OUT_INTERNATIONAL: QBrush(QColor(20, 20, 20));;
            case Qcallog::TYPE_OUT_NATIONAL: QBrush(QColor(20, 20, 20));;
            case Qcallog::TYPE_OUT_LOCAL: QBrush(QColor(100, 200, 100));
            case Qcallog::TYPE_TRANZIT_INTERNATIONAL: return QBrush(QColor(255, 80, 80));
            case Qcallog::TYPE_TRANZIT_NATIONAL: return QBrush(QColor(255, 10, 20));
            case Qcallog::TYPE_TRANZIT_LOCAL: return QBrush(QColor(100, 100, 200));
        }
    }


    if (role == Qt::DisplayRole) {
        if (index.column() == COL_IN_TYPE || index.column() == COL_OUT_TYPE) {
            QString strtype =  QSqlTableModel::data(QSqlTableModel::index(index.row(), index.column() + 0), role).toString();
            QString str1 =  QSqlTableModel::data(QSqlTableModel::index(index.row(), index.column() + 1), role).toString();
            QString str2 = QString("%1").arg(QSqlTableModel::data(QSqlTableModel::index(index.row(), index.column() + 2), role).toInt(), 3, 10, QLatin1Char('0'));
            QString str3 =  QString("%1").arg(QSqlTableModel::data(QSqlTableModel::index(index.row(), index.column() + 3), role).toInt(), 3, 10, QLatin1Char('0'));

            QString val = QString("%1%2%3%4").arg(strtype).arg(str1).arg(strtype == "A" ? "" : str2).arg(strtype == "A" ? "" : str3);//допилить нули
            return val;
        }

        if (index.column() == COL_DATE)
            return QDate::fromJulianDay(QSqlTableModel::data(QSqlTableModel::index(index.row(), index.column() + 0), role).toLongLong()).toString("dd-MM-yyyy");

        if (index.column() == COL_TIME)
            return QTime(0, 0).addSecs(QSqlTableModel::data(QSqlTableModel::index(index.row(), index.column() + 0), role).toLongLong()).toString("hh-mm-ss");

        if (index.column() == COL_TIME_SEIZ)
            return QTime(0, 0).addSecs(QSqlTableModel::data(QSqlTableModel::index(index.row(), index.column() + 0), role).toLongLong()).toString("hh-mm-ss");

        if (index.column() == COL_TIME_TALK)
            return QTime(0, 0).addSecs(QSqlTableModel::data(QSqlTableModel::index(index.row(), index.column() + 0), role).toLongLong()).toString("hh-mm-ss");

        if (index.column() == COL_CALL_TYPE)
            return getQStringTypeCalls(QSqlTableModel::data(QSqlTableModel::index(index.row(), index.column() + 0), role).toInt());

    }

    return QSqlTableModel::data(index, role);
}


//int QCDRTableModel::getIntTypeCalls(const QModelIndex &index) const
//{

//    QStringList l = QStringList()
//            << QSqlTableModel::data(QSqlTableModel::index(index.row(), COL_IN_TYPE), Qt::DisplayRole).toString()
//            << QSqlTableModel::data(QSqlTableModel::index(index.row(), COL_OUT_TYPE), Qt::DisplayRole).toString()
//            << QSqlTableModel::data(QSqlTableModel::index(index.row(), COL_IN_NUMBER), Qt::DisplayRole).toString()
//            << QSqlTableModel::data(QSqlTableModel::index(index.row(), COL_OUT_NUMBER), Qt::DisplayRole).toString();
//    return Qcallog::getIntTypeCalls(l);
//}

QString QCDRTableModel::getQStringTypeCalls(int value) const
{
    switch (value) {
        case Qcallog::TYPE_LOCAL: return "Внутренний";
        case Qcallog::TYPE_IN_LOCAL: return "Вход местный";
        case Qcallog::TYPE_OUT_INTERNATIONAL: return "Исход международный";
        case Qcallog::TYPE_OUT_NATIONAL: return "Исход междугородный";
        case Qcallog::TYPE_OUT_LOCAL: return "Исход местный";
        case Qcallog::TYPE_TRANZIT_INTERNATIONAL: return "Международный транзит";
        case Qcallog::TYPE_TRANZIT_NATIONAL: return "Междугородный транзит";
        case Qcallog::TYPE_TRANZIT_LOCAL: return "Транзит";
        default: return "unknown";
    }
}

