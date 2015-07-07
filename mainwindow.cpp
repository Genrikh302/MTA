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
#include <QMessageBox>

#include "qprogrampropertydialog.h"

#define VERSION "1.0.0b"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    if (!logdb.createConnection())
        qDebug() << "Problem creating db";

    if (!logdb.createTabelCDR())
        qDebug() << "Unable to create a cdr table";

    if (!logdb.createTabelNationalCode())
        qDebug() << "Unable to create a national code table";

    if (!logdb.createTabelInternationalCode())
        qDebug() << "Unable to create a international code table";


    // сразу скормим файл с тектстовой инфой



    cdrModel = new QCDRTableModel(this, logdb.getDB());
    cdrModel->setTable("logbase");
    //cdrModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    cdrModel->select();

    addCDRFileToDB(QApplication::instance()->applicationDirPath()+QString("/CDR/cdr_log_17_03_2015.log"));
    //cdrModel->submitAll();
    cdrModel->select();


    nationalCode = new QSqlTableModel(this, logdb.getDB());
    nationalCode->setTable("NationalCode");
    //nationalCode->setEditStrategy(QSqlTableModel::OnManualSubmit);
    nationalCode->select();
    int row = nationalCode->rowCount();
    if (!row) {
        if (!nationalCode->insertRow(row))
            qDebug() << "insertRow nationalCode" << nationalCode->lastError().text();
        nationalCode->setData(nationalCode->index(row, 0), "8");
        if (!nationalCode->submitAll())
            qDebug() << "submit nationalCode" << nationalCode->lastError().text();
    }
//    nationalCode->setEditStrategy(QSqlTableModel::OnFieldChange);

    internationalCode = new QSqlTableModel(this, logdb.getDB());
    internationalCode->setTable("InternationalCode");
    internationalCode->setEditStrategy(QSqlTableModel::OnManualSubmit);
    internationalCode->select();
    row = internationalCode->rowCount();
    if (!row) {
        if (!internationalCode->insertRow(row))
            qDebug() << "insertRow internationalCode" << internationalCode->lastError().text();
        internationalCode->setData(nationalCode->index(row, 0), "810");
        if (!internationalCode->submitAll())
            qDebug() << "submit internationalCode" << internationalCode->lastError().text();
    }


    ui->tableView->setModel(cdrModel);

    cdrModel->setHeaderData(QCDRTableModel::COL_IN_TYPE, Qt::Horizontal, "Аб. А\nВх. канал ");
    cdrModel->setHeaderData(QCDRTableModel::COL_IN_MN, Qt::Horizontal, "Вх. модуль");
    cdrModel->setHeaderData(QCDRTableModel::COL_IN_SLPCM, Qt::Horizontal, "Вх. поток");
    cdrModel->setHeaderData(QCDRTableModel::COL_IN_PORT, Qt::Horizontal, "Вх. канал");
    cdrModel->setHeaderData(QCDRTableModel::COL_IN_AON, Qt::Horizontal, "Вх. АОН");
    cdrModel->setHeaderData(QCDRTableModel::COL_IN_NUMBER, Qt::Horizontal, "Вх. Номер");


    cdrModel->setHeaderData(QCDRTableModel::COL_OUT_TYPE, Qt::Horizontal, "Аб. Б\nИсх. канал ");
    cdrModel->setHeaderData(QCDRTableModel::COL_OUT_MN, Qt::Horizontal, "Исх. модуль");
    cdrModel->setHeaderData(QCDRTableModel::COL_OUT_SLPCM, Qt::Horizontal, "Исх. поток");
    cdrModel->setHeaderData(QCDRTableModel::COL_OUT_PORT, Qt::Horizontal, "Исх. канал");
    cdrModel->setHeaderData(QCDRTableModel::COL_OUT_AON, Qt::Horizontal, "Исх. АОН");
    cdrModel->setHeaderData(QCDRTableModel::COL_OUT_NUMBER, Qt::Horizontal, "Исх. Номер");

    cdrModel->setHeaderData(QCDRTableModel::COL_DATE, Qt::Horizontal, "Дата");
    cdrModel->setHeaderData(QCDRTableModel::COL_TIME, Qt::Horizontal, "Время");

    cdrModel->setHeaderData(QCDRTableModel::COL_CALL_TYPE, Qt::Horizontal, "Тип");
    cdrModel->setHeaderData(QCDRTableModel::COL_TIME_SEIZ, Qt::Horizontal, "Дл. занятия");
    cdrModel->setHeaderData(QCDRTableModel::COL_TIME_TALK, Qt::Horizontal, "Дл. разговора");
    cdrModel->setHeaderData(QCDRTableModel::COL_CRELEASE, Qt::Horizontal, "Причина \nзавершения");


    // скрываем лишние столбцы
    ui->tableView->setColumnHidden(QCDRTableModel::COL_IN_MN, true); // inc1
    ui->tableView->setColumnHidden(QCDRTableModel::COL_IN_SLPCM, true); // inc2
    ui->tableView->setColumnHidden(QCDRTableModel::COL_IN_PORT, true); // inc3
    ui->tableView->setColumnHidden(QCDRTableModel::COL_OUT_MN, true); // outinc1
    ui->tableView->setColumnHidden(QCDRTableModel::COL_OUT_SLPCM, true); // outinc2
    ui->tableView->setColumnHidden(QCDRTableModel::COL_OUT_PORT, true); // outinc3
    ui->tableView->setColumnHidden(QCDRTableModel::COL_ID, true); // id


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
//    std::list <Qcallog> l;
    //Занесение данных из файлов в лист, из листа в базу
    QSqlDatabase::database().transaction();
    while (!in.atEnd())
    {
        //QString line = inputFile.readLine();
        Qcallog logstr;
        in >> logstr;
        //l.push_back(logstr);
        logdb << logstr;
        //(*cdrModel) << logstr;
    }
    QSqlDatabase::database().commit();
