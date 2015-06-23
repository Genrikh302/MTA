#include "mainwindow.h"
#include "qlogdb.h"
#include "qcallog.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QtDebug>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QModelIndex>
#include <QTableView>
#include <QTableWidgetItem>
#include "mytablemodel.h"
#include "filterdialog.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    //Выбор нескольких файлов для обработки
    QStringList filesway = QFileDialog::getOpenFileNames(0, "Open Dialog", "", "*.log *.txt");
    //qDebug()<<filesway;
    if(!logdb.createConnection()){
        qDebug()<<"Problem creating db";
    }
    QSqlQuery query;
    QString str = "CREATE TABLE logbase ( "    //проверить if not exists
                  "intype VARCHAR(1),"
                  "ininc1 int, "
                  "ininc2 int, "
                  "ininc3 int, "
                  "innum VARCHAR(28), "
                  "inanum int, "
                  "outtype VARCHAR(1), "
                  "outinc1 int, "
                  "outinc2 int, "
                  "outinc3 int, "
                  "outnum VARCHAR(28), "
                  "outanum int, "
                  "date int, "
                  "linelen int, "
                  "callen int , "
                  "relreason int)" ;
    if (!query.exec(str)) {
        qDebug() << "Unable to create a table";
    }
    //Последовательная обработка файлов
    for(int i = 0; i < /*filesway.size()*/1; i++)
    {
        //QFile inputFile(filesway.at(i));
        QFile inputFile("/Users/genrikhmayorov/Desktop/CDR/cdr_log_17_03_2015.log");
        if(!inputFile.open(QIODevice::ReadOnly))
        {
            qDebug()<<"ERROR: Can't open file "<<filesway.at(i);
            continue;
        }
        QTextStream in(&inputFile);
        std::list <Qcallog> l;
        //Занесение данных из файлов в лист, из листа в базу
        while (!in.atEnd())
        {
                //QString line = inputFile.readLine();
                Qcallog logstr;
                in >> logstr;
                l.push_back(logstr);
                logdb << logstr;
        }
        for(auto i = l.begin(); i != l.end(); i++){
            i->print();
        }
        inputFile.close();
        }
    MyTableModel *model = new MyTableModel;
    model->setTable("logbase");
    model->setFilter("intype = 'C'"); //сделать через setQuery
    //model->setFilter("intype = 'A'");
    //model->setFilter("ininc1 < 111 ");
    model->select();
    ui->tableView->setModel(model);
//    QTableView *base = ui->tableView;
//    QSqlTableModel *model = new QSqlTableModel;
//    model->setTable("logbase");
//    model->select();
//    base->setModel(model);
}



void MainWindow::on_pushButton_clicked()
{
    QWidget* Form = new FilterDialog;
    Form->setAttribute(Qt::WA_DeleteOnClose, true);
    Form->show();

}
