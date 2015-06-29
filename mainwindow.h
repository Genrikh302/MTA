#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QtSql>
#include <QDateTime>
#include "qlogdb.h"
#include "qcallog.h"
#include "filterdialog.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    //QSqlTableModel *model;
    ~MainWindow();
    void addCDRFileToDB(const QString &file); // добавляет CDR file в базу
    void getfilters(FilterDialog fildial); // получает фильтры из диалогового окна

private slots:
    void on_actionOpen_triggered();

    void on_pushButton_clicked();


private:
    Ui::MainWindow *ui;
    Qlogdb logdb;

    PropertyFilter propertyFilter;
//    QString abinf;
//    QString aboutf;
//    QString datesincef;
//    QString datetof;
//    QString timesincef;
//    QString timetof;
//    QString busylenfromf;
//    QString busylentof;
//    QString talklenfromf;
//    QString talklentof;
//    QString inaonf;
//    QString innumf;
//    QString outaonf;
//    QString outnumf;
};


#endif // MAINWINDOW_H
