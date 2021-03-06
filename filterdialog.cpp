#include "filterdialog.h"
#include "ui_filterdialog.h"
#include "mainwindow.h"
#include <QtGui>
#include <QDebug>
#include <QRegExp>
#include <QRegExpValidator>
#include <QStyledItemDelegate>
#include "cdrtablemodel.h"


FilterDialog::FilterDialog(const PropertyFilter &propertyFilter, const QStringList &names, QWidget *parent) :
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

    QRegExp regExp = QRegExp(QString("("
                             "("
                             "([Cc](([0-9]{9,9})|([0-9]{9,9}-[Cc][0-9]{9,9})))|"
                             "([Cc]([*]|([0-9]{3,3}[*])|([0-9]{6,6}[*])))|"
                             "([Aa](([0-9]{1,10})|([0-9]{1,10}-[Aa][0-9]{1,10})))|"
                             "(%1)"
                             ")"
                             ",)+").arg(names.join("|")));

    QRegExpValidator *validator = new QRegExpValidator(regExp, this);

    ui->abin->addItem("");
    ui->abin->addItems(names);

    ui->about->addItem("");
    ui->about->addItems(names);

    ui->abin->setValidator(validator);
    ui->about->setValidator(validator);
//    ui->comboBox->setValidator(validator);


    ui->abin->lineEdit()->setPlaceholderText("C000000000,A00000");
    ui->about->lineEdit()->setPlaceholderText("C000000000,A00000");


    // нужны валидаторы на ввод нумеров возможны цифры и буквы A-F
    regExp = QRegExp("((([0-9,A-F,?]|([0-9,a-f,?])){0,28}[*]))");
    validator = new QRegExpValidator(regExp, this);
    ui->inaon->setValidator(validator);
    ui->innum->setValidator(validator);
    ui->outaon->setValidator(validator);
    ui->outnum->setValidator(validator);

    // валидатор для времени
    regExp = QRegExp("((([0-1]{1,1}[0-9]{1,1})|([2]{1,1}[0-3]{1,1})):[0-5]{1,1}[0-9]{1,1}:[0-5]{1,1}[0-9]{1,1})");
    validator = new QRegExpValidator(regExp, this);
    ui->busyfrom->setValidator(validator);
    ui->busyto->setValidator(validator);
    ui->talklenfrom->setValidator(validator);
    ui->talklento->setValidator(validator);

    // каналы
    //ui->abin->setText(propertyFilter.abinf());
    addr1 = propertyFilter.abinf();
    ui->abin->lineEdit()->setText(addr1);


    //ui->about->setText(propertyFilter.aboutf());
    addr2 = propertyFilter.aboutf();
    ui->about->lineEdit()->setText(addr2);

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
    QDate current = QDate::currentDate();
    if (propertyFilter.datesincef().isEmpty()) {
    #ifdef QT_DEBUG
        ui->datesince->setDate(current.addDays(-current.day() + 1).addMonths(-current.month() + 1));
    #else
        ui->datesince->setDate(current.addDays(-current.day() + 1));
    #endif
    }
    else
        ui->datesince->setDate(QDate::fromString(propertyFilter.datesincef(),"dd.MM.yy").addYears(100));

    if (propertyFilter.datetof().isEmpty())
        ui->dateto->setDate(current);
    else
        ui->dateto->setDate(QDate::fromString(propertyFilter.datetof(),"dd.MM.yy").addYears(100));

    // время
    if (propertyFilter.timefromf().isEmpty())
        ui->timesince->setTime(QTime::fromString("00:00:00", "hh:mm:ss"));
    else
        ui->timesince->setTime(QTime::fromString(propertyFilter.timefromf(), "hh:mm:ss"));

    if (propertyFilter.timetof().isEmpty())
        ui->timeto->setTime(QTime::fromString("23:59:59", "hh:mm:ss"));
    else
        ui->timeto->setTime(QTime::fromString(propertyFilter.timetof(), "hh:mm:ss"));


    //причины отбоя
    ui->reason->addItem(QString("%1").arg(tr("не задана")), QVariant(0));
    for (auto v : QCDRSortFilterModel::causeValue.keys())
        ui->reason->addItem(QString("%1 - %2").arg(QCDRSortFilterModel::causeValue.value(v).second).arg(v), QVariant(v));
    int index = ui->reason->findData(propertyFilter.releaseCause());
    ui->reason->setCurrentIndex(index < 0 ? 0 : index);


    // типы вызова


    modelType = new QStandardItemModel(Qcallog::TYPE_LAST_CODE, 1);
//  //  ui->type->addItem(QString("%1").arg(tr("не задана")), QVariant(0));

    QStandardItem* item = new QStandardItem(QString("не задана"));
    item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    item->setData(Qt::Unchecked, Qt::CheckStateRole);
    modelType->setItem(0, 0, item);
    for (int i = Qcallog::TYPE_LOCAL; i < Qcallog::TYPE_LAST_CODE; i++) {
        QStandardItem* item = new QStandardItem(QString(Qcallog::getQStringTypeCalls(i)));
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setData(Qt::Unchecked, Qt::CheckStateRole);
        modelType->setItem(i, 0, item);
    }
    ui->type->setModel(modelType);
    ui->type->setItemDelegate(new QStyledItemDelegate());

    connect(modelType, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(type_calls_changed(const QModelIndex&, const QModelIndex&)));

