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
    ~MainWindow();

//Здесь были private slots, не работало
private slots:
    void on_actionOpen_triggered();


private:
    Ui::MainWindow *ui;
    Qlogdb logdb; //раньше была private
};


#endif // MAINWINDOW_H
