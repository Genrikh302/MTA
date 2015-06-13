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
