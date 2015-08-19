#include "progressworker.h"
#include <QDebug>
#include "qcallog.h"
//#include "qlogdb.h"
#include <unistd.h>

ProgressWorker::ProgressWorker(Qlogdb *logdb, QStringList *files,  QProgressDialog *progressDialog, QObject *parent) : QObject(parent)
{
    fExit = false;
    this->logdb = logdb;
    this->files = files;
    this->progressDialog = progressDialog;
}

void ProgressWorker::stop()
{
    fExit = true; // для остановки процесса
}

void ProgressWorker::addCDRFileToDB(const QString &file, int fileid, int size) {
    QFile inputFile(file);
    int i = 0;

    //statusBar()->showMessage(tr("Loaded %1").arg(file), 2000);
    if(!inputFile.open(QIODevice::ReadOnly))
    {
        qDebug()<<"ERROR: Can't open file "<< file;
        return;
    }
//   emit fileSize(size);
    QTextStream in(&inputFile);
//    std::list <Qcallog> l;
    //Занесение данных из файлов в лист, из листа в базу
    QSqlDatabase::database().transaction();
//    progress->refresh(0);
    while (!in.atEnd())
    {
        //QString line = inputFile.readLine();
        //при включении обновления програссбара срабатывает throw из qcallog
        //обновление сделано с частотой прохождения строк, тк шаг обновления .pos меняется и нельзя подобрать кратность
//        if(in.pos() > size/100 * i){
//             emit fileProgress(i);
//             i++;
//        }
        Qcallog logstr;
        in >> logstr;
        logstr.setFilekey(fileid);
        //l.push_back(logstr);
        *logdb << logstr;
        //qDebug() << in.pos() << inputFile.size();

    }
    QSqlDatabase::database().commit();
//    for(auto i = l.begin(); i != l.end(); i++)
//        i->print();
    inputFile.close();
}


void ProgressWorker::addFileListToCDRbase()
{
    //Последовательная обработка файлов
    int fileIndex = 0;

    for (QString str : *files) {
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
                    addCDRFileToDB(str, fileid, fileInfo.size());
                    emit listProgress(fileIndex, files->size());
                    fileIndex++;
                }
            }
        }
//        progressDialog->setFileNum(fileIndex, files.size());
//        fileIndex++;
//        //иначе не успевает перерисовываться
//        QEventLoop loop;
//        QTimer::singleShot(1, &loop, SLOT(quit()));
//        loop.exec();
//        progressDialog->repaint();
        //qDebug() << fileIndex;
    }
    emit finished(); // отправляем сообщение что закончили работу
}

ProgressWorker::~ProgressWorker()
{

}

