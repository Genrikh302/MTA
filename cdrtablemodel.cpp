#include "cdrtablemodel.h"
#include <QSqlRecord>
#include <QDebug>
#include <QSqlQuery>
#include <QDateTime>

#include "qcallog.h"


QMap <unsigned char, QString> QCDRTableModel::causeValue =
{
   {  1, "Unallocated number"},
   {  2, "No route to transit network"},
   {  3, "No route to destination"},
   {  6, "Channel unacceptable"},
   {  7, "Call awarded"},
   {  8, "Preemption"},
   { 16, "Normal call clearing"},
   { 17, "User busy"},
   { 18, "No user responding"},
   { 19, "No answer from user"},
   { 20, "Subscriber absent"},
   { 21, "Call rejected"},
   { 22, "Number changed"},
   { 25, "Exchange routing error"},
   { 26, "Non selected user clearing"},
   { 27, "Destination out of order"},
   { 28, "Invalid number format"},
   { 29, "Facility rejected"},
   { 30, "Response to status qnquiry"},
   { 31, "Normal unspecified"},
   { 34, "No circuit channel available"},
   { 38, "Network out of order"},
   { 41, "Temporary failure"},
   { 42, "Switching equipment congestion"},
   { 43, "Access information discarded"},
   { 44, "Reqsted channel not available"},
   { 46, "Precedence call blocked"},
   { 47, "Resource unavailable unspecif"},
   { 49, "Quality of service unavailable"},
   { 50, "Reqsted facility not subscribed"},
   { 57, "Bearer capability not authrzed"},
   { 58, "Bearer capability not available"},
   { 63, "Service not available unspec"},
   { 65, "Bearer capability not implmted"},
   { 66, "Channel type not implemented"},
   { 69, "Reqsted facility not implmted"},
   { 70, "Only restricted digital bearer"},
   { 79, "Service not implemented"},
   { 81, "Invalid call reference value"},
   { 82, "Identified channel not exist"},
   { 83, "No call ID for suspended call"},
   { 84, "Call identity in use"},
   { 85, "No call suspended"},
   { 86, "Call has been cleared"},
   { 87, "User not a member of CUG"},
   { 88, "Non existent closed user group"},
   { 89, "Called user not member of GUG"},
   { 90, "Called user belongs to GUG"},
   { 91, "Invalid transit network"},
   { 95, "Invalid message unspecified"},
   { 96, "Mandatory IE is missing"},
   { 97, "Messsage type non existent"},
   { 98, "Msg uncompatible non existent"},
   { 99, "IE non existent"},
   {100, "Invalid IE contents"},
   {101, "Message not compatible"},
   {102, "Recovery on timer expiry"},
   {111, "Protocol error unspecified"},
   {127, "Interworking unspecified"}
};

QCDRTableModel::QCDRTableModel(QObject *parent, QSqlDatabase db) : QSqlTableModel(parent, db)
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

            QString val = QString("%1%2%3%4").arg(strtype).arg(str1).arg(strtype == "A" ? "" : str2).arg(strtype == "A" ? "" : str3);
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

        if (index.column() == COL_CRELEASE)
            return causeValue.value(QSqlTableModel::data(QSqlTableModel::index(index.row(), index.column() + 0), role).toInt());

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

