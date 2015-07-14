#include "filterdialog.h"
#include "ui_filterdialog.h"
#include "mainwindow.h"
#include <QtGui>
#include <QDebug>
#include <QRegExp>
#include <QRegExpValidator>
#include "cdrtablemodel.h"


//TODO Сделать выбор не только по каналам, но и по обозванным направлениям

FilterDialog::FilterDialog(const PropertyFilter &propertyFilter, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FilterDialog)
{
    ui->setupUi(this);

    //установка начальных значений даты и времени
    QDate datesince, dateto;
    ui->datesince->setDate(datesince.fromString("01-01-2000", "dd-MM-yyyy"));
    ui->dateto->setDate(dateto.currentDate());
    QTime timesince, timeto;
    ui->timesince->setTime(timesince.fromString("00:00:00", "hh:mm:ss"));
    ui->timeto->setTime(timeto.fromString("23:59:59", "hh:mm:ss"));

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

    // валидатор для времени
    regExp = QRegExp("([0-2]{1,1}[0-9]{1,1}[:][0-2]{1,1}[0-9]{1,1}[:][0-2]{1,1}[0-9]{1,1})");
    validator = new QRegExpValidator(regExp, this);
    ui->busyfrom->setValidator(validator);
    ui->busyto->setValidator(validator);
    ui->talklenfrom->setValidator(validator);
    ui->talklento->setValidator(validator);

    // каналы
    ui->abin->setText(propertyFilter.abinf());
    ui->about->setText(propertyFilter.aboutf());

    // выход
    ui->outaon->setText(propertyFilter.outaonf());
    ui->outnum->setText(propertyFilter.outnumf());

    // вход
    ui->inaon->setText(propertyFilter.inaonf());
    ui->innum->setText(propertyFilter.innumf());

    // время разговора
    ui->talklenfrom->setText(propertyFilter.talklenfromf());
    ui->talklento->setText(propertyFilter.talklentof());

    // длительность занятия
    ui->busyfrom->setText(propertyFilter.busylenfromf());
    ui->busyto->setText(propertyFilter.busylentof());

    // даты
    ui->datesince->setDate(QDate::fromString(propertyFilter.datesincef(),"dd.MM.yy").addYears(100));
    ui->dateto->setDate(QDate::fromString(propertyFilter.datetof(),"dd.MM.yy").addYears(100));

    //причины отбоя
    ui->reason->addItem(QString("%1").arg(tr("не задана")), QVariant(0));
    foreach (auto v, QCDRSortFilterModel::causeValue.keys())
        ui->reason->addItem(QString("%1 - %2").arg(QCDRSortFilterModel::causeValue.value(v)).arg(v), QVariant(v));
    int index = ui->reason->findData(propertyFilter.releaseCause());
    ui->reason->setCurrentIndex(index < 0 ? 0 : index);


    // типы вызова
    ui->type->addItem(QString("%1").arg(tr("не задана")), QVariant(0));
    for (int i = Qcallog::TYPE_LOCAL; i < Qcallog::TYPE_LAST_CODE; i++)
        ui->type->addItem(QString(Qcallog::getQStringTypeCalls(i)), QVariant(i));
    index = ui->type->findData(propertyFilter.typeCalls());
    ui->type->setCurrentIndex(index < 0 ? 0 : index);
}

FilterDialog::~FilterDialog()
{
    delete ui;
}

void FilterDialog::writefil(PropertyFilter &f)
{
    f.setAbinf(ui->abin->text());
    f.setAboutf(ui->about->text());
    f.setDatesincef(ui->datesince->text());
    f.setDatetof(ui->dateto->text());
    f.setOutaonf(ui->outaon->text());
    f.setOutnumf(ui->outnum->text());
    f.setInaonf(ui->inaon->text());
    f.setInnumf(ui->innum->text());
    f.setTalklenfromf(ui->talklenfrom->text());
    f.setTalklentof(ui->talklento->text());
    f.setBusylenfromf(ui->busyfrom->text());
    f.setBusylentof(ui->busyto->text());
    f.setTimefromf(ui->timesince->text());
    f.setTimetof(ui->timeto->text());
    f.setReleaseCause(ui->reason->currentData().toInt());
    f.setTypeCalls(ui->type->currentData().toInt());
}

