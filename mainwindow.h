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
    void getfilters(FilterDialog &filter);

private slots:
    void on_actionOpen_triggered();

    void on_pushButton_clicked();


private:
    Ui::MainWindow *ui;
    Qlogdb logdb;
    QString ab1filter;
};


#endif // MAINWINDOW_H
