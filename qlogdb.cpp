#include "qlogdb.h"
//#include "qcallog.h"
//#include "mainwindow.h"
Qlogdb::Qlogdb()
{

}

Qlogdb::~Qlogdb()
{

}

bool Qlogdb::createConnection()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");
//    db.setDatabaseName("logbase.db");
    if (!db.open()) {
       qDebug() << "Cannot open database:" << db.lastError();
       return false;
    }
    return true;
}

bool Qlogdb::createTabelCDR()
{
    QSqlQuery query("",db);
    QString str = "CREATE TABLE logbase ( "    //проверить if not exists
                  "intype VARCHAR(1),"
                  "ininc1 int, "
                  "ininc2 int, "
                  "ininc3 int, "
                  "inanum VARCHAR(28), "
                  "innum VARCHAR(28), "
                  "outtype VARCHAR(1), "
                  "outinc1 int, "
                  "outinc2 int, "
                  "outinc3 int, "
                  "outanum VARCHAR(28), "
                  "outnum VARCHAR(28), "
                  "date int, "
                  "time int, "
                  "type int, "
                  "linelen int, "
                  "callen int , "
                  "relreason int, "
                  "id integer primary key autoincrement)" ;

    return query.exec(str);
}

// создается таблица для кодов выхода на междугороднюю сеть сеть
bool Qlogdb::createTabelNationalCode()
{
    QSqlQuery query("", db);
    QString str = "create table if not exists NationalCode ( "
            "code varchar(28) )";
    return query.exec(str);
}


// создается таблица для кодов выхода на международную сеть
bool Qlogdb::createTabelInternationalCode()
{
    QSqlQuery query("",db);
    QString str = "create table if not exists InternationalCode ( "
            "code varchar(28) )";
    return query.exec(str);
}

