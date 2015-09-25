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
#include <QProgressBar>
#include "qprogrampropertydialog.h"
#include "qsshlogindialog.h"
#include "qsshselectdialog.h"
#include "qsshfile.h"

#include "common.h"


#include <libssh2.h>

#ifndef WIN32
#include <arpa/inet.h>
#endif

#ifdef WIN32
#include <winsock2.h>
#include <QTextCodec>
#include <QThread>
#endif

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>


#include "qprogressdialog.h"
#include "progressworker.h"

#define VERSION "1.0.0b"


QStringList MainWindow::nationalPrefix;
QStringList MainWindow::internationalPrefix;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

#ifdef Q_OS_WIN
    setWindowFlags(windowFlags() | Qt::CustomizeWindowHint);
#endif

    if (!logdb.createConnection())
        qDebug() << "Problem creating db";

    if (!logdb.createTabelCDR())
        qDebug() << "Unable to create a cdr table";

    if (!logdb.createTabelNationalCode())
        qDebug() << "Unable to create a national code table";

    if (!logdb.createTabelInternationalCode())
        qDebug() << "Unable to create a international code table";


    if (!logdb.createTabelDirectionName())
        qDebug() << "Unable to create a direction name table";

    if (!logdb.createTabelDirectionChannel())
        qDebug() << "Unable to create a direction channel table";

    if (!logdb.createTabelLoadedFile())
        qDebug() << "Unable to create a loaded file table";

    // сразу скормим файл с тектстовой инфой


    cdrModel = new QCDRTableModel(this, logdb.getDB());
    cdrModel->setTable("logbase");
    //cdrModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    cdrModel->select();


    QCDRSortFilterModel *sortFilterModel = new QCDRSortFilterModel();
    sortFilterModel->setSourceModel(cdrModel);


//#ifdef QT_DEBUG
//    if (!cdrModel->rowCount())
//        addFileListToCDRbase(QStringList() << QApplication::instance()->applicationDirPath()+QString("/CDR/cdr_log_17_03_2015.log") );
//    cdrModel->select();
//#endif


    nationalCode = new QSqlTableModel(this, logdb.getDB());
    nationalCode->setTable("NationalCode");
    //nationalCode->setEditStrategy(QSqlTableModel::OnManualSubmit);
    nationalCode->select();

#ifdef QT_DEBUG
    int row = nationalCode->rowCount();
    if (!row) {
        if (!nationalCode->insertRow(row))
            qDebug() << "insertRow nationalCode" << nationalCode->lastError().text();
        nationalCode->setData(nationalCode->index(row, 0), "8");
        if (!nationalCode->submitAll())
            qDebug() << "submit nationalCode" << nationalCode->lastError().text();
    }
#endif

    internationalCode = new QSqlTableModel(this, logdb.getDB());
    internationalCode->setTable("InternationalCode");
    internationalCode->setEditStrategy(QSqlTableModel::OnManualSubmit);
    internationalCode->select();
#ifdef QT_DEBUG
    row = internationalCode->rowCount();
    if (!row) {
        if (!internationalCode->insertRow(row))
            qDebug() << "insertRow internationalCode" << internationalCode->lastError().text();
        internationalCode->setData(nationalCode->index(row, 0), "810");
        if (!internationalCode->submitAll())
            qDebug() << "submit internationalCode" << internationalCode->lastError().text();
    }
#endif

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
    ui->tableView->setColumnHidden(QCDRSortFilterModel::COL_FILEKEY, true); // file key
    ui->tableView->setColumnHidden(QCDRSortFilterModel::COL_ID, true); // id


    //connect(ui->tableView->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(slot_table_clicked(int)));
    //ui->tableView->sortByColumn(QCDRSortFilterModel::COL_ID);
    //ui->tableView->so

    // высоту ячейки
    ui->tableView->verticalHeader()->setDefaultSectionSize(18);
//    ui->tableView->setColumnWidth(QCDRTableModel::COL_IN_TYPE, 150);
//    ui->tableView->setColumnWidth(QCDRTableModel::COL_OUT_TYPE, 150);
//    ui->tableView->setColumnWidth(QCDRTableModel::COL_CRELEASE, 150);

    fillPrefixList();


    directionName = new QSqlTableModel();
    directionName->setTable("DirectionName");
    directionName->setHeaderData(0, Qt::Horizontal, "id");
    directionName->setHeaderData(1, Qt::Horizontal, "Имя");

    //directionName->setEditStrategy(QSqlTableModel::OnManualSubmit);
    directionName->select();
