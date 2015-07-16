#include "qfiltercombobox.h"

#include <QKeyEvent>

void QFilterComboBox::keyPressEvent(QKeyEvent * e)
{

    if (Qt::Key_Up == e->key())
        return;

    if (Qt::Key_Down == e->key())
        return;

    QComboBox::keyPressEvent(e);
}
