#ifndef QPROGRESS_H
#define QPROGRESS_H

#include <QDialog>

namespace Ui {
class qprogress;
}

class QProgress : public QDialog
{
    Q_OBJECT

public:
    explicit QProgress(QWidget *parent = 0);
    void setCurFileNum(int filenum, int sumfiles);
    void setAllFiles(int sumfiles);
    void refresh(int curpos);
    void setFileSize(int size);
    ~QProgress();

private:
    Ui::qprogress *ui;
    int curfilenum;
    int allfiles;
    int curpos;
    int filesize;
};

#endif // QPROGRESS_H
