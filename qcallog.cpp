#include "qcallog.h"
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <sstream>


Qcallog::Qcallog()
{

}

Qcallog::~Qcallog()
{

}

QTextStream &operator>>(QTextStream &in, Qcallog &log) throw(std::invalid_argument)
{
    //сделать stringstream, считать целую строку, сделать по ней хэш в базу
    //Разбираем данные о первом абоненте

    //разбираем данные о первом абоненте    
    in >> log.in;

    //разбираем данные о втором абоненте
    in >> log.out;

    //Разбираем общие данные о звонке
    QString datestr;
    QString timestr;
    in >> datestr;
    in >> timestr;

    log.date = QDate::fromString(datestr, "dd-MM-yy");
    log.date = log.date.addYears(100);
    log.time = QTime::fromString(timestr, "hh:mm:ss");

    in >> log.linelen;
    in >> log.callen;
    in >> log.relreason;

    in.skipWhiteSpace();
    return in;
}

void Qcallog::print()
{
     qDebug() << in.sPrint() << out.sPrint() << this->date.toString("dd-MM-yy hh:mm:ss") << this->linelen << this->callen <<this->relreason;
}

void Qcallog::s::sWrite(char type, const QString &draft, const QString &inc1, const QString &inc2, const QString &inc3, const QString &num, const QString &anum)
{
    this->type = type;
    this->draft = draft;
    this->inc1 = inc1;
    this->inc2 = inc2;
    this->inc3 = inc3;
    this->num = num;
    this->anum = anum;
}

QString Qcallog::s::sPrint()
{
    QString stringpart;
    stringpart[0] = this->type;
    stringpart += this->draft + " " + this->num + " " + this->anum;
    return stringpart;
}

void Qcallog::s::getval(char &_type, QString &_draft, QString &_inc1, QString &_inc2,  QString &_inc3, QString &_num, QString &_anum)
{
    _type = Qcallog::s::type;
    _draft = Qcallog::s::draft;
    _inc1 = Qcallog::s::inc1;
    _inc2 = Qcallog::s::inc2;
    _inc3 = Qcallog::s::inc3;
    _num = Qcallog::s::num;
    _anum = Qcallog::s::anum;
}

//Заполнение лога данными из потока
QTextStream &operator >> (QTextStream &in, Qcallog::s &log) throw(std::invalid_argument)
{
    QString draft;
    in.skipWhiteSpace();
    in >> log.type;
    in >> draft;
    if (log.type == 'C' || log.type == 'c') {
        log.type = 'C';
        log.inc1 = draft.mid(0,3);
        log.inc2 = draft.mid(3,3);
        log.inc3 = draft.mid(6);
    }
    else if (log.type == 'A' || log.type == 'a') {
        log.type = 'A';
        log.inc1 = draft;
        log.inc2 = log.inc3 = "-";
    }
    else
        throw(std::invalid_argument("Ошибка чтения из потока, не известное значение типа канала"));

    if (draft == "-")
        log.anum = draft;
    else
        in >> log.anum;

    in >> log.num;
    return in;
}

void Qcallog::getval(char &_intype, QString &_indraft, QString &_ininc1, QString &_ininc2,  QString &_ininc3, QString &_innum, QString &_inanum, char &_outtype, QString &_outdraft, QString &_outinc1, QString &_outinc2,  QString &_outinc3, QString &_outnum, QString &_outanum, QDate &_date, QTime &_time, int &_linelen, int &_callen, int &_relreason)
{
    this->in.getval(_intype, _indraft, _ininc1, _ininc2, _ininc3, _innum, _inanum);
    this->out.getval(_outtype, _outdraft, _outinc1, _outinc2, _outinc3, _outnum, _outanum);
    _date = Qcallog::date;
    _time = Qcallog::time;
    _linelen = Qcallog::linelen;
    _callen = Qcallog::callen;
    _relreason = Qcallog::relreason;
}


Qlogdb operator << (Qlogdb &logdb, Qcallog &log)
{
    char intype, outtype;
    QString indraft, ininc1, ininc2, ininc3, innum, inanum, outdraft, outinc1, outinc2, outinc3, outnum, outanum;
    QDate date;
    QTime time;
    int linelen;
    int callen;
    int relreason;
    int filekey;
    bool ok;
    log.getval(intype, indraft, ininc1, ininc2, ininc3, innum, inanum, outtype, outdraft, outinc1, outinc2, outinc3, outnum, outanum, date, time, linelen, callen, relreason);
    filekey = log.getFilekey();
    QSqlQuery query;

    query.prepare("INSERT INTO  logbase (intype, ininc1, ininc2, ininc3, innum, inanum, outtype, outinc1, outinc2, outinc3, outnum, outanum, date, time, type, linelen, callen, relreason, filekey) "
                               "VALUES (:intype, :ininc1, :ininc2, :ininc3, :innum, :inanum, :outtype, :outinc1, :outinc2, :outinc3, :outnum, :outanum, :date, :time, :type, :linelen, :callen, :relreason, :filekey)");
    query.bindValue(":intype", QString(intype));
    query.bindValue(":ininc1", ininc1.toInt(&ok, 10));
    query.bindValue(":ininc2", ininc2.toInt(&ok, 10));
    query.bindValue(":ininc3", ininc3.toInt(&ok, 10));
    query.bindValue(":innum", innum);
    query.bindValue(":inanum", inanum);
    query.bindValue(":outtype", QString(outtype));
    query.bindValue(":outinc1", outinc1.toInt(&ok, 10));
    query.bindValue(":outinc2", outinc2.toInt(&ok, 10));
    query.bindValue(":outinc3", outinc3.toInt(&ok, 10));
    query.bindValue(":outnum", outnum);
    query.bindValue(":outanum", outanum);
    query.bindValue(":date", date.toJulianDay()); //преобразовать в нормальную дату
    query.bindValue(":time", QTime(0,0).secsTo(time));
    query.bindValue(":type", 0);
    query.bindValue(":linelen", linelen);
    query.bindValue(":callen", callen);
    query.bindValue(":relreason", relreason);
    query.bindValue(":filekey", filekey);
    if (!query.exec())
        qDebug() << "Unable to insert value" << query.lastError();

    return logdb;
}

