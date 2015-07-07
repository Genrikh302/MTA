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



QStringList MainWindow::nationalPrefix;
QStringList MainWindow::internationalPrefix;

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


    QCDRSortFilterModel *sortFilterModel = new QCDRSortFilterModel();
    sortFilterModel->setSourceModel(cdrModel);


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


    //ui->tableView->setModel(cdrModel);
    ui->tableView->setModel(sortFilterModel);

    cdrModel->setHeaderData(QCDRSortFilterModel::COL_IN_TYPE, Qt::Horizontal, "Аб. А\nВх. канал ");
    cdrModel->setHeaderData(QCDRSortFilterModel::COL_IN_MN, Qt::Horizontal, "Вх. модуль");
    cdrModel->setHeaderData(QCDRSortFilterModel::COL_IN_SLPCM, Qt::Horizontal, "Вх. поток");
    cdrModel->setHeaderData(QCDRSortFilterModel::COL_IN_PORT, Qt::Horizontal, "Вх. канал");
    cdrModel->setHeaderData(QCDRSortFilterModel::COL_IN_AON, Qt::Horizontal, "Вх. АОН");
    cdrModel->setHeaderData(QCDRSortFilterModel::COL_IN_NUMBER, Qt::Horizontal, "Вх. Номер");


    cdrModel->setHeaderData(QCDRSortFilterModel::COL_OUT_TYPE, Qt::Horizontal, "Аб. Б\nИсх. канал ");
    cdrModel->setHeaderData(QCDRSortFilterModel::COL_OUT_MN, Qt::Horizontal, "Исх. модуль");
    cdrModel->setHeaderData(QCDRSortFilterModel::COL_OUT_SLPCM, Qt::Horizontal, "Исх. поток");
    cdrModel->setHeaderData(QCDRSortFilterModel::COL_OUT_PORT, Qt::Horizontal, "Исх. канал");
    cdrModel->setHeaderData(QCDRSortFilterModel::COL_OUT_AON, Qt::Horizontal, "Исх. АОН");
    cdrModel->setHeaderData(QCDRSortFilterModel::COL_OUT_NUMBER, Qt::Horizontal, "Исх. Номер");

    cdrModel->setHeaderData(QCDRSortFilterModel::COL_DATE, Qt::Horizontal, "Дата");
    cdrModel->setHeaderData(QCDRSortFilterModel::COL_TIME, Qt::Horizontal, "Время");

    cdrModel->setHeaderData(QCDRSortFilterModel::COL_CALL_TYPE, Qt::Horizontal, "Тип");
    cdrModel->setHeaderData(QCDRSortFilterModel::COL_TIME_SEIZ, Qt::Horizontal, "Дл. занятия");
    cdrModel->setHeaderData(QCDRSortFilterModel::COL_TIME_TALK, Qt::Horizontal, "Дл. разговора");
    cdrModel->setHeaderData(QCDRSortFilterModel::COL_CRELEASE, Qt::Horizontal, "Причина \nзавершения");


    // скрываем лишние столбцы
    ui->tableView->setColumnHidden(QCDRSortFilterModel::COL_IN_MN, true); // inc1
    ui->tableView->setColumnHidden(QCDRSortFilterModel::COL_IN_SLPCM, true); // inc2
    ui->tableView->setColumnHidden(QCDRSortFilterModel::COL_IN_PORT, true); // inc3
    ui->tableView->setColumnHidden(QCDRSortFilterModel::COL_OUT_MN, true); // outinc1
    ui->tableView->setColumnHidden(QCDRSortFilterModel::COL_OUT_SLPCM, true); // outinc2
    ui->tableView->setColumnHidden(QCDRSortFilterModel::COL_OUT_PORT, true); // outinc3
    ui->tableView->setColumnHidden(QCDRSortFilterModel::COL_ID, true); // id


    // высоту ячейки
    ui->tableView->verticalHeader()->setDefaultSectionSize(18);
