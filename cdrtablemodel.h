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
};

#endif // MYTABLEMODEL_H
