#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QtSql>
#include <QDateTime>
#include "qlogdb.h"
#include "qcallog.h"
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

private slots:
    void on_actionOpen_triggered();

    void on_pushButton_clicked();



private:
    Ui::MainWindow *ui;
    Qlogdb logdb;
};


#endif // MAINWINDOW_H
