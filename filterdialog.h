#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

#include <QDialog>

namespace Ui {
class FilterDialog;
}

class FilterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FilterDialog(QWidget *parent = 0);
    //void getfil(void); метод для занесения текста из окна фильтра в местные фильтры
    void writefil(QString &_abinf, QString &_aboutf, QString &_datesincef, QString &_datetof, QString &_timefromf, QString &_timetof, QString &_busylenfromf, QString &_busylentof, QString &_talklenfromf,  QString &_talklentof, QString &_inaonf, QString &_innumf, QString &_outaonf, QString &_outnumf);
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
