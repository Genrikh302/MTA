#ifndef QPROGRESS_H
#define QPROGRESS_H

#include <QDialog>

namespace Ui {
class qprogress;
}

class QProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QProgressDialog(QWidget *parent = 0);
    void setFileNum(int filenum, int sumfiles);
    void refresh(int curpos);
    void setFileSize(int size);
    ~QProgressDialog();

public slots:
    void on_fileProgress(int value);
    //void on_fileSize(int size);
    void on_listProgress(int filenum, int sumfiles);
private slots:
    void on_qprogress_rejected();

signals:
    void closed();
private:
    Ui::qprogress *ui;
    int curfilenum;
    int allfiles;
    int curpos;
    int filesize;
};

//class ProgressTask
//    : public QObject
//{
//    Q_OBJECT


//public:
//    ProgressTask( QObject *parent = NULL );
//    ~ProgressTask();


//private:
//};
#endif // QPROGRESS_H