//    row = directionName->rowCount();
//    if (!row) {
//        QSqlQuery query;
//        query.prepare("INSERT INTO  DirectionName (name) "
//                                   "VALUES (:name)");
//        query.bindValue(":name", QString("Супер канал"));
//        if (!query.exec())
//            qDebug() << "Unable to insert value" << query.lastError();

//        directionName->select();
//    }

    channelModel = new QChannelTableModel();
    channelModel->setTable("DirectionChannel");
    //channelModel->setEditStrategy(QSqlTableModel::OnFieldChange);
    channelModel->setHeaderData(0, Qt::Horizontal, "key");
    channelModel->setHeaderData(1, Qt::Horizontal, "C");
    channelModel->setHeaderData(2, Qt::Horizontal, "По");

    channelModel->select();
//    row = channelModel->rowCount();

//    if (!row) {
//        QSqlQuery query;
//        query.prepare("INSERT INTO  DirectionChannel (key, fr, by) "
//                                   "VALUES (:key, :fr, :by)");
//        query.bindValue(":key", 1);
//        query.bindValue(":fr", (100ULL << 32) | (128 << 16) | 1);
//        query.bindValue(":by", (100ULL << 32) | (128 << 16) | 32);
//        if (!query.exec())
//            qDebug() << "Unable to insert value" << query.lastError();

//        channelModel->select();
//    }
    showRecordCount();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::addFileListToCDRbase(const QStringList &files)
{
    //Последовательная обработка файлов
    //int fileIndex = 0;
    load = new QThread();
    worker = new ProgressWorker(&logdb, files, progressDialog);
    progressDialog = new QProgressDialog(); //QObject не может копироваться, поэтому указатель
    progressDialog->setWindowTitle(tr("Загрузка файлов"));
    progressDialog->show();
    worker->moveToThread(load);
    connect(load, SIGNAL(started()), worker, SLOT(addFileListToCDRbase()));
    connect(worker, SIGNAL(finished()), load, SLOT(quit())); // оповестим процесс что мы заончили
    connect(worker, SIGNAL(finished()), SLOT(on_worker_finish())); // оповестим MainWindow что мы закончили
    connect(worker, SIGNAL(listProgress(int, int)), progressDialog, SLOT(on_listProgress(int, int)));
    connect(worker, SIGNAL(fileProgress(int)), progressDialog, SLOT(on_fileProgress(int)));
    connect(progressDialog, SIGNAL(closed()),  load, SLOT(quit()));
    connect(progressDialog, SIGNAL(closed()),  SLOT(FileLoadDialog_closed()));
    load->start();
    progressDialog->exec(); // запустили окно
/*    for (QString str : files) {
        QFileInfo fileInfo(str);
        QSqlQuery q;
        if (!q.exec(QString("select id from LoadedFile where name = '%1'").arg(fileInfo.fileName())))
            qDebug() << q.lastError().text();

        if (q.next())
            continue;
        else {// в базе нет
            // добавляем в базу
            q.prepare("insert into LoadedFile (name) "
                      "values (:name)");
            q.bindValue(":name", QString(fileInfo.fileName()));
            if (!q.exec())
                qDebug() << q.lastError().text();

            if (q.exec(QString("select id from LoadedFile where name = '%1'").arg(fileInfo.fileName()))) {
                if (q.next()) {
                    int fileid = q.value(0).toInt();
                    progress->setFileSize(fileInfo.size());
                    addCDRFileToDB(str, fileid, progressDialog, fileInfo.size());
                }
            }
        }
        progressDialog->setFileNum(fileIndex, files.size());
        fileIndex++;
        //иначе не успевает перерисовываться
        QEventLoop loop;
        QTimer::singleShot(1, &loop, SLOT(quit()));
        loop.exec();
        progressDialog->repaint();
        qDebug() << fileIndex;
    }*/
    worker->stop(); // вышли из окна остановили процесс
    load->exit();
    if(progressDialog)
        delete progressDialog;
    progressDialog = NULL;
    if(worker)
        delete worker;
    worker = NULL;
    if(load)
        delete load;
    load = NULL;
}

