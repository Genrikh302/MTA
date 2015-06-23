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
private:
    QSqlDatabase db;
};

#endif // QLOGDB_H