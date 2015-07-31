#include "cdrtablemodel.h"
#include <QSqlRecord>
#include <QDebug>
#include <QSqlQuery>
#include <QDateTime>

#include "qcallog.h"


//QMap <unsigned char, QString> QCDRSortFilterModel::causeValue =
QMap <unsigned char, QPair<QString, QString> > QCDRSortFilterModel::causeValue =
{
   {  1, {"Unallocated number", "Неназначенный номер"} },
   {  2, {"No route to transit network", "Нет маршрута к заданной транзитной сети"} },
   {  3, {"No route to destination", "Нет маршрута к пункту назначения"} },
   {  6, {"Channel unacceptable", "Канал неприемлем"}},
   {  7, {"Call awarded", "Вызов принят и передается по установленному каналу"}},
   {  8, {"Preemption", "Прервание обслуживания вызова"}},
   { 16, {"Normal call clearing", "Нормальное разьединение"}},
   { 17, {"User busy", "Абонент занят"}},
   { 18, {"No user responding", "Абонент не отвечает"}},
   { 19, {"No answer from user", "Нет ответа от абонента"}},
   { 20, {"Subscriber absent", "Абонент отсутствует"}},
   { 21, {"Call rejected", "Вызов отклонен"}},
   { 22, {"Number changed", "Изменен номер"}},
   { 25, {"Exchange routing error", "Ощибка маршрутизацмм"}},
   { 26, {"Non selected user clearing", "Сброс невыбранного пользователя"}},
   { 27, {"Destination out of order", "Неисправность в пункте назначения"}},
   { 28, {"Invalid number format", "Недействительный формат номера"}},
   { 29, {"Facility rejected", "Услуга отклонена"}},
   { 30, {"Response to status qnquiry", "Реакция на запрос запрос состояния"}},
   { 31, {"Normal unspecified", "Нормальный, неспецифицированный"}},
   { 34, {"No circuit channel available", "Нет свободной линии/канала"}},
   { 38, {"Network out of order", "Неисправность сети"}},
   { 40, {"Permanent frame mode connection operational", "Действующее постоянное соединение в кадровом режиме"}},
   { 41, {"Temporary failure", "Временная неисправность"}},
   { 42, {"Switching equipment congestion", "Перегрузка коммутационного оборудования"}},
   { 43, {"Access information discarded", "Информация о доступе отклонена"}},
   { 44, {"Reqsted channel not available", "Требуемый маршрут/канал недоступен"}},
   { 46, {"Precedence call blocked", "Приоритетный вызов заблокирован"}},
   { 47, {"Resource unavailable unspecif", "Ресурсы недоступны, не специфицированы"}},
   { 49, {"Quality of service unavailable", "Качество обслуживания недоступно"}},
   { 50, {"Reqsted facility not subscribed", "Отсутствует подписка на требуемую услугу"}},
   { 57, {"Bearer capability not authrzed", "Функция передачи информации не разрешена"}},
   { 58, {"Bearer capability not available", "Функция передачи информации в настоящее время недоступна"}},
   { 63, {"Service not available unspec", "Услуга или опция недоступна, неспецифицирована"}},
   { 65, {"Bearer capability not implmted", "Функция передачи информации не реализована"}},
   { 66, {"Channel type not implemented", "Тип канала не реализован"}},
   { 69, {"Reqsted facility not implmted", "Запрошенная услуга не реализована"}},
   { 70, {"Only restricted digital bearer", "Доступна только ограниченная функция передачи цифровой информации"}},
   { 79, {"Service not implemented", "Услуга или опция не реализована, неспецифицирована"}},
   { 81, {"Invalid call reference value", "Недействительное значение метки соединения"}},
   { 82, {"Identified channel not exist", "Идентифицированный канал не существует"}},
   { 83, {"No call ID for suspended call", "Существует приостановленный вызов, но его идентификатор не существует"}},
   { 84, {"Call identity in use", "Используемый идентификатор вызова"}},
   { 85, {"No call suspended", "Отсутствуют приостановленные вызовы"}},
   { 86, {"Call has been cleared", "Разъединение соединения для вызова, имееющего запрашиваемый идентификатор вызова"}},
   { 87, {"User not a member of CUG", "Абонент не является членом замкнутой группы пользователей"}},
   { 88, {"Non existent closed user group", "Несовместимый пункт назначения"}},
   { 89, {"Called user not member of GUG", "Вызываемый пользователь не является членом замкнутой группы пользователей"}},
   { 90, {"Called user belongs to GUG", "Несуществующая замкнутая группа пользователей"}},
   { 91, {"Invalid transit network", "Неправильный выбор транзитной сети"}},
   { 95, {"Invalid message unspecified", "Неправильное сообщение, не пецифицировано"}},
   { 96, {"Mandatory IE is missing", "Пропущен обязательный информационный элемент"}},
   { 97, {"Messsage type non existent", "Тип сообщения не существует или не реализован"}},
   { 98, {"Msg uncompatible non existent", "Сообщение не соответствует состоянию вызова или сообщение несуществующего типа или не реализовано"}},
   { 99, {"IE non existent", "Несуществующий или нереализованный информационный элемент"}},
   {100, {"Invalid IE contents", "Недействительное содержимое информационного элемента"}},
   {101, {"Message not compatible", "Сообщение не соответствует состоянию вызова"}},
   {102, {"Recovery on timer expiry", "Восстановление после истечения времени установленного таймера"}},
   {111, {"Protocol error unspecified", "Ошибка протокола, неспецифицирована"}},
   {127, {"Interworking unspecified", "Взаимодействие, неспецифицирована"}},
   {133, {"MTA: close connection", "MTA: оборвалось подключение"}},
   {134, {"MTA: overflow tranmitted buffer", "MTA: переполнение буфера передачи"}},
   {135, {"MTA: found double call id", "MTA: обнаружен двойной идентификатор вызова"}}
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
            case Qcallog::TYPE_LOCAL: return QBrush(QColor(90, 90, 90));;
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
            return QCDRSortFilterModel::causeValue.value(QSqlTableModel::data(idx, role).toInt()).second;

    }
    if (role == Qt::UserRole) {
//        if (idx.column() == QCDRSortFilterModel::COL_CRELEASE)
//            return QSqlTableModel::data(idx, Qt::DisplayRole);

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
