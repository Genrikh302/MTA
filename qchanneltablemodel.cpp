#include "qchanneltablemodel.h"

QVariant QChannelTableModel::data(const QModelIndex & idx, int role) const
{
    if (!idx.isValid())
        return QVariant();

    if (role == Qt::DisplayRole) {

        qint64 v = QSqlTableModel::data(idx, role).toLongLong();

        return QString("C%1%2%3").arg((v >> 32) & 0xffff, 3, 10, QLatin1Char('0')).arg((v >> 16) & 0xffff, 3, 10, QLatin1Char('0')).arg(v & 0xffff, 3, 10, QLatin1Char('0'));
    }
    return QSqlTableModel::data(idx, role);
}
