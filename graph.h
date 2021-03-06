#ifndef GRAPH_H
#define GRAPH_H

#include <QDialog>
#include <QSqlTableModel>
#include <QAction>

namespace Ui {
class Graph;
}

class Graph : public QDialog
{
    Q_OBJECT

public:
    explicit Graph(QWidget *parent = 0);
    void buildReportReleaseCause(QSqlTableModel *cdrModel);
    void buildReportSucessCallsDate(QSqlTableModel *cdrModel);
    void buildReportSucessCallsTime(QSqlTableModel *cdrModel);
    void buildReportSucessCallsWeekDay(QSqlTableModel *cdrModel);
    void buildReportAbonents(QSqlTableModel *cdrModel);
    void buildReportLoad(QSqlTableModel *cdrModel);
    void buildReportCallens(QSqlTableModel *cdrModel);
    ~Graph();

private slots:
    void save(void);

private:
    Ui::Graph *ui;
    //int relindex;
    //int columnum;
};

#endif // GRAPH_H