//    ui->tableView->setColumnWidth(QCDRTableModel::COL_IN_TYPE, 150);
//    ui->tableView->setColumnWidth(QCDRTableModel::COL_OUT_TYPE, 150);
//    ui->tableView->setColumnWidth(QCDRTableModel::COL_CRELEASE, 150);

    fillPrefixList();
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

    fillPrefixList();

    applyFilter();
}

void MainWindow::applyFilter()
{
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


    // лямбда для фмльтра по нумерам
    auto appendLikeFilter = [] (QString &f, const QString &filterStr, const QString &paramName) {
        if (filterStr.isEmpty())
            return;

        if (!f.isEmpty())
            f.append(" and ");

        QString filter = filterStr;
        filter.replace('?', QString("_"));
        filter.replace('*', QString("%"));
        f.append(QString(" %1 like '%2'").arg(paramName).arg(filter));
    };


    // лямда функция для фильтра по префиксам
    auto appendPrefixFilter = [] (const QSqlTableModel *m, bool negation = false) -> QString {
        QString likePrefix;
        for (int i = 0; i < m->rowCount(); i++)
            likePrefix.append(QString("%1innum like '%2%'").arg(likePrefix.isEmpty() ? "" : " or ").arg(m->data(m->index(i, 0)).toString()));

        if (!likePrefix.isEmpty())
            likePrefix = QString("and %1 (%2)").arg(negation ? "not" : "").arg(likePrefix);
        return likePrefix;
    };

    appendLikeFilter(filter, propertyFilter.inaonf(), "inanum");
    appendLikeFilter(filter, propertyFilter.innumf(), "innum");


    // филльтр по признаку завершения
    if (propertyFilter.releaseCause()) {
        if (!filter.isEmpty())
            filter.append(" and ");
        filter.append(QString(" relreason = %1").arg(propertyFilter.releaseCause()));
    }

    // фильтр по типу вызова
    if (propertyFilter.typeCalls()) {

        if (!filter.isEmpty())
            filter.append(" and ");
        switch (propertyFilter.typeCalls()) {
            case Qcallog::TYPE_LOCAL: filter.append("intype = 'A' and outtype = 'A'"); break;
            case Qcallog::TYPE_IN_LOCAL: filter.append("intype = 'C' and outtype = 'A'"); break;
            case Qcallog::TYPE_OUT_NATIONAL: filter.append(QString("intype = 'A' and outtype = 'C' %1").arg(appendPrefixFilter(nationalCode))); break;
            case Qcallog::TYPE_OUT_INTERNATIONAL: filter.append(QString("intype = 'A' and outtype = 'C' %1").arg(appendPrefixFilter(internationalCode))); break;
            case Qcallog::TYPE_OUT_LOCAL: filter.append(QString("intype = 'A' and outtype = 'C' %1").arg(appendPrefixFilter(nationalCode, true))); break;
            case Qcallog::TYPE_TRANZIT_NATIONAL: filter.append(QString("intype = 'C' and outtype = 'C' %1").arg(appendPrefixFilter(nationalCode))); break;
            case Qcallog::TYPE_TRANZIT_INTERNATIONAL: filter.append(QString("intype = 'C' and outtype = 'C' %1").arg(appendPrefixFilter(internationalCode))); break;
            case Qcallog::TYPE_TRANZIT_LOCAL: filter.append(QString("intype = 'C' and outtype = 'C' %1").arg(appendPrefixFilter(nationalCode, true))); break;
        }
    }

    cdrModel->setFilter(filter);
}

void MainWindow::on_pushButton_clicked()
{
    FilterDialog* Form = new FilterDialog(propertyFilter);

    if (Form->exec() == QDialog::Accepted)
    {

        Form->writefil(propertyFilter);
        applyFilter();
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

void MainWindow::fillPrefixList()
{
    nationalPrefix.clear();
    internationalPrefix.clear();

    nationalCode->select();

    for (int i = 0; i < nationalCode->rowCount(); i++)
        nationalPrefix << nationalCode->data(nationalCode->index(i, 0)).toString();

    internationalCode->select();

    for (int i = 0; i < internationalCode->rowCount(); i++)
        internationalPrefix << internationalCode->data(internationalCode->index(i, 0)).toString();

}
