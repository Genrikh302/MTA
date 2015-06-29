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
            char getType() {return type;}
            const QString& getDraft() {return draft;}
            const QString& getInc1() {return inc1;}
            const QString& getInc2() {return inc2;}
            const QString& getInc3() {return inc3;}

            void sWrite(char type, const QString &draft, const QString &inc1, const QString &inc2, const QString &inc3, const QString &num, const QString &anum);
            void getval(char &_type, QString &_draft, QString &_inc1, QString &_inc2,  QString &_inc3, QString &_num, QString &_anum);
            QString sPrint();

            //Заполнение лога данными из потока
            friend QTextStream &operator >> (QTextStream &in, s &log);

    } in, out;

    void print();
    //Заполнение лога данными из потока
    friend QTextStream &operator >> (QTextStream &in, Qcallog &log);
    //Заполнение БД данными из лога
    friend Qlogdb operator << (Qlogdb &logdb, Qcallog &log);
    void getval(char &_intype, QString &_indraft, QString &_ininc1, QString &_ininc2,  QString &_ininc3, QString &_innum, QString &_inanum, char &_outtype, QString &_outdraft, QString &_outinc1, QString &_outinc2,  QString &_outinc3, QString &_outnum, QString &_outanum, QDate &_date, QTime &_time, int &_linelen, int &_callen, int &_relreason);

    // признаки номера
    static const unsigned char CALL_TYPE_INTERNATIONAL = 1;
    static const unsigned char CALL_TYPE_NATIONAL      = 2;
    static const unsigned char CALL_TYPE_LOCAL         = 4;

    static int getTypeCall(const QString &num); // по первым цифрам номера тип номера


    // признаки вызова
    static const unsigned char TYPE_LOCAL                 = 1;
    static const unsigned char TYPE_IN_LOCAL              = 2;
    static const unsigned char TYPE_OUT_INTERNATIONAL     = 3;
    static const unsigned char TYPE_OUT_NATIONAL          = 4;
    static const unsigned char TYPE_OUT_LOCAL             = 5;
    static const unsigned char TYPE_TRANZIT_INTERNATIONAL = 6;
    static const unsigned char TYPE_TRANZIT_NATIONAL      = 7;
    static const unsigned char TYPE_TRANZIT_LOCAL         = 8;

    static int getIntTypeCalls(const QStringList &l);


private:
    QString wholelog;
    QDate date;
    QTime time;
    int linelen;
    int callen;
    int relreason;
};

#endif // QCALLOG_H




