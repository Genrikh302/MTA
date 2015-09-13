#ifndef QFILTERCOMBOBOX_H
#define QFILTERCOMBOBOX_H

#include <QComboBox>

class QFilterComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit QFilterComboBox(QWidget *parent = 0) : QComboBox(parent) {}
    virtual void keyPressEvent(QKeyEvent * e);
};

class QTypeCallsComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit QTypeCallsComboBox(QWidget *parent = 0) : QComboBox(parent) {}
    virtual void hidePopup();
    virtual void showPopup();
};

#endif // QFILTERCOMBOBOX_H
