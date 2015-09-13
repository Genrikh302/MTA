#include "qfiltercombobox.h"

#include <QKeyEvent>
#include <QStandardItemModel>

void QFilterComboBox::keyPressEvent(QKeyEvent * e)
{

    if (Qt::Key_Up == e->key())
        return;

    if (Qt::Key_Down == e->key())
        return;

    QComboBox::keyPressEvent(e);
}


void QTypeCallsComboBox::hidePopup()
{
//    qDebug() << "QTypeCallsComboBox::hidePopup";
    QStandardItemModel *m = qobject_cast<QStandardItemModel *>(this->model());

    QStringList txt;
    if (!m)
        return;

    for (int i = 0; i < m->rowCount(); i++)
        if (m->item(i)->checkState() == Qt::Checked)
            txt.push_back(m->item(i)->text());

    m->item(0)->setData(txt.join(','), Qt::DisplayRole);
    QComboBox::hidePopup();
}

void QTypeCallsComboBox::showPopup()
{
//    qDebug() << "QTypeCallsComboBox::showPopup";

    QStandardItemModel *m = qobject_cast<QStandardItemModel *>(this->model());
    if (m)
        m->item(0)->setData("не задана", Qt::DisplayRole);

    QComboBox::showPopup();
}

