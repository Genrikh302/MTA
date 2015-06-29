#include "filterdialog.h"
#include "ui_filterdialog.h"
#include "mainwindow.h"
#include <QtGui>
#include <QDebug>

FilterDialog::FilterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FilterDialog)
{
    ui->setupUi(this);
}

FilterDialog::~FilterDialog()
{
    delete ui;
}


/*void FilterDialog::getfil()
{
    abinf = ui->abin->text();
    aboutf = ui->about->text();
    datesincef = ui->datesince->text();
    datetof = ui->dateto->text();
    timesincef = ui->timesince->text();
    timetof = ui->timeto->text();
    busylenfromf = ui->busyfrom->text();
    busylentof = ui->busyto->text();
    talklenfromf = ui->talklenfrom->text();
    talklentof = ui->talklento->text();
    inaonf = ui->inaon->text();
    innumf = ui->innum->text();
    outaonf = ui->outaon->text();
    outnumf = ui->outnum->text();
    qDebug() << abinf << "+";
}*/

void FilterDialog::writefil(QString &_abinf, QString &_aboutf, QString &_datesincef, QString &_datetof, QString &_timesincef, QString &_timetof, QString &_busylenfromf, QString &_busylentof, QString &_talklenfromf,  QString &_talklentof, QString &_inaonf, QString &_innumf, QString &_outaonf, QString &_outnumf)
{
    _abinf = ui->abin->text();
    _aboutf = ui->about->text();
    _datesincef = ui->datesince->text();
    _datetof = ui->dateto->text();
    _timesincef = ui->timesince->text();
    _timetof = ui->timeto->text();
    _busylenfromf = ui->busyfrom->text();
    _busylentof = ui->busyto->text();
    _talklenfromf = ui->talklenfrom->text();
    _talklentof = ui->talklento->text();
    _inaonf = ui->inaon->text();
    _innumf = ui->innum->text();
    _outaonf = ui->outaon->text();
    _outnumf = ui->outnum->text();
}

void FilterDialog::on_clearbutton_clicked()
{
    ui->abin->clear();
    ui->about->clear();
    ui->datesince->clear();
    ui->dateto->clear();
    ui->timesince->clear();
    ui->timeto->clear();
    ui->busyfrom->clear();
    ui->busyto->clear();
    ui->talklenfrom->clear();
    ui->talklento->clear();
    ui->inaon->clear();
    ui->innum->clear();
    ui->outaon->clear();
    ui->outnum->clear();
    //qDebug() << abinf << "-";
}
