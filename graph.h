#ifndef GRAPH_H
#define GRAPH_H

#include <QDialog>
#include <QSqlTableModel>

namespace Ui {
class Graph;
}

class Graph : public QDialog
{
    Q_OBJECT

public:
    explicit Graph(QWidget *parent = 0);
    void buildReportReleaseCause(QSqlTableModel *cdrModel);
    void buildReportSucessCalls(QSqlTableModel *cdrModel, QDate date);
    ~Graph();

private:
    Ui::Graph *ui;
    //int relindex;
    //int columnum;
};

#endif // GRAPH_H
