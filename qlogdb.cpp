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
    //db.setDatabaseName("logbase");
    if (!db.open()) {
       qDebug() << "Cannot open database:" << db.lastError();
       return false;
    }
    return true;
}

bool Qlogdb::createTabel()
{
    QSqlQuery query("",db);
    QString str = "CREATE TABLE logbase ( "    //проверить if not exists
                  "intype VARCHAR(1),"
                  "ininc1 int, "
                  "ininc2 int, "
                  "ininc3 int, "
                  "innum VARCHAR(28), "
                  "inanum int, "
                  "outtype VARCHAR(1), "
                  "outinc1 int, "
                  "outinc2 int, "
                  "outinc3 int, "
                  "outnum VARCHAR(28), "
                  "outanum int, "
                  "date int, "
                  "linelen int, "
                  "callen int , "
                  "relreason int)" ;

    return query.exec(str);
}