void FilterDialog::on_clearbutton_clicked()
{
    ui->abin->clear();
    ui->about->clear();
    ui->busyfrom->clear();
    ui->busyto->clear();
    ui->talklenfrom->clear();
    ui->talklento->clear();
    ui->inaon->clear();
    ui->innum->clear();
    ui->outaon->clear();
    ui->outnum->clear();
    QDate datesince, dateto;
    ui->datesince->setDate(datesince.fromString("01-01-2000", "dd-MM-yyyy"));
    ui->dateto->setDate(dateto.currentDate());
    QTime timesince, timeto;
    ui->timesince->setTime(timesince.fromString("00:00:00", "hh:mm:ss"));
    ui->timeto->setTime(timeto.fromString("23:59:59", "hh:mm:ss"));

    ui->reason->setCurrentIndex(0);
    ui->type->setCurrentIndex(0);
}


QString PropertyFilter::abinf() const
{
    return m_abinf;
}

void PropertyFilter::setAbinf(const QString &abinf)
{
    m_abinf = abinf;
}

QString PropertyFilter::datesincef() const
{
    return m_datesincef;
}

void PropertyFilter::setDatesincef(const QString &datesincef)
{
    m_datesincef = datesincef;
}

QString PropertyFilter::datetof() const
{
    return m_datetof;
}

void PropertyFilter::setDatetof(const QString &datetof)
{
    m_datetof = datetof;
}

QString PropertyFilter::timefromf() const
{
    return m_timefromf;
}

void PropertyFilter::setTimefromf(const QString &timefromf)
{
    m_timefromf = timefromf;
}

QString PropertyFilter::timetof() const
{
    return m_timetof;
}

void PropertyFilter::setTimetof(const QString &timetof)
{
    m_timetof = timetof;
}

QString PropertyFilter::busylenfromf() const
{
    return m_busylenfromf;
}

void PropertyFilter::setBusylenfromf(const QString &busylenfromf)
{
    m_busylenfromf = busylenfromf;
}

QString PropertyFilter::busylentof() const
{
    return m_busylentof;
}

void PropertyFilter::setBusylentof(const QString &busylentof)
{
    m_busylentof = busylentof;
}

QString PropertyFilter::talklenfromf() const
{
    return m_talklenfromf;
}

void PropertyFilter::setTalklenfromf(const QString &talklenfromf)
{
    m_talklenfromf = talklenfromf;
}

QString PropertyFilter::talklentof() const
{
    return m_talklentof;
}

void PropertyFilter::setTalklentof(const QString &talklentof)
{
    m_talklentof = talklentof;
}

QString PropertyFilter::inaonf() const
{
    return m_inaonf;
}

void PropertyFilter::setInaonf(const QString &inaonf)
{
    m_inaonf = inaonf;
}

QString PropertyFilter::innumf() const
{
    return m_innumf;
}

void PropertyFilter::setInnumf(const QString &innumf)
{
    m_innumf = innumf;
}

QString PropertyFilter::outaonf() const
{
    return m_outaonf;
}

void PropertyFilter::setOutaonf(const QString &outaonf)
{
    m_outaonf = outaonf;
}

QString PropertyFilter::outnumf() const
{
    return m_outnumf;
}

void PropertyFilter::setOutnumf(const QString &outnumf)
{
    m_outnumf = outnumf;
}

qint16 PropertyFilter::releaseCause() const
{
    return m_releaseCause;
}

void PropertyFilter::setReleaseCause(const qint16 &releaseCause)
{
    m_releaseCause = releaseCause;
}

qint8 PropertyFilter::typeCalls() const
{
    return m_typeCalls;
}

void PropertyFilter::setTypeCalls(const qint8 &typeCalls)
{
    m_typeCalls = typeCalls;
}
QString PropertyFilter::aboutf() const
{
    return m_aboutf;
}

void PropertyFilter::setAboutf(const QString &aboutf)
{
    m_aboutf = aboutf;
}