void MainWindow::on_worker_finish()
{
    qDebug() << "finish";
    if (progressDialog)
        progressDialog->close();
}

void MainWindow::FileLoadDialog_closed(){
    worker->stop(); // вышли из окна остановили процесс
    load->exit();
}

void MainWindow::on_actionOpen_triggered()
{
    //Выбор нескольких файлов для обработки
    QStringList filesway = QFileDialog::getOpenFileNames(0, tr("Открыть"), "", "*.log *.txt");
    if (filesway.empty())
        return;

    addFileListToCDRbase(filesway);
//    QCDRTableModel *model = static_cast<QCDRTableModel *>(ui->tableView->model());
//    cdrModel->submitAll();

    cdrModel->select();
    ui->tableView->reset();

    showRecordCount();
}

//QMessageBox writeError(QMessageBox::Warning,
//                                   tr("Ошибка записи файла"),
//                                   tr("произошла ошибка записи файла ") + (diskFile ? diskFile->fileName() : ""),
//                                   QMessageBox::Ok,
//                                   this);
//            writeError.setModal(true);
//            writeError.exec();


#ifdef Q_OS_WIN

#define CLEAR_SSH_VAR \
    libssh2_session_disconnect(session, "Normal Shutdown, Thank you for playing"); \
    libssh2_session_free(session); \
    closesocket(sock);

#else
#define CLEAR_SSH_VAR \
    libssh2_session_disconnect(session, "Normal Shutdown, Thank you for playing"); \
    libssh2_session_free(session); \
    ::close(sock);

#endif

void MainWindow::on_action_SSH_import_triggered()
{
    // открытие файла по ssh
    // читаем опции из файла
    QSettings settings;
    QString host = settings.value("host", "127.0.0.1").toString();
    QString port = settings.value("port", "22").toString();
    QString user = settings.value("user", "atsuser").toString();
    QString password = settings.value("password", "atsuser").toString();

    QSSHLoginDialog login(host, port, user, password);

    if (login.exec() != QDialog::Accepted)
        return;

#ifdef Q_OS_WIN
    WSADATA wsadata;

    WSAStartup(MAKEWORD(2,0), &wsadata);
#endif

    LIBSSH2_SESSION *session = NULL;
    int sock = -1;
    QStringList v;

    try {
        int rc = libssh2_init (0);

        if (rc != 0)
            throw(std::logic_error(tr("Ошибка инициализации ssh2").toStdString()));


        v = login.getIP().split(':');
        QString ip  = v.size() ? v[0] : "127.0.0.1";
        QString port = v.size() == 2 ? v[1] : "22" ;
        QString user = login.getUser();
        QString password = login.getPassword();

        unsigned long hostaddr = inet_addr(ip.toStdString().c_str());

        sock = socket(AF_INET, SOCK_STREAM, 0);

        struct sockaddr_in sin;
        sin.sin_family = AF_INET;
        sin.sin_port = htons(port.toInt());
        sin.sin_addr.s_addr = hostaddr;
        int error = ::connect(sock, (struct sockaddr*)(&sin), sizeof(struct sockaddr_in));
        if (error != 0)
            throw(std::logic_error(tr("Ошибка %1 подключения на ip адрес %2").arg(strerror(error)).arg(ip.toStdString().c_str()).toStdString()));

        session = libssh2_session_init();
        if(!session)
            throw(std::logic_error(tr("Ошибка создания сессиии ssh2").toStdString()));

        rc = libssh2_session_handshake(session, sock);

        if (rc)
            throw(std::runtime_error(tr("Ошибка рукопожатия ssh2").toStdString()));

        if (libssh2_userauth_password(session, user.toStdString().c_str(), password.toStdString().c_str()))
            throw(std::runtime_error(tr("Ошибка авторизации ssh2").toStdString()));


        // успешно залогинились, все сохраняем параметры
        settings.setValue("host", ip);
        settings.setValue("port", port);
        settings.setValue("user", user);
        settings.setValue("password", password);

        v.clear();

        LIBSSH2_CHANNEL *channel = libssh2_channel_open_session(session);
        if (channel) {
            libssh2_channel_set_blocking(channel, 1);
            libssh2_channel_exec(channel, QString(QString("ls -r -c ") + __FILE_PATH__ + QString("*.log | xargs -n1 basename")).toLocal8Bit().data());

            char buf[1000];

            QString out;
            int res = 0;
            do  {
                memset(buf, 0 , sizeof(buf));
                res = libssh2_channel_read(channel, buf, sizeof(buf) - 1);
                out.append(buf);
            } while (res > 0);

            v = out.split("\n", QString::SkipEmptyParts);

            libssh2_channel_close(channel);
            libssh2_channel_free(channel);
        } else
            throw(std::runtime_error(tr("Ошибка создания канала ssh").toStdString()));
    }

    catch (const std::logic_error& e) {
        QMessageBox error(QMessageBox::Warning, tr("Ошибка"), e.what(), QMessageBox::Ok, this);
        error.setModal(true);
        error.exec();
        return;
    }

    catch (const std::runtime_error& e) {
        CLEAR_SSH_VAR;
        QMessageBox error(QMessageBox::Warning, tr("Ошибка"), e.what(), QMessageBox::Ok, this);
        error.setModal(true);
        error.exec();
        return;
    }

    // создаем окно с полученой выборкой
    qDebug() << v;
    while (1) {
        QSSHSelectDialog sshSelectDlg(v);
        if (sshSelectDlg.exec() != QDialog::Accepted)
            break;

        QStringList selFiles = sshSelectDlg.getSelectedFiles();
        if (selFiles.empty())
            break;

        for (auto f: selFiles) {
            try {
                QSSHFile sshf(session, f, this);

                if (!sshf.open(QIODevice::ReadOnly))
                    continue;


                QTextStream in(&sshf);
                QSqlDatabase::database().transaction();
                while (!in.atEnd())
                {
                    Qcallog logstr;
                    in >> logstr;
                }
                QSqlDatabase::database().commit();
                sshf.close();
            }
            catch (const std::runtime_error& e) {
                continue;
            }

        }


        // загружаем данные по ssh
        break;
    }

    CLEAR_SSH_VAR
    libssh2_exit();

#ifdef Q_OS_WIN
    WSACleanup();
#endif

    cdrModel->select();
    ui->tableView->reset();

    showRecordCount();

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
    QProgramPropertyDialog propertyDialog(nationalCode, internationalCode, directionName, channelModel );
    propertyDialog.exec();

    fillPrefixList();

    applyFilter();
}

