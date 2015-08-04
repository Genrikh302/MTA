#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QtSql>
#include <QDateTime>
#include "qlogdb.h"
#include "qcallog.h"
#include "cdrtablemodel.h"
#include "filterdialog.h"
#include "qchanneltablemodel.h"
#include "graph.h"


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
    void addCDRFileToDB(const QString &file, int fileid); // добавляет CDR file в базу
    void getfilters(FilterDialog fildial); // получает фильтры из диалогового окна

    static const QStringList & getNationalPrefix() {return nationalPrefix;}
    static const QStringList & getInternationalPrefix() {return internationalPrefix;}
private slots:
    void on_actionOpen_triggered();  // ипорт из файла
    void on_actionExit_triggered();  // выход из программы
    void on_actionAbout_triggered(); // открытие окна о программе
    void on_actionProgramProperty_triggered(); // открытие настроек программы
    void on_actionReleaseCause_triggered(); //
    void on_actionSucessCallsDate_triggered(); //
    void on_actionSucessCallsTime_triggered(); //
    void on_actionSucessCallsWeekDay_triggered();

    void on_pushButton_clicked();

    void on_pushDelete_clicked();

    void on_pushSave_clicked();

    void on_pushDrawReleaseCause_clicked();

    void on_pushDrawSucessCallsDate_clicked();

    void on_pushDrawSucessCallsTime_clicked();

    void on_pushDrawSucessCallsWeekDay_clicked();

    void on_pushAbonents_clicked();

    void on_pushLoad_clicked();

    void on_pushDrawCallens_clicked();

private:
    Ui::MainWindow *ui;
    Qlogdb logdb;    

    PropertyFilter propertyFilter;

    QSqlTableModel *cdrModel;
    QSqlTableModel *nationalCode;
    QSqlTableModel *internationalCode;

    QSqlTableModel *directionName;
    QChannelTableModel *channelModel;

    static QStringList nationalPrefix;
    static QStringList internationalPrefix;

    void fillPrefixList(); // заполняет списки префиксов, для ускорения
    void applyFilter(); // применяет фильтр на таблицу
    void addFileListToCDRbase(const QStringList &files); // добавление файлов в базу
    void showRecordCount();
};


#endif // MAINWINDOW_H
