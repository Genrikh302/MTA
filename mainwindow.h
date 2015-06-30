#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QtSql>
#include <QDateTime>
#include "qlogdb.h"
#include "qcallog.h"
#include "cdrtablemodel.h"
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
    void on_actionOpen_triggered();  // ипорт из файла
    void on_actionExit_triggered();  // выход из программы
    void on_actionAbout_triggered(); // открытие окна о программе
    void on_actionProgramProperty_triggered(); // открытие настроек программы

    void on_pushButton_clicked();


    void on_pushDelete_clicked();

private:
    Ui::MainWindow *ui;
    Qlogdb logdb;    

    PropertyFilter propertyFilter;

    QCDRTableModel *cdrModel;
    QSqlTableModel *nationalCode;
    QSqlTableModel *internationalCode;
};


#endif // MAINWINDOW_H
