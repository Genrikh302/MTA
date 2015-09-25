#ifndef PROGRESSWORKER_H
#define PROGRESSWORKER_H

#include <QObject>
#include "qlogdb.h"
#include "qprogressdialog.h"

class MainProgressWorker : public QObject
{
    Q_OBJECT

private:

    QProgressDialog *progressDialog;

protected:
    bool fExit;
    Qlogdb* logdb;
     QStringList files;

public:
    MainProgressWorker(Qlogdb *logdb, const QStringList &files, QProgressDialog *progressDialog, QObject *parent = NULL);
    virtual void addCDRFileToDB(const QString &file, int fileid, int size) = 0;
    virtual ~MainProgressWorker() {}

signals:
    void finished();
    void fileProgress(int value);
    void fileSize(int size);
    void listProgress(int filenum, int sumfiles);

public slots:
    void addFileListToCDRbase();
    void stop();

};

class ProgressWorker : public MainProgressWorker
{
//    Q_OBJECT
public:
    explicit ProgressWorker(Qlogdb *logdb, const QStringList &files, QProgressDialog *progressDialog, QObject *parent = 0);
    void addCDRFileToDB(const QString &file, int fileid, int size);
    ~ProgressWorker();


//public slots:
//    void addFileListToCDRbase();
//    void stop();
};

#endif // PROGRESSWORKER_H
