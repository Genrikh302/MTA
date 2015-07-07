#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

#include <QDialog>

namespace Ui {
class FilterDialog;
}

class PropertyFilter : public QObject {
    Q_OBJECT

private:
    QString m_abinf; // ????
    QString m_aboutf;
    QString m_datesincef;
    QString m_datetof;
    QString m_timefromf;
    QString m_timetof;
    QString m_busylenfromf;
    QString m_busylentof;
    QString m_talklenfromf;
    QString m_talklentof;
    QString m_inaonf;
    QString m_innumf;
    QString m_outaonf;
    QString m_outnumf;

    qint16  m_releaseCause;

public:

    PropertyFilter() : QObject(), m_releaseCause(0) {}
    QString aboutf() const ;
    void setAboutf(const QString &aboutf);
    QString abinf() const;
    void setAbinf(const QString &abinf);
    QString datesincef() const;
    void setDatesincef(const QString &datesincef);
    QString datetof() const;
    void setDatetof(const QString &datetof);
    QString timefromf() const;
    void setTimefromf(const QString &timefromf);
    QString timetof() const;
    void setTimetof(const QString &timetof);
    QString busylenfromf() const;
    void setBusylenfromf(const QString &busylenfromf);
    QString busylentof() const;
    void setBusylentof(const QString &busylentof);
    QString talklenfromf() const;
    void setTalklenfromf(const QString &talklenfromf);
    QString talklentof() const;
    void setTalklentof(const QString &talklentof);
    QString inaonf() const;
    void setInaonf(const QString &inaonf);
    QString innumf() const;
    void setInnumf(const QString &innumf);
    QString outaonf() const;
    void setOutaonf(const QString &outaonf);
    QString outnumf() const;
    void setOutnumf(const QString &outnumf);
    qint16 releaseCause() const;
    void setReleaseCause(const qint16 &releaseCause);
};

class FilterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FilterDialog(const PropertyFilter &propertyFilter, QWidget *parent = 0);
    //void getfil(void); метод для занесения текста из окна фильтра в местные фильтры
    void writefil(PropertyFilter &f);
    //занесение текста из полей для фильтров в указаные стринги
    ~FilterDialog();

private slots:

    void on_clearbutton_clicked();

private:
    Ui::FilterDialog *ui;
    /*QString abinf; ненужные фильтры
    QString aboutf;
    QString datesincef;
    QString datetof;
    QString timesincef;
    QString timetof;
    QString busylenfromf;
    QString busylentof;
    QString talklenfromf;
    QString talklentof;
    QString inaonf;
    QString innumf;
    QString outaonf;
    QString outnumf;*/
};

#endif // FILTERDIALOG_H
