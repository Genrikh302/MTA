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

QTextStream &operator>>(QTextStream &in, Qcallog &log)
{
    //сделать stringstream, считать целую строку, сделать по ней хэш в базу
    //Разбираем данные о первом абоненте
    char type;
    QString draft;
    QString inc1;
    QString inc2;
    QString inc3;
    QString num;
    QString anum;
    //log.wholelog = intext.readLine();
    //std::stringstream in;
    //std::istringstream in(log.wholelog);
    //in << log.wholelog;
    in >> type;
        in >> draft;
        if(draft.size() == 9){
            inc1 = draft.mid(0,3);
            inc2 = draft.mid(3,3);
            inc3 = draft.mid(6);
        }
        else if(draft.size() == 3){
            inc1 = draft.mid(0,3);
        }
        else{
            qDebug() << "Error reading files";
        }
        in >> num;
        in >> anum;
        log.in.sWrite(type, draft, inc1, inc2, inc3, num, anum);
        //разбираем данные о втором абоненте
        in.skipWhiteSpace();
        in >> type;
        in >> draft;
        if(draft.size() == 9){
            inc1 = draft.mid(0,3);
            inc2 = draft.mid(3,3);
            inc3 = draft.mid(6);
        }
        else if(draft.size() == 3){
            inc1 = draft.mid(0,3);
        }
        else{
            qDebug() << "Error reading files";
        }
        in >> num;
        in >> anum;
        log.out.sWrite(type, draft, inc1, inc2, inc3, num, anum);
    //Разбираем общие данные о звонке
    QString datestr;
    QString timestr;
    in >> datestr;
    in >> timestr;
    datestr += " " + timestr;
    QString dateformat = "dd-MM-yy hh:mm:ss";
    log.date = QDateTime::fromString(datestr, dateformat);
    in >> log.linelen;
    in >> log.callen;
    in >> log.relreason;
    in.skipWhiteSpace();
    return in;
}

void Qcallog::print()
{
     qDebug() << in.sPrint() << out.sPrint() << this->date.toString("dd-MM-yy hh:mm:ss") << this->linelen << this->callen <<this->relreason;
     //qDebug() << this->wholelog;
}

void Qcallog::s::sWrite(char _type, QString _draft, QString _inc1, QString _inc2,  QString _inc3, QString _num, QString _anum)
{
    Qcallog::s::type = _type;
    Qcallog::s::draft = _draft;
    Qcallog::s::inc1 = _inc1;
    Qcallog::s::inc2 = _inc2;
    Qcallog::s::inc3 = _inc3;
    Qcallog::s::num = _num;
    Qcallog::s::anum = _anum;
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

void Qcallog::getval(char &_intype, QString &_indraft, QString &_ininc1, QString &_ininc2,  QString &_ininc3, QString &_innum, QString &_inanum, char &_outtype, QString &_outdraft, QString &_outinc1, QString &_outinc2,  QString &_outinc3, QString &_outnum, QString &_outanum, QDateTime &_date, int &_linelen, int &_callen, int &_relreason)
{
    this->in.getval(_intype, _indraft, _ininc1, _ininc2, _ininc3, _innum, _inanum);
    this->out.getval(_outtype, _outdraft, _outinc1, _outinc2, _outinc3, _outnum, _outanum);
    _date = Qcallog::date;
    _linelen = Qcallog::linelen;
    _callen = Qcallog::callen;
    _relreason = Qcallog::relreason;
}


Qlogdb operator << (Qlogdb &logdb, Qcallog &log)
{
    char intype, outtype;
    QString indraft, ininc1, ininc2, ininc3, innum, inanum, outdraft, outinc1, outinc2, outinc3, outnum, outanum;
    QDateTime date;
    int linelen;
    int callen;
    int relreason;
    bool ok;
    log.getval(intype, indraft, ininc1, ininc2, ininc3, innum, inanum, outtype, outdraft, outinc1, outinc2, outinc3, outnum, outanum, date, linelen, callen, relreason);
    QSqlQuery query;
    query.prepare("INSERT INTO  logbase (intype, ininc1, ininc2, ininc3, innum, inanum, outtype, outinc1, outinc2, outinc3, outnum, outanum, date,  linelen, callen, relreason) VALUES (:intype, :ininc1, :ininc2, :ininc3, :innum, :inanum, :outtype, :outinc1, :outinc2, :outinc3, :outnum, :outanum, :date,  :linelen, :callen, :relreason)");
    query.bindValue(":intype", QString(intype));
    query.bindValue(":ininc1", ininc1.toInt(&ok, 10));
    query.bindValue(":ininc2", ininc2.toInt(&ok, 10));
    query.bindValue(":ininc3", ininc3.toInt(&ok, 10));
    query.bindValue(":innum", innum);
    query.bindValue(":inanum", inanum.toInt(&ok, 10));
    query.bindValue(":outtype", QString(outtype));
    query.bindValue(":outinc1", outinc1.toInt(&ok, 10));
    query.bindValue(":outinc2", outinc2.toInt(&ok, 10));
    query.bindValue(":outinc3", outinc3.toInt(&ok, 10));
    query.bindValue(":outnum", outnum);
    query.bindValue(":outanum", outanum.toInt(&ok, 10));
    query.bindValue(":date", date.toTime_t()); //преобразовать в нормальную дату
    query.bindValue(":linelen", linelen);
    query.bindValue(":callen", callen);
    query.bindValue(":relreason", relreason);
    if (!query.exec()) {
        qDebug() << "Unable to insert value"<<logdb.lastError();
    }
    return logdb;
}

