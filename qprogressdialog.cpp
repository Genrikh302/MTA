#include "qprogressdialog.h"
#include "ui_qprogress.h"

QProgressDialog::QProgressDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::qprogress)
{
    ui->setupUi(this);
     ui->progressBar->setMaximum(100);
}

QProgressDialog::~QProgressDialog()
{
    delete ui;
}
void QProgressDialog::on_listProgress(int filenum, int sumfiles){
    ui->curFile->setText("Загружено " + QString("%1").arg(filenum) + " файлов" + " из " + QString("%1").arg(sumfiles));
}

void QProgressDialog::on_fileProgress(int value){
    ui->progressBar->setValue(value);
}

//void QProgressDialog::on_fileSize(int size){
//    ui->progressBar->setMaximum(size);
//}