//    for(auto i = l.begin(); i != l.end(); i++)
//        i->print();

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

//    QCDRTableModel *model = static_cast<QCDRTableModel *>(ui->tableView->model());
//    cdrModel->submitAll();

    cdrModel->select();
    ui->tableView->reset();
}


// выход из программы
void MainWindow::on_actionExit_triggered()
{
    close();
}

// открытие окна 'О программе'
void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, tr("О программе"),
            "<center><b>" + tr("Отчет 2015") + " " + QString(VERSION) + "</b><br>" \
             + tr("основан на Qt") + " " + qVersion() + "<br>"
             "</center>"
             "<center><a href=\"www.m-200.com\">www.m-200.com</a></center>"
             "<center><a href=\"mailto:service@m-200.com\">Техническая поддержка</a></center>");
}

// открытие окна настрое программы
void MainWindow::on_actionProgramProperty_triggered()
{
    QProgramPropertyDialog propertyDialog(nationalCode, internationalCode);
    propertyDialog.exec();
}

void MainWindow::on_pushButton_clicked()
{
    FilterDialog* Form = new FilterDialog(propertyFilter);

    if (Form->exec() == QDialog::Accepted)
    {
//        PropertyFilter p;
        //Form->writefil(abinf, aboutf, datesincef, datetof, timesincef, timetof, busylenfromf, busylentof, talklenfromf, talklentof, inaonf, innumf, outaonf, outnumf);
        Form->writefil(propertyFilter);
        //qDebug() << abinf << aboutf << datesincef << timetof << inaonf;

        //QCDRTableModel *model = static_cast<QCDRTableModel *>(ui->tableView->model());

        QString filter;
        if (!propertyFilter.abinf().isEmpty()) {
            QString abinf = propertyFilter.abinf();
            QTextStream str(&abinf);
            Qcallog::s log;
            str >> log;

            // фильтр по входящему каналу
            const char intype = log.getType();
            if (intype == 'A')
                filter.append(QString(" intype = '%1' and ininc1 = %2").arg(intype).arg(log.getInc1()));

            if (intype == 'C') {
                filter = QString(" intype = '%1'").arg(intype);
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

        if (!propertyFilter.aboutf().isEmpty()) {
            QString aboutf = propertyFilter.aboutf();
            QTextStream str(&aboutf);
            Qcallog::s log;
            str >> log;

            // фильтр по исходящему каналу
            const char outtype = log.getType();
            if (outtype == 'A')
                filter.append(QString(" outtype = '%1' and outinc1 = %2").arg(outtype).arg(log.getInc1()));

            if (outtype == 'C') {
                if (!filter.isEmpty())
                    filter.append(" and ");
                filter.append(QString("outtype = '%1'").arg(outtype));
                if (log.getInc1() != "*") {// все модули
                    filter.append(QString(" and outinc1 = %1").arg(log.getInc1()));
                    if (log.getInc2() != "*") { // все потоки
                        filter.append(QString(" and outinc2 = %1").arg(log.getInc2()));
                        if (log.getInc3() != "*") // все каналы
                            filter.append(QString(" and outinc3 = %1").arg(log.getInc3()));
                    }
                }
            }
        }
        if (!propertyFilter.outaonf().isEmpty()) {
            QString outaonf = propertyFilter.outaonf();
            if (!filter.isEmpty())
                filter.append(" and ");

            // заменяем из человеческого формама в SqlLiteовский
            outaonf.replace('?', QString("_"));
            outaonf.replace('*', QString("%"));
            filter.append(QString(" outanum like '%1'").arg(outaonf));
        }

        if (!propertyFilter.outnumf().isEmpty()) {
            QString outnumf = propertyFilter.outnumf();
            if (!filter.isEmpty())
                filter.append(" and ");
            // заменяем из человеческого формама в SqlLiteовский
            outnumf.replace('?', QString("_"));
            outnumf.replace('*', QString("%"));
            filter.append(QString(" outnum like '%1'").arg(outnumf));
        }
        if ( !propertyFilter.busylenfromf().isEmpty()) {
            if (!filter.isEmpty())
                filter.append(" and ");
            filter.append(QString(" linelen >= %1").arg(propertyFilter.busylenfromf()));
        }
        if ( !propertyFilter.busylentof().isEmpty()) {
            if (!filter.isEmpty())
                filter.append(" and ");
            filter.append(QString(" linelen <= %1").arg(propertyFilter.busylentof()));
        }
        if ( !propertyFilter.talklenfromf().isEmpty()) {
            if (!filter.isEmpty())
                filter.append(" and ");
            filter.append(QString(" callen >= %1").arg(propertyFilter.talklenfromf()));
        }
        if ( !propertyFilter.talklentof().isEmpty()) {
            if (!filter.isEmpty())
                filter.append(" and ");
            filter.append(QString(" callen <= %1").arg(propertyFilter.talklentof()));
        }
        //!!!!!
        if ( !propertyFilter.timefromf().isEmpty()) {
            //if (!filter.isEmpty())
                //filter.append(" and ");
            qDebug() << propertyFilter.timefromf() << propertyFilter.datesincef();
        }
        //!!!!!!
        if (propertyFilter.releaseCause()) {
            if (!filter.isEmpty())
                filter.append(" and ");
            filter.append(QString(" relreason = %1").arg(propertyFilter.releaseCause()));

        }

        cdrModel->setFilter(filter);
        qDebug() << filter;
    }
}

void MainWindow::on_pushDelete_clicked()
{
    QItemSelectionModel *selModel = ui->tableView->selectionModel();

    if (!selModel)
        return;

    QModelIndexList indexes = selModel->selectedRows();

    if (indexes.isEmpty())
        return;

    QSqlQuery query;

    QSqlDatabase::database().transaction();

    int idIndex = cdrModel->fieldIndex("id");
    foreach (auto i, indexes) {

        int v = cdrModel->data(cdrModel->index(i.row(), idIndex)).toInt();

        if (!query.exec(QString("DELETE FROM logbase WHERE id = %1").arg(v)))
            qDebug() << "Unable to delete value" << logdb.lastError();
    }
    QSqlDatabase::database().commit();


    cdrModel->select();
    ui->tableView->reset();
}