//    ui->type->addItem(QString("%1").arg(tr("не задана")), QVariant(0));
//    for (int i = Qcallog::TYPE_LOCAL; i < Qcallog::TYPE_LAST_CODE; i++)
//        ui->type->addItem(QString(Qcallog::getQStringTypeCalls(i)), QVariant(i));

    QQueue <qint8> q = propertyFilter.typeCalls();
    QStringList txt;
    if (q.empty())
        q << 0;

    for (qint8 i : q) {
        modelType->item(i)->setCheckState(Qt::Checked);
        txt.push_back(modelType->item(i)->text());
    }

    modelType->item(0)->setData(txt.join(','), Qt::DisplayRole);

//    index = ui->type->findData(propertyFilter.typeCalls());

//    modelType->item(index < 0 ? 0 : index)->setCheckState(Qt::Checked);
//    ui->type->setCurrentIndex(index < 0 ? 0 : index);

    highlighted = false;
}

void FilterDialog::type_calls_changed(const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
    Q_UNUSED(bottomRight);

    QStandardItem* item = modelType->item(topLeft.row());

    modelType->blockSignals(true);
    if (topLeft.row() == 0 && item->checkState() == Qt::Checked) { // кликнули на 0
        for (int i = 1; i < modelType->rowCount(); i++) // убрали все checkбоксы
            modelType->item(i)->setCheckState(Qt::Unchecked);
    }
    else
    if (item->checkState() == Qt::Checked) // поставили check, убрали на 0
        modelType->item(0)->setCheckState(Qt::Unchecked);
    else { // сняли check
        int i = 0;
        for (i = 1; i < modelType->rowCount(); i++)
            if (modelType->item(i)->checkState() == Qt::Checked)
                break;
        if (i == modelType->rowCount())
            modelType->item(0)->setCheckState(Qt::Checked);
    }
    modelType->blockSignals(false);
}

FilterDialog::~FilterDialog()
{
    delete ui;
}

void FilterDialog::writefil(PropertyFilter &f)
{
    //f.setAbinf(ui->abin->text());
    f.setAbinf(ui->abin->lineEdit()->text());
    //f.setAboutf(ui->about->text());
    f.setAboutf(ui->about->lineEdit()->text());
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

    QQueue <qint8> q;
    for (int i = 0; i < modelType->rowCount(); i++)
        if (modelType->item(i)->checkState() == Qt::Checked)
            q.push_back(i);

    f.setTypeCalls(q);
}

void FilterDialog::on_clearbutton_clicked()
{
    ui->abin->lineEdit()->clear();
    ui->about->lineEdit()->clear();
    ui->busyfrom->clear();
    ui->busyto->clear();
    ui->talklenfrom->clear();
    ui->talklento->clear();
    ui->inaon->clear();
    ui->innum->clear();
    ui->outaon->clear();
    ui->outnum->clear();
    QDate datesince = QDate::currentDate();
    //ui->datesince->setDate(datesince.fromString("01-01-2000", "dd-MM-yyyy"));
#ifdef QT_DEBUG
    ui->datesince->setDate(datesince.addDays(-datesince.day() + 1).addMonths(-datesince.month() + 1));
#else
    ui->datesince->setDate(datesince.addDays(-datesince.day() + 1));
#endif
    ui->dateto->setDate( QDate::currentDate());
    QTime timesince, timeto;
    ui->timesince->setTime(timesince.fromString("00:00:00", "hh:mm:ss"));
    ui->timeto->setTime(timeto.fromString("23:59:59", "hh:mm:ss"));

    ui->reason->setCurrentIndex(0);

    //ui->type->setCurrentIndex(0);
    modelType->item(0)->setData(QString("не задана"), Qt::DisplayRole);
    modelType->item(0)->setCheckState(Qt::Checked);
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

QQueue<qint8> PropertyFilter::typeCalls() const
{
    return m_typeCalls;
}

void PropertyFilter::setTypeCalls(const QQueue <qint8> &typeCalls)
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

void FilterDialog::on_abin_currentTextChanged(const QString &arg1)
{
    if (highlighted) {
        if (!arg1.isEmpty())
            if (!addr1.contains(arg1))
                addr1.append(QString("%1%2").arg(addr1.isEmpty() ? "" : addr1.right(1) == "," ? "" : ",").arg(arg1));
        ui->abin->lineEdit()->blockSignals(true);
        ui->abin->lineEdit()->setText(addr1);
        ui->abin->lineEdit()->blockSignals(false);
    } else
        addr1 = arg1;
    highlighted = false;

}

void FilterDialog::on_abin_highlighted(const QString &arg1)
{
    Q_UNUSED(arg1);

    highlighted = true;
}

void FilterDialog::on_about_currentTextChanged(const QString &arg1)
{
    if (highlighted) {
        if (!arg1.isEmpty())
            if (!addr2.contains(arg1))
                addr2.append(QString("%1%2").arg(addr2.isEmpty() ? "" : addr2.right(1) == "," ? "" : ",").arg(arg1));
        ui->about->lineEdit()->blockSignals(true);
        ui->about->lineEdit()->setText(addr2);
        ui->about->lineEdit()->blockSignals(false);
    } else
        addr2 = arg1;
    highlighted = false;
}

void FilterDialog::on_about_highlighted(const QString &arg1)
{
    Q_UNUSED(arg1);
    highlighted = true;
}

//void FilterDialog::on_type_currentIndexChanged(int index)
//{
//    qDebug() << "Calls Type Index Changed";
//}

//void FilterDialog::on_type_highlighted(int index)
//{
//    qDebug() << "Calls Type highlighted";
//}
