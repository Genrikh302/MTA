#ifndef MYTABLEMODEL_H
#define MYTABLEMODEL_H

#include <QObject>
#include <QSqlTableModel>

class MyTableModel : public QSqlTableModel
{
public:
    MyTableModel();
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    ~MyTableModel();
};

#endif // MYTABLEMODEL_H
