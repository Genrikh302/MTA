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
    bool createTabelInternationalCode(); // коды выхода на межнар
    bool createTabelNationalCode(); // коды выхода на междугородку
    bool createTabelDirectionName(); // имена направлений
    bool createTabelDirectionChannel(); // каналы в направлениях
    bool createTabelLoadedFile(); // загруженые файлы

    QSqlDatabase &getDB() {return db;}
private:
    QSqlDatabase db;

};

#endif // QLOGDB_H
