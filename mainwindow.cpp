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
#include "cdrtablemodel.h"
#include "filterdialog.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    if (!logdb.createConnection())
        qDebug() << "Problem creating db";

    if (!logdb.createTabel())
        qDebug() << "Unable to create a table";


    // сразу скормим файл с тектстовой инфой
    addCDRFileToDB(QApplication::instance()->applicationDirPath()+QString("/CDR/cdr_log_17_03_2015.log"));

    QCDRTableModel *model = new QCDRTableModel;
    model->setTable("logbase");
    model->setFilter("intype = 'C'"); //сделать через setQuery
    //model->setFilter("intype = 'A'");
    //model->setFilter("ininc1 < 111 ");
    model->select();
    ui->tableView->setModel(model);

    model->setHeaderData(0, Qt::Horizontal, "Канал");
    model->setHeaderData(1, Qt::Horizontal, "Модуль");
    model->setHeaderData(2, Qt::Horizontal, "Поток");
    model->setHeaderData(3, Qt::Horizontal, "Канал");
    model->setHeaderData(4, Qt::Horizontal, "Номер");


    // скрываем лишние столбцы
    ui->tableView->setColumnHidden(1, true);
    ui->tableView->setColumnHidden(2, true);
    ui->tableView->setColumnHidden(3, true);



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addCDRFileToDB(const QString &file) {
    QFile inputFile(file);

    if(!inputFile.open(QIODevice::ReadOnly))
    {
        qDebug()<<"ERROR: Can't open file "<< file;
        return;
    }

    QTextStream in(&inputFile);
    std::list <Qcallog> l;
    //Занесение данных из файлов в лист, из листа в базу
    while (!in.atEnd())
    {
        //QString line = inputFile.readLine();
        Qcallog logstr;
        in >> logstr;
//        l.push_back(logstr);
        logdb << logstr;
    }

    for(auto i = l.begin(); i != l.end(); i++)
        i->print();

    inputFile.close();
}

void MainWindow::on_actionOpen_triggered()
{
    //Выбор нескольких файлов для обработки
    QStringList filesway = QFileDialog::getOpenFileNames(0, "Open Dialog", "", "*.log *.txt");
    if (filesway.empty())
        return;

    //Последовательная обработка файлов
    foreach (QString str, filesway)
        addCDRFileToDB(str);

    QCDRTableModel *model = static_cast<QCDRTableModel *>(ui->tableView->model());
    model->select();
    ui->tableView->reset();

}



void MainWindow::on_pushButton_clicked()
{
    QWidget* Form = new FilterDialog;
    Form->setAttribute(Qt::WA_DeleteOnClose, true);
    Form->show();

}
