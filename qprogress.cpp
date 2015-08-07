#include "qprogress.h"
#include "ui_qprogress.h"

QProgress::QProgress(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::qprogress)
{
    ui->setupUi(this);
}

QProgress::~QProgress()
{
    delete ui;
}

void QProgress::setFileSize(int size){
    ui->progressBar->setMaximum(size);
}

void QProgress::refresh(int size){
    ui->progressBar->setValue(size);
}

void QProgress::setCurFileNum(int filenum, int sumfiles){
    ui->curFile->setText("Загружено " + QString("%1").arg(filenum) + " файлов" + " из " + QString("%1").arg(sumfiles));
}