void MainWindow::applyFilter()
{
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

    auto appendAdressFilter = [] (QString &f, const QString &filterStr, const QString &paramName) {

        if (filterStr.isEmpty())
            return;

        if (!f.isEmpty())
            f.append(" and ");

        QStringList addressList = filterStr.split(',');

        bool firstElementList = true;
        //for (auto address : addressList) {
        if (!addressList.isEmpty())
            f.append(" ( ");
        while(!addressList.isEmpty()) {
            QString address = addressList.front();
            addressList.pop_front();

            bool isRanged = false;

            Qcallog::s a1;
            Qcallog::s a2;
            // адрес может быть еще диапазоном

            if (address.contains('-')) {
                isRanged = true;
                QStringList fromto = address.split('-');
                QTextStream str(&fromto[0]);
                str >> a1;

                str.setString(&fromto[1]);
                str >> a2;
            } else {
                QTextStream str(&address);
                try {
                    str >> a1;
                }
                catch (std::invalid_argument& e) // возможно это имя направления
                {
                    //qDebug() << e.what() << endl;
                    QSqlQuery q;
                    if (!q.exec(QString("select id from DirectionName where name = '%1'").arg(address))) {
                        qDebug() << "error select from table DirectionName";
                        continue;
                    }

                    if (q.next()) { // отбираем id имени
                        int id = q.value("id").toInt();
                        if (!q.exec(QString("select fr, by from DirectionChannel where key = %1").arg(id))) {
                            qDebug() << "error select from table DirectionChannel";
                            continue;
                        }

                        while (q.next()) {
                            qint64 fr = q.value("fr").toLongLong();
                            qint64 by = q.value("by").toLongLong();

                            QString addressName = QString("C%1%2%3-C%4%5%6")
                                    .arg((fr >> 32) & 0xff, 3, 10, QLatin1Char('0'))
                                    .arg((fr >> 16) & 0xff, 3, 10, QLatin1Char('0'))
                                    .arg((fr >> 00) & 0xff, 3, 10, QLatin1Char('0'))
                                    .arg((by >> 32) & 0xff, 3, 10, QLatin1Char('0'))
                                    .arg((by >> 16) & 0xff, 3, 10, QLatin1Char('0'))
                                    .arg((by >> 00) & 0xff, 3, 10, QLatin1Char('0'));
                            addressList.push_back(addressName);
                        }
                    }

                    continue;
                }
            }

            if (!firstElementList)
                f.append(" or ");
            firstElementList = false;

            const char type = a1.getType();

            if (type == 'A') {
                f.append(QString("(%1type = '%2'").arg(paramName).arg(type));
                if (!isRanged)
                    f.append(QString(" and %1inc1 = %2").arg(paramName).arg(a1.getInc1()));
                else
                    f.append(QString(" and %1inc1 >= %2 and %1inc1 <= %3").arg(paramName).arg(a1.getInc1()).arg(a2.getInc1()));
                f.append(")");

            }

            if (type == 'C') {
                f.append(QString("(%1type = '%2'").arg(paramName).arg(type));
                if (!isRanged) {
                    if (a1.getInc1() != "*") {// все модули
                        f.append(QString(" and %1inc1 = %2").arg(paramName).arg(a1.getInc1()));
                        if (a1.getInc2() != "*") { // все потоки
                            f.append(QString(" and %1inc2 = %2").arg(paramName).arg(a1.getInc2()));
                            if (a1.getInc3() != "*") // все каналы
                                f.append(QString(" and %1inc3 = %2").arg(paramName).arg(a1.getInc3()));
                        }
                    }
                } else {
                    f.append(QString(" and %1inc1 >= %2 and %1inc1 <= %3").arg(paramName).arg(a1.getInc1()).arg(a2.getInc1()));
                    f.append(QString(" and %1inc2 >= %2 and %1inc2 <= %3").arg(paramName).arg(a1.getInc2()).arg(a2.getInc2()));
                    f.append(QString(" and %1inc3 >= %2 and %1inc3 <= %3").arg(paramName).arg(a1.getInc3()).arg(a2.getInc3()));
                }

                f.append(")");
            }
        }
        if (!firstElementList)
            f.append(" ) ");
    };

    QString filter;
//    if (!propertyFilter.abinf().isEmpty()) {

    appendAdressFilter(filter, propertyFilter.abinf(), "in");

    appendLikeFilter(filter, propertyFilter.inaonf(), "inanum");
    appendLikeFilter(filter, propertyFilter.innumf(), "innum");

    appendAdressFilter(filter, propertyFilter.aboutf(), "out");

    appendLikeFilter(filter, propertyFilter.outaonf(), "outanum");
    appendLikeFilter(filter, propertyFilter.outnumf(), "outnum");

    if ( !propertyFilter.busylenfromf().isEmpty()) {
        if (!filter.isEmpty())
            filter.append(" and ");
        int busylenfrom = QTime(0,0).secsTo(QTime::fromString(propertyFilter.busylenfromf(), "hh:mm:ss"));
        filter.append(QString(" linelen >= %1").arg(busylenfrom));
    }
    if ( !propertyFilter.busylentof().isEmpty()) {
        if (!filter.isEmpty())
            filter.append(" and ");
        int busylento = QTime(0,0).secsTo(QTime::fromString(propertyFilter.busylentof(), "hh:mm:ss"));
        filter.append(QString(" linelen <= %1").arg(busylento));
    }
    if ( !propertyFilter.talklenfromf().isEmpty()) {
        if (!filter.isEmpty())
            filter.append(" and ");
        int talklenfrom = QTime(0,0).secsTo(QTime::fromString(propertyFilter.talklenfromf(), "hh:mm:ss"));
        filter.append(QString(" callen >= %1").arg(talklenfrom));
    }
    if ( !propertyFilter.talklentof().isEmpty()) {
        if (!filter.isEmpty())
            filter.append(" and ");
        int talklento= QTime(0,0).secsTo(QTime::fromString(propertyFilter.talklentof(), "hh:mm:ss"));
        filter.append(QString(" callen <= %1").arg(talklento));
    }

    //фильтр по дате
    if ( !propertyFilter.datesincef().isEmpty()) {
        if (!filter.isEmpty())
            filter.append(" and ");
        int datesince = QDate::fromString(propertyFilter.datesincef(), "dd.MM.yy").addYears(100).toJulianDay();
        filter.append(QString(" date >= %1").arg(datesince));
    }
    if ( !propertyFilter.datetof().isEmpty()) {
        if (!filter.isEmpty())
            filter.append(" and ");
        int dateto = QDate::fromString(propertyFilter.datetof(), "dd.MM.yy").addYears(100).toJulianDay();
        filter.append(QString(" date <= %1").arg(dateto));
    }

    //фильтр по времени
    if ( !propertyFilter.timefromf().isEmpty()){
        if (!filter.isEmpty())
            filter.append(" and ");
        int timefrom = QTime(0,0).secsTo(QTime::fromString(propertyFilter.timefromf(), "hh:mm:ss"));
        filter.append(QString(" time >= %1").arg(timefrom));
    }

    if ( !propertyFilter.timetof().isEmpty()){
        if (!filter.isEmpty())
            filter.append(" and ");
        int timeto = QTime(0,0).secsTo(QTime::fromString(propertyFilter.timetof(), "hh:mm:ss"));
        filter.append(QString(" time <= %1").arg(timeto));
    }

    // фильтр по признаку завершения
    if (propertyFilter.releaseCause()) {
        if (!filter.isEmpty())
            filter.append(" and ");
        filter.append(QString(" relreason = %1").arg(propertyFilter.releaseCause()));
    }

    // фильтр по типу вызова
    QQueue <qint8> q = propertyFilter.typeCalls();
    QString callTypeFilter;
    for (qint8 i : q) {
        if (!callTypeFilter.isEmpty())
            callTypeFilter.append(" or ");
        switch (i) {
            case Qcallog::TYPE_NULL: break;
            case Qcallog::TYPE_LOCAL: callTypeFilter.append("(intype = 'A' and outtype = 'A'"); break;
            case Qcallog::TYPE_IN_LOCAL: callTypeFilter.append("(intype = 'C' and outtype = 'A'"); break;
            case Qcallog::TYPE_OUT_NATIONAL: callTypeFilter.append(QString("(intype = 'A' and outtype = 'C' %1 %2").arg(appendPrefixFilter(nationalCode)).arg(appendPrefixFilter(internationalCode, true))); break;
            case Qcallog::TYPE_OUT_INTERNATIONAL: callTypeFilter.append(QString("(intype = 'A' and outtype = 'C' %1").arg(appendPrefixFilter(internationalCode))); break;
            case Qcallog::TYPE_OUT_LOCAL: callTypeFilter.append(QString("(intype = 'A' and outtype = 'C' %1").arg(appendPrefixFilter(nationalCode, true).arg(appendPrefixFilter(internationalCode, true)))); break;
            case Qcallog::TYPE_TRANZIT_NATIONAL: callTypeFilter.append(QString("(intype = 'C' and outtype = 'C' %1 %2").arg(appendPrefixFilter(nationalCode)).arg(appendPrefixFilter(internationalCode, true))); break;
            case Qcallog::TYPE_TRANZIT_INTERNATIONAL: callTypeFilter.append(QString("(intype = 'C' and outtype = 'C' %1").arg(appendPrefixFilter(internationalCode))); break;
            case Qcallog::TYPE_TRANZIT_LOCAL: callTypeFilter.append(QString("(intype = 'C' and outtype = 'C' %1 %2").arg(appendPrefixFilter(nationalCode, true)).arg(appendPrefixFilter(internationalCode, true))); break;
        }
        if (!callTypeFilter.isEmpty())
            callTypeFilter.append(")");
    }

    if (!callTypeFilter.isEmpty()) {
        if (!filter.isEmpty())
            filter.append(" and ");
        filter.append(callTypeFilter);
    }


//    if (propertyFilter.typeCalls()) {

//        if (!filter.isEmpty())
//            filter.append(" and ");
//        switch (propertyFilter.typeCalls()) {
//            case Qcallog::TYPE_LOCAL: filter.append("intype = 'A' and outtype = 'A'"); break;
//            case Qcallog::TYPE_IN_LOCAL: filter.append("intype = 'C' and outtype = 'A'"); break;
//            case Qcallog::TYPE_OUT_NATIONAL: filter.append(QString("intype = 'A' and outtype = 'C' %1 %2").arg(appendPrefixFilter(nationalCode)).arg(appendPrefixFilter(internationalCode, true))); break;
//            case Qcallog::TYPE_OUT_INTERNATIONAL: filter.append(QString("intype = 'A' and outtype = 'C' %1").arg(appendPrefixFilter(internationalCode))); break;
//            case Qcallog::TYPE_OUT_LOCAL: filter.append(QString("intype = 'A' and outtype = 'C' %1").arg(appendPrefixFilter(nationalCode, true).arg(appendPrefixFilter(internationalCode, true)))); break;
//            case Qcallog::TYPE_TRANZIT_NATIONAL: filter.append(QString("intype = 'C' and outtype = 'C' %1 %2").arg(appendPrefixFilter(nationalCode)).arg(appendPrefixFilter(internationalCode, true))); break;
//            case Qcallog::TYPE_TRANZIT_INTERNATIONAL: filter.append(QString("intype = 'C' and outtype = 'C' %1").arg(appendPrefixFilter(internationalCode))); break;
//            case Qcallog::TYPE_TRANZIT_LOCAL: filter.append(QString("intype = 'C' and outtype = 'C' %1 %2").arg(appendPrefixFilter(nationalCode, true)).arg(appendPrefixFilter(internationalCode, true))); break;
//        }
//    }

    cdrModel->setFilter(filter);
    cdrModel->select();
    ui->tableView->reset();

    showRecordCount();
}

