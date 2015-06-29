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
    //model->setFilter("intype = 'C'"); //сделать через setQuery
    //model->setFilter("intype = 'A'");
    //model->setFilter("ininc1 < 111 ");
    model->select();
    ui->tableView->setModel(model);

    model->setHeaderData(QCDRTableModel::COL_IN_TYPE, Qt::Horizontal, "Аб. А\nВх. канал ");
    model->setHeaderData(QCDRTableModel::COL_IN_MN, Qt::Horizontal, "Вх. модуль");
    model->setHeaderData(QCDRTableModel::COL_IN_SLPCM, Qt::Horizontal, "Вх. поток");
    model->setHeaderData(QCDRTableModel::COL_IN_PORT, Qt::Horizontal, "Вх. канал");
    model->setHeaderData(QCDRTableModel::COL_IN_AON, Qt::Horizontal, "Вх. АОН");
    model->setHeaderData(QCDRTableModel::COL_IN_NUMBER, Qt::Horizontal, "Вх. Номер");


    model->setHeaderData(QCDRTableModel::COL_OUT_TYPE, Qt::Horizontal, "Аб. Б\nИсх. канал ");
    model->setHeaderData(QCDRTableModel::COL_OUT_MN, Qt::Horizontal, "Исх. модуль");
    model->setHeaderData(QCDRTableModel::COL_OUT_SLPCM, Qt::Horizontal, "Исх. поток");
    model->setHeaderData(QCDRTableModel::COL_OUT_PORT, Qt::Horizontal, "Исх. канал");
    model->setHeaderData(QCDRTableModel::COL_OUT_AON, Qt::Horizontal, "Исх. АОН");
    model->setHeaderData(QCDRTableModel::COL_OUT_NUMBER, Qt::Horizontal, "Исх. Номер");

    model->setHeaderData(QCDRTableModel::COL_DATE, Qt::Horizontal, "Дата");
    model->setHeaderData(QCDRTableModel::COL_TIME, Qt::Horizontal, "Время");

    model->setHeaderData(QCDRTableModel::COL_CALL_TYPE, Qt::Horizontal, "Тип");
    model->setHeaderData(QCDRTableModel::COL_TIME_SEIZ, Qt::Horizontal, "Дл. занятия");
    model->setHeaderData(QCDRTableModel::COL_TIME_TALK, Qt::Horizontal, "Дл. разговора");
    model->setHeaderData(QCDRTableModel::COL_CRELEASE, Qt::Horizontal, "Причина \nзавершения");


    // скрываем лишние столбцы
    ui->tableView->setColumnHidden(QCDRTableModel::COL_IN_MN, true); // inc1
    ui->tableView->setColumnHidden(QCDRTableModel::COL_IN_SLPCM, true); // inc2
    ui->tableView->setColumnHidden(QCDRTableModel::COL_IN_PORT, true); // inc3
    ui->tableView->setColumnHidden(QCDRTableModel::COL_OUT_MN, true); // outinc1
    ui->tableView->setColumnHidden(QCDRTableModel::COL_OUT_SLPCM, true); // outinc2
    ui->tableView->setColumnHidden(QCDRTableModel::COL_OUT_PORT, true); // outinc3


    // высоту ячейки
    ui->tableView->verticalHeader()->setDefaultSectionSize(18);
//    ui->tableView->setColumnWidth(QCDRTableModel::COL_IN_TYPE, 150);
//    ui->tableView->setColumnWidth(QCDRTableModel::COL_OUT_TYPE, 150);
//    ui->tableView->setColumnWidth(QCDRTableModel::COL_CRELEASE, 150);



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addCDRFileToDB(const QString &file) {
    QFile inputFile(file);

    statusBar()->showMessage(tr("Loaded %1").arg(file), 2000);

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
        //l.push_back(logstr);
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
    FilterDialog* Form = new FilterDialog(propertyFilter);

    if (Form->exec() == QDialog::Accepted)
    {
        qDebug() << "Ok";
//        PropertyFilter p;
        //Form->writefil(abinf, aboutf, datesincef, datetof, timesincef, timetof, busylenfromf, busylentof, talklenfromf, talklentof, inaonf, innumf, outaonf, outnumf);
        Form->writefil(propertyFilter);
        //qDebug() << abinf << aboutf << datesincef << timetof << inaonf;

        QCDRTableModel *model = static_cast<QCDRTableModel *>(ui->tableView->model());

        QString filter;
        if (!propertyFilter.abinf().isEmpty()) {
            QString abinf = propertyFilter.abinf();
            QTextStream str(&abinf);
            Qcallog::s log;
            str >> log;

            // фильтр по входящему каналу
            const char type = log.getType();
            if (type == 'A')
                filter.append(QString(" intype = '%1' and ininc1 = %2").arg(type).arg(log.getInc1()));

            if (type == 'C') {
                filter = QString(" intype = '%1'").arg(type);
                if (log.getInc1() != "*") {// все модули
                    filter.append(QString(" and ininc1 = %1").arg(log.getInc1()));
                    if (log.getInc2() != "*") { // все потоки
                        filter.append(QString(" and ininc2 = %1").arg(log.getInc2()));
                        if (log.getInc3() != "*") // все каналы
                            filter.append(QString(" and ininc3 = %1").arg(log.getInc3()));
                    }
                }
            }
        }

        if (!propertyFilter.inaonf().isEmpty()) {
            QString inaonf = propertyFilter.inaonf();
            if (!filter.isEmpty())
                filter.append(" and ");

            // заменяем из человеческого формама в SqlLiteовский
            inaonf.replace('?', QString("_"));
            inaonf.replace('*', QString("%"));
            filter.append(QString(" inanum like '%1'").arg(inaonf));
        }


        if (!propertyFilter.innumf().isEmpty()) {
            QString innumf = propertyFilter.innumf();
            if (!filter.isEmpty())
                filter.append(" and ");

            // заменяем из человеческого формама в SqlLiteовский
            innumf.replace('?', QString("_"));
            innumf.replace('*', QString("%"));
            filter.append(QString(" innum like '%1'").arg(innumf));
        }

        model->setFilter(filter);
    }

}


