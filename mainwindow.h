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
#include "qprogressdialog.h"
#include "progressworker.h"


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
    //void addCDRFileToDB(const QString &file, int fileid, QProgressDialog *progress, int size); // добавляет CDR file в базу
    void getfilters(FilterDialog fildial); // получает фильтры из диалогового окна

    static const QStringList & getNationalPrefix() {return nationalPrefix;}
    static const QStringList & getInternationalPrefix() {return internationalPrefix;}
signals:
    void updatebar(int value);
    void updatefilenum();

private slots:
    void on_actionOpen_triggered();  // импорт из файла
    void on_action_SSH_import_triggered();  // импорт по SSH
    void on_actionExit_triggered();  // выход из программы
    void on_actionAbout_triggered(); // открытие окна о программе
    void on_actionProgramProperty_triggered(); // открытие настроек программы
    void on_actionReleaseCause_triggered(); //
    void on_actionSucessCallsDate_triggered(); //
    void on_actionSucessCallsTime_triggered(); //
    void on_actionSucessCallsWeekDay_triggered();
    void on_actionAbonents_triggered();
    void on_actionLoad_triggered();
    void on_actionDrawCallens_triggered();

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

    QProgressDialog *progressDialog;
    QThread* load;
    ProgressWorker *worker;

public slots:
//    void slot_table_clicked(int index);
    void on_worker_finish();
    void FileLoadDialog_closed();
//    void on_fileProgress(const int value);
//    void on_listProgress(const int value);
};


#endif // MAINWINDOW_H
