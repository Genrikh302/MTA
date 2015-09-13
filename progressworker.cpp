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
    int bitesum = 0;
    int i = 0;
    //statusBar()->showMessage(tr("Loaded %1").arg(file), 2000);
    if(!inputFile.open(QIODevice::ReadOnly))
    {
        qDebug()<<"ERROR: Can't open file "<< file;
        return;
    }
    QTextStream in(&inputFile);
//    std::list <Qcallog> l;
    //Занесение данных из файлов в лист, из листа в базу
    QSqlDatabase::database().transaction();
//    progress->refresh(0);
    emit fileProgress(0);
    while (!in.atEnd())
    {
        //QString line = inputFile.readLine();
        //при включении обновления програссбара срабатывает throw из qcallog
        if(bitesum > size / 100  * i){
             emit fileProgress(bitesum * 100 / size);
        }
//        if(i % 50 == 0){
//            emit fileProgress(in.pos() * 100 / size);
//        }
        Qcallog logstr;
        in >> logstr;
        QString logstring = logstr.toString();
        logstr.setFilekey(fileid);
        //l.push_back(logstr);
        *logdb << logstr;
        i++;
        bitesum += logstring.size() * sizeof(char) + sizeof(char);
        //qDebug() << file <<  in.pos() << bitesum;
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
    emit listProgress(0,1);
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
    }
    emit finished(); // отправляем сообщение что закончили работу
}

ProgressWorker::~ProgressWorker()
{

}

