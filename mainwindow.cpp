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
    QString str = "CREATE TABLE logbase (id integer primary key autoincrement, "    //проверить if not exists
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
   //query.prepare("INSERT INTO  (resptype, resp1, resp2, resp3, respnum, resplen, inctype, inc1, inc2, inc3, incnum, inclen) VALUES (:resptype, :resp1, :resp2, :resp3, :respnum, :resplen, :inctype, :inc1, :inc2, :inc3, :incnum, :inclen)");

    //base->show();
    //setCentralWidget(base);

    //Последовательная обработка файлов
    for(int i = 0; i < /*filesway.size()*/1; i++)
    {
        //QFile inputFile(filesway.at(i));
        QFile inputFile("/Users/genrikhmayorov/Desktop/CDR/cdr_log_17_03_2015.log");
        if(!inputFile.open(QIODevice::ReadOnly))
        {
            //qDebug()<<"ERROR: Can't open file "<<filesway.at(i); !!!!!Вернуть на место
            //continue;
        }
        QTextStream in(&inputFile);
        std::list <Qcallog> l;
        //Построчное чтение из файла
        while (!in.atEnd())
        {
                //QString line = inputFile.readLine();
                Qcallog logstr;
                in >> logstr;
                l.push_back(logstr);
                logdb << logstr;
                //logstr.print();
        }
        for(auto i = l.begin(); i != l.end(); i++){
            i->print();
        }
        inputFile.close();
        }
    QTableView *base = ui->tableView;
    QSqlTableModel *model = new QSqlTableModel;
    model->setTable("logbase");
    model->select();
    base->setModel(model);
}

//void MainWindow::on_FindButton_clicked()
//{
//    //QString searchString = ui->SearchWord->text();
//    //ui->textEdit->find(searchString, QTextDocument::FindWholeWords);
//}

