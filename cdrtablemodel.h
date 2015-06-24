#ifndef MYTABLEMODEL_H
#define MYTABLEMODEL_H

#include <QObject>
#include <QSqlTableModel>



class QCDRTableModel : public QSqlTableModel
{
public:
    QCDRTableModel();
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    ~QCDRTableModel();

    const unsigned char COL_TYPE_IN    = 0;
    const unsigned char COL_TYPE_OUT   = 6;
    const unsigned char COL_MODULE_OUT = 7;
    const unsigned char COL_PCM_OUT    = 8;
    const unsigned char COL_PORT_OUT   = 9;
    const unsigned char COL_NUMBER_OUT = 10;
    const unsigned char COL_AON_OUT    = 11;
    const unsigned char COL_DATE_OUT   = 12;
};

#endif // MYTABLEMODEL_H
