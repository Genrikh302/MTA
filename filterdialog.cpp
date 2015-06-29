#include "filterdialog.h"
#include "ui_filterdialog.h"
#include "mainwindow.h"
#include <QtGui>
#include <QDebug>
#include <QRegExp>
#include <QRegExpValidator>

FilterDialog::FilterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FilterDialog)
{
    ui->setupUi(this);

    // валидаторы на ввод каналов и абонентов
    //QRegExp regExp = QRegExp("(([C,c][0-9]{9,9})|([C,c][0-9]{0,8}[*]{1,1})|([A,a][0-9]{1,10}))");
    QRegExp regExp = QRegExp("(([C,c][0-9]{9,9})|([C,c](([*]{1,1})|([0-9]{3,3}[*]{1,1})|([0-9]{6,6}[*]{1,1})))|([A,a][0-9]{1,10}))");
    QRegExpValidator *validator = new QRegExpValidator(regExp, this);

    ui->abin->setValidator(validator);
    ui->about->setValidator(validator);

    // нужны валидаторы на ввод нумеров возможны цыфры и буквы A-F
    regExp = QRegExp("((([0-9,A-F,?]|([0-9,a-f,?])){0,28}[*]))");
    validator = new QRegExpValidator(regExp, this);
    ui->inaon->setValidator(validator);
    ui->innum->setValidator(validator);
    ui->outaon->setValidator(validator);
    ui->outnum->setValidator(validator);
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
