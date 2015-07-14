#ifndef QCHANNELTABLEMODEL_H
#define QCHANNELTABLEMODEL_H

#include <QSqlTableModel>

class QChannelTableModel : public QSqlTableModel
{
    Q_OBJECT

public:
    QChannelTableModel(QObject * parent = 0, QSqlDatabase db = QSqlDatabase()) : QSqlTableModel(parent, db) {}
    virtual ~QChannelTableModel() {}
    virtual QVariant data(const QModelIndex & idx, int role = Qt::DisplayRole) const;
};

#endif // QCHANNELTABLEMODEL_H
