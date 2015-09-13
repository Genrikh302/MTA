#ifndef PROGRESSWORKER_H
#define PROGRESSWORKER_H

#include <QObject>
#include "qlogdb.h"
#include "qprogressdialog.h"

class ProgressWorker : public QObject
{
    Q_OBJECT
    bool fExit;
private:
    Qlogdb* logdb;
    QStringList *files;
    QProgressDialog *progressDialog;
public:
    explicit ProgressWorker(Qlogdb *logdb, QStringList *files, QProgressDialog *progressDialog, QObject *parent = 0);
    //void addFileListToCDRbase();
    void addCDRFileToDB(const QString &file, int fileid, int size);
    ~ProgressWorker();

signals:
    void finished();
    void fileProgress(int value);
    void fileSize(int size);
    void listProgress(int filenum, int sumfiles);

public slots:
    void addFileListToCDRbase();
    void stop();
};

#endif // PROGRESSWORKER_H
