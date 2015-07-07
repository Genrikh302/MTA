#ifndef QLOGDB_H
#define QLOGDB_H
#include <QtSql>
#include <QtDebug>

class Qlogdb
{
public:
    Qlogdb();
    ~Qlogdb();
    bool createConnection();
    QSqlError lastError() const {return db.lastError();}

    bool createTabelCDR(); // создаем таблицу
    bool createTabelInternationalCode();
    bool createTabelNationalCode();
    bool createTabelDirectionName();
    bool createTabelDirectionChannel();


    QSqlDatabase &getDB() {return db;}
private:
    QSqlDatabase db;

};

#endif // QLOGDB_H
