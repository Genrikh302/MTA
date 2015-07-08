#include "cdrtablemodel.h"
#include <QSqlRecord>
#include <QDebug>
#include <QSqlQuery>
#include <QDateTime>

#include "qcallog.h"


QMap <unsigned char, QString> QCDRSortFilterModel::causeValue =
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


QVariant QCDRTableModel::data(const QModelIndex & idx, int role) const
{
    if (!idx.isValid())
        return QVariant();

    if (role == Qt::TextColorRole) { // расскрасим
        int typecall = data(index(idx.row(), QCDRSortFilterModel::COL_CALL_TYPE), Qt::UserRole).toInt();

        switch (typecall) {
            case Qcallog::TYPE_LOCAL: return QBrush(QColor(255, 255, 255));;
            case Qcallog::TYPE_IN_LOCAL: return QBrush(QColor(50, 255, 50));
            case Qcallog::TYPE_OUT_INTERNATIONAL: return QBrush(QColor(20, 20, 20));;
            case Qcallog::TYPE_OUT_NATIONAL: return QBrush(QColor(20, 20, 20));;
            case Qcallog::TYPE_OUT_LOCAL: return QBrush(QColor(100, 200, 100));
            case Qcallog::TYPE_TRANZIT_INTERNATIONAL: return QBrush(QColor(255, 80, 80));
            case Qcallog::TYPE_TRANZIT_NATIONAL: return QBrush(QColor(255, 10, 20));
            case Qcallog::TYPE_TRANZIT_LOCAL: return QBrush(QColor(100, 100, 200));
            default: return QBrush(QColor(0, 0, 0));
        }
    }

    if (role == Qt::DisplayRole || role == Qt::UserRole) {
        if (idx.column() == QCDRSortFilterModel::COL_IN_TYPE || idx.column() == QCDRSortFilterModel::COL_OUT_TYPE) {
            QString strtype = QSqlTableModel::data(idx, Qt::DisplayRole).toString();
            int module = QSqlTableModel::data(QSqlTableModel::index(idx.row(), idx.column() + 1), Qt::DisplayRole).toInt();
            int pcm = QSqlTableModel::data(QSqlTableModel::index(idx.row(), idx.column() + 2), Qt::DisplayRole).toInt();
            int ch = QSqlTableModel::data(QSqlTableModel::index(idx.row(), idx.column() + 3), Qt::DisplayRole).toInt();

            QString str1 = QString("%1").arg(module, 3, 10, QLatin1Char('0'));
            QString str2 = QString("%1").arg(pcm, 3, 10, QLatin1Char('0'));
            QString str3 = QString("%1").arg(ch, 3, 10, QLatin1Char('0'));

            QString val = QString("%1%2%3%4").arg(strtype).arg(str1).arg(strtype == "A" ? "" : str2).arg(strtype == "A" ? "" : str3);
            if (strtype == "C") {
                // попытаемся найти имя этого канала по базу
                qint64 address = ((qint64)module << 32) | (pcm << 16) | ch;
                QSqlQuery query;
                if (query.exec(QString("select name from DirectionChannel, DirectionName where key = id and fr <= %1 and by >= %1").arg(address))) {
                    if (query.next())
                        return query.value(0).toString();
                }
            }

            return val;
        }
    }

    if (role == Qt::DisplayRole) {
        if (idx.column() == QCDRSortFilterModel::COL_DATE)
            return QDate::fromJulianDay(QSqlTableModel::data(idx, role).toLongLong()).toString("dd-MM-yyyy");

        if (idx.column() == QCDRSortFilterModel::COL_TIME)
            return QTime(0, 0).addSecs(QSqlTableModel::data(idx, role).toLongLong()).toString("hh:mm:ss");

        if (idx.column() == QCDRSortFilterModel::COL_TIME_SEIZ)
            return QTime(0, 0).addSecs(QSqlTableModel::data(idx, role).toLongLong()).toString("hh:mm:ss");

        if (idx.column() == QCDRSortFilterModel::COL_TIME_TALK)
            return QTime(0, 0).addSecs(QSqlTableModel::data(idx, role).toLongLong()).toString("hh:mm:ss");

        if (idx.column() == QCDRSortFilterModel::COL_CALL_TYPE)
            return Qcallog::getQStringTypeCalls(data(idx, Qt::UserRole).toInt());

        if (idx.column() == QCDRSortFilterModel::COL_CRELEASE)
            return QCDRSortFilterModel::causeValue.value(QSqlTableModel::data(idx, role).toInt());

    }
    if (role == Qt::UserRole) {
        if (idx.column() == QCDRSortFilterModel::COL_CRELEASE)
            return QSqlTableModel::data(idx, Qt::DisplayRole);

        if (idx.column() == QCDRSortFilterModel::COL_CALL_TYPE) {
            QString strTypeIn = QSqlTableModel::data(index(idx.row(), QCDRSortFilterModel::COL_IN_TYPE), Qt::DisplayRole).toString();
            QString strTypeOut = QSqlTableModel::data(index(idx.row(), QCDRSortFilterModel::COL_OUT_TYPE), Qt::DisplayRole).toString();
            QString strNumberIn = QSqlTableModel::data(index(idx.row(), QCDRSortFilterModel::COL_IN_NUMBER), Qt::DisplayRole).toString();
            QString strNumberOut = QSqlTableModel::data(index(idx.row(), QCDRSortFilterModel::COL_OUT_NUMBER), Qt::DisplayRole).toString();
            return Qcallog::getIntTypeCalls(QStringList() << strTypeIn << strTypeOut << strNumberIn << strNumberOut);
        }
    }

    return QSqlTableModel::data(idx, role);
}


