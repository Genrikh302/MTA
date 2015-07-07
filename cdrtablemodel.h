#ifndef MYTABLEMODEL_H
#define MYTABLEMODEL_H

#include <QObject>
#include <QSqlTableModel>
#include <QSortFilterProxyModel>
#include <QMap>


class QCDRSortFilterModel : public QSortFilterProxyModel
{
private:
    int getIntTypeCalls(const QModelIndex &index) const; //

public:
    QCDRSortFilterModel(QObject *parent = 0) : QSortFilterProxyModel(parent) {}

    static const unsigned char COL_IN_TYPE    = 0;
    static const unsigned char COL_IN_MN      = 1; // In module number
    static const unsigned char COL_IN_SLPCM   = 2; // In slot/pcm number
    static const unsigned char COL_IN_PORT    = 3; // In port number
    static const unsigned char COL_IN_AON     = 4; // In AON number
    static const unsigned char COL_IN_NUMBER  = 5; // In Number
    static const unsigned char COL_OUT_TYPE   = 6;
    static const unsigned char COL_OUT_MN     = 7;
    static const unsigned char COL_OUT_SLPCM  = 8;
    static const unsigned char COL_OUT_PORT   = 9;
    static const unsigned char COL_OUT_AON    = 10;
    static const unsigned char COL_OUT_NUMBER = 11;
    static const unsigned char COL_DATE       = 12;    
    static const unsigned char COL_TIME       = 13;
    static const unsigned char COL_CALL_TYPE  = 14;
    static const unsigned char COL_TIME_SEIZ  = 15;
    static const unsigned char COL_TIME_TALK  = 16;
    static const unsigned char COL_CRELEASE   = 17;
    static const unsigned char COL_ID         = 18;

    static QMap <unsigned char, QString> causeValue;


};

class QCDRTableModel : public QSqlTableModel
{

public:
    QCDRTableModel(QObject *parent = 0, QSqlDatabase db = QSqlDatabase());
    virtual QVariant data(const QModelIndex & idx, int role = Qt::DisplayRole) const;
    ~QCDRTableModel();

};

#endif // MYTABLEMODEL_H