void MainWindow::on_pushButton_clicked()
{

    directionName->select();
    int nameIndex = directionName->fieldIndex("name");
    directionName->select();

    while (directionName->canFetchMore())
        directionName->fetchMore();

    QStringList names;
    for (int i = 0; i < directionName->rowCount(); i++)
        names << directionName->data(directionName->index(i, nameIndex)).toString();

    FilterDialog* Form = new FilterDialog(propertyFilter, names);

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
    for (auto i : indexes) {

        int v = cdrModel->data(cdrModel->index(i.row(), idIndex)).toInt();

        if (!query.exec(QString("DELETE FROM logbase WHERE id = %1").arg(v)))
            qDebug() << "Unable to delete value" << logdb.lastError();
    }
    QSqlDatabase::database().commit();

    cdrModel->select();
    ui->tableView->reset();

    showRecordCount();
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

void MainWindow::on_pushSave_clicked()
{
    // выбираем имя файла
    QString filesave = QFileDialog::getSaveFileName(0, tr("Сохранение"), "", "*.csv");
    if (filesave.isEmpty())
        return;

    QItemSelectionModel *selModel = ui->tableView->selectionModel();

    if (!selModel)
        return;

    QModelIndexList indexes = selModel->selectedRows();


    auto s = [] (QTextStream &s, const QSqlTableModel *m, const int row) {
        for (int j = 0; j < m->columnCount(); j++) {
            int role = Qt::DisplayRole;
            if (j == QCDRSortFilterModel::COL_ID)
                continue;
            if (j == QCDRSortFilterModel::COL_CRELEASE)
                role = Qt::EditRole;
            s << m->data(m->index(row, j), role).toString() << ';';
        }
        s << endl;
    };

    QFile csv(filesave);
    if (csv.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream csvstram(&csv);

        if (indexes.count() <= 1) {
            // записываем все что видим в таблице
            cdrModel->select();
            while (cdrModel->canFetchMore())
                cdrModel->fetchMore();
            for (int i = 0; i < cdrModel->rowCount(); i++)
                s(csvstram, cdrModel, i);
        }
        else {
            // записываем только выделенные
            for (auto i : indexes)
                s(csvstram, cdrModel, i.row());
        }
    }
    csv.close();
}

//void MainWindow::slot_table_clicked(int index)
//{
//    qDebug() << "cicked" << index;
//}

const  QString minutes (int n)
{
    const QString m[3] = {"минута", "минуты", "минут"};
    n = abs (n);
    int d = n % 100;
    if (d >= 20)
        d = n % 10;
    int i = 2;
    if (d == 1)
        i = 0;
    if (d >= 2 && d <= 4)
        i = 2;
    return m[i];
}

void MainWindow::showRecordCount()
{
    //while (cdrModel->canFetchMore())
    //    cdrModel->fetchMore();

    QSqlQuery q;
    QString qv = QString("SELECT COUNT(*) FROM logbase %1").arg(cdrModel->filter().isEmpty() ? "" : QString(" WHERE %1").arg(cdrModel->filter()));
    if (!q.exec(QString("SELECT COUNT(*) FROM logbase %1").arg(cdrModel->filter().isEmpty() ? "" : QString(" WHERE %1").arg(cdrModel->filter()))))
        qDebug() << "get count" << logdb.lastError();


    //int rowCount = cdrModel->rowCount();
    int rowCount = 0;
    if (q.next())
        rowCount = q.value(0).toInt();


    if (!q.exec(QString("SELECT sum(callen) FROM logbase %1").arg(cdrModel->filter().isEmpty() ? "" : QString(" WHERE %1").arg(cdrModel->filter()))))
        qDebug() << "get sum" << logdb.lastError();
    int callLen = 0;
    if (q.next())
        callLen = q.value(0).toInt();


//    int talkDays = 0;
//    if (callLen >= 24*60*60) {// больше суток
//        talkDays = callLen / (24*60*60);
//        callLen = callLen  % (24*60*60);
//    }

    //QTime talkTime = QTime(0, 0).addSecs(callLen);
    //if (talkDays > 0)
    //    statusBar()->showMessage(tr("Записей %1 время разговора %2 суток %3").arg(rowCount).arg(talkDays).arg(talkTime.toString("hh:mm:ss")));
    //else

    statusBar()->showMessage(tr("Записей %1, время разговора %2 %3").arg(rowCount).arg(callLen / 60).arg(minutes(callLen / 60)));


    ui->tableView->setSortingEnabled(true);
    ui->tableView->setColumnHidden(QCDRSortFilterModel::COL_ID, false); // id
    ui->tableView->sortByColumn(QCDRSortFilterModel::COL_ID, Qt::AscendingOrder);
    ui->tableView->setColumnHidden(QCDRSortFilterModel::COL_ID, true); // id

    if (rowCount > 500)
        ui->tableView->setSortingEnabled(false);
}

#define _GRAPH_(name) \
    Graph* graphic = new Graph(); \
    graphic->build##name(cdrModel);

//Построение графика по причинам отбоя
void MainWindow::on_pushDrawReleaseCause_clicked()
{
    _GRAPH_(ReportReleaseCause);
}

//Построение графика по причинам отбоя
void MainWindow::on_actionReleaseCause_triggered()
{
    _GRAPH_(ReportReleaseCause);
}

//Построение графика по успешности вызовов по дате
void MainWindow::on_actionSucessCallsDate_triggered()
{
    _GRAPH_(ReportSucessCallsDate);
}

//Построение графика по успешности вызовов по дате
void MainWindow::on_pushDrawSucessCallsDate_clicked()
{
    _GRAPH_(ReportSucessCallsDate);
}

//Построение графика по успешности вызовов по времени
void MainWindow::on_pushDrawSucessCallsTime_clicked()
{
    _GRAPH_(ReportSucessCallsTime);
}

//Построение графика по успешности вызовов по времени
void MainWindow::on_actionSucessCallsTime_triggered()
{
    _GRAPH_(ReportSucessCallsTime);
}


//Построение графика по успешности вызовов по дню недели
void MainWindow::on_pushDrawSucessCallsWeekDay_clicked()
{
    _GRAPH_(ReportSucessCallsWeekDay);
}

//Построение графика по успешности вызовов по дню недели
void MainWindow::on_actionSucessCallsWeekDay_triggered()
{
    _GRAPH_(ReportSucessCallsWeekDay);
}


// распределение по кагалам
void MainWindow::on_pushAbonents_clicked()
{
    _GRAPH_(ReportAbonents);
}

void MainWindow::on_actionAbonents_triggered(){
    _GRAPH_(ReportAbonents);
}

void MainWindow::on_pushLoad_clicked()
{
    _GRAPH_(ReportLoad);
}

void MainWindow::on_actionLoad_triggered(){
    _GRAPH_(ReportLoad);
}

void MainWindow::on_pushDrawCallens_clicked()
{
    _GRAPH_(ReportCallens);
}

void MainWindow::on_actionDrawCallens_triggered(){
    _GRAPH_(ReportCallens);
}
