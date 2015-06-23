#ifndef QCALLOG_H
#define QCALLOG_H
#include "mainwindow.h"
#include "qlogdb.h"
class Qcallog
{
public:
    Qcallog();
    ~Qcallog();
    class s{
        private: //заменить на private и написать методы для доступа
            char type;
            QString draft;
            QString inc1;
            QString inc2;
            QString inc3;
            QString num;
            QString anum;
        public:
            void sWrite(char _type, QString _draft, QString _inc1, QString _inc2,  QString _inc3, QString _num, QString _anum);
            void getval(char &_type, QString &_draft, QString &_inc1, QString &_inc2,  QString &_inc3, QString &_num, QString &_anum);
            QString sPrint();
    } in, out;
    void print();
    //Заполнение лога данными из потока
    friend QTextStream &operator >> (QTextStream &in, Qcallog &log);
    //Заполнение БД данными из лога
    friend Qlogdb operator << (Qlogdb &logdb, Qcallog &log);
    void getval(char &_intype, QString &_indraft, QString &_ininc1, QString &_ininc2,  QString &_ininc3, QString &_innum, QString &_inanum, char &_outtype, QString &_outdraft, QString &_outinc1, QString &_outinc2,  QString &_outinc3, QString &_outnum, QString &_outanum, QDateTime &_date, int &_linelen, int &_callen, int &_relreason);

private:
    QString wholelog;
    QDateTime date;
    int linelen;
    int callen;
    int relreason;
};

#endif // QCALLOG_H