//void operator << (QCDRTableModel &m, Qcallog &log)
//{
//    char intype, outtype;
//    QString indraft, ininc1, ininc2, ininc3, innum, inanum, outdraft, outinc1, outinc2, outinc3, outnum, outanum;
//    QDate date;
//    QTime time;
//    int linelen;
//    int callen;
//    int relreason;
//    bool ok;
//    log.getval(intype, indraft, ininc1, ininc2, ininc3, innum, inanum, outtype, outdraft, outinc1, outinc2, outinc3, outnum, outanum, date, time, linelen, callen, relreason);
//    QSqlQuery query;

//    int calltype = Qcallog::getIntTypeCalls(QStringList() << QString(intype) << QString(outtype) << innum << outnum);

//    int row = m.rowCount();
//    if (!m.insertRow(row))
//        qDebug() << "insertRow nationalCode" << m.lastError().text();
//    m.setData(m.index(row, m.fieldIndex("intype")), QString(intype));
//    m.setData(m.index(row, m.fieldIndex("ininc1")), ininc1.toInt(&ok, 10));
//    m.setData(m.index(row, m.fieldIndex("ininc2")), ininc2.toInt(&ok, 10));
//    m.setData(m.index(row, m.fieldIndex("ininc3")), ininc3.toInt(&ok, 10));
//    m.setData(m.index(row, m.fieldIndex("innum")), innum);
//    m.setData(m.index(row, m.fieldIndex("inanum")), inanum);
//    m.setData(m.index(row, m.fieldIndex("outtype")), QString(outtype));
//    m.setData(m.index(row, m.fieldIndex("outinc1")), outinc1.toInt(&ok, 10));
//    m.setData(m.index(row, m.fieldIndex("outinc2")), outinc2.toInt(&ok, 10));
//    m.setData(m.index(row, m.fieldIndex("outinc3")), outinc3.toInt(&ok, 10));
//    m.setData(m.index(row, m.fieldIndex("outnum")), outnum);
//    m.setData(m.index(row, m.fieldIndex("outanum")), outanum);
//    m.setData(m.index(row, m.fieldIndex("date")), date.toJulianDay());
//    m.setData(m.index(row, m.fieldIndex("time")), QTime(0,0).secsTo(time));
//    m.setData(m.index(row, m.fieldIndex("type")), calltype);
//    m.setData(m.index(row, m.fieldIndex("linelen")), linelen);
//    m.setData(m.index(row, m.fieldIndex("callen")), callen);
//    m.setData(m.index(row, m.fieldIndex("relreason")), relreason);
//}

int Qcallog::getTypeCall(const QString &num)
{
    const QStringList prefix_national = MainWindow::getNationalPrefix();
    const QStringList prefix_international = MainWindow::getInternationalPrefix();

    // возможно еще длину проверим
    for (const QString prefix : prefix_international)
        if (num.startsWith(prefix))
            return CALL_TYPE_INTERNATIONAL;

    for (const QString prefix : prefix_national)
        if (num.startsWith(prefix))
            return CALL_TYPE_NATIONAL;

    return CALL_TYPE_LOCAL;
}

int Qcallog::getIntTypeCalls(const QStringList &l)
{
    QString intype = l[0];
    QString outtype = l[1];

    QString num1 = l[2];
    QString num2 = l[3];

    int typecall1 = getTypeCall(num1);
    int typecall2 = getTypeCall(num2);

    int typecall = typecall1 | typecall2;

    if (typecall & CALL_TYPE_INTERNATIONAL)
        typecall = CALL_TYPE_INTERNATIONAL;

    if (typecall & CALL_TYPE_NATIONAL)
        typecall = CALL_TYPE_NATIONAL;

    if (typecall & CALL_TYPE_LOCAL)
        typecall = CALL_TYPE_LOCAL;

    if (intype == "A" && outtype == "A")
        return TYPE_LOCAL;

    if (intype == "C" && outtype == "A")
        return TYPE_IN_LOCAL;

    if (intype == "A" && outtype == "C") {
        switch (typecall) {
            case CALL_TYPE_INTERNATIONAL: return TYPE_OUT_INTERNATIONAL;
            case CALL_TYPE_NATIONAL: return TYPE_OUT_NATIONAL;
            default: return TYPE_OUT_LOCAL;
        }
    }

    if (intype == "C" && outtype == "C") {
        switch (typecall) {
            case CALL_TYPE_INTERNATIONAL: return TYPE_TRANZIT_INTERNATIONAL;
            case CALL_TYPE_NATIONAL: return TYPE_TRANZIT_NATIONAL;
            default: return TYPE_TRANZIT_LOCAL;
        }
    }
    return TYPE_OUT_LOCAL;
}

QString Qcallog::getQStringTypeCalls(int value)
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
void Qcallog::setFilekey(int value)
{
    filekey = value;
}
int Qcallog::getFilekey() const
{
    return filekey;
}


