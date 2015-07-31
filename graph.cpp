#include "graph.h"
#include "ui_graph.h"
#include <QDebug>
#include <QMap>
#define customPlot ui->Plot

Graph::Graph(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Graph)
{
    ui->setupUi(this);

#ifdef Q_OS_WIN
    setWindowFlags(windowFlags() | Qt::CustomizeWindowHint | Qt::WindowMinMaxButtonsHint);
#endif

}

Graph::~Graph()
{
    delete ui;
}

void Graph::buildReportReleaseCause(QSqlTableModel *cdrModel)
{
    setWindowTitle(tr("Причины отбоя"));
    cdrModel->select();
    QCustomPlot* plot = ui->Plot;
    int idIndex = cdrModel->fieldIndex("relreason");
    int m[256];

    int columnum = 0;
    memset(m, 0, sizeof(m));
    while (cdrModel->canFetchMore())
        cdrModel->fetchMore();

    for (int i = 0; i < cdrModel->rowCount(); i++)
        m[cdrModel->data(cdrModel->index(i, idIndex), Qt::EditRole).toInt()]++;

    QCPBars *bar = new QCPBars(plot->xAxis, plot->yAxis);
    int ymax = 0;
    QVector<double> data;
    QVector<double> ticks;
    QVector<QString> labels;
    plot->addPlottable(bar);
    for (size_t relindex = 0; relindex < sizeof(m) / sizeof(int); relindex++) {
        if(m[relindex]) {
             columnum++;
             ticks << columnum;
             //qDebug() << relindex << "=" << m[relindex] << columnum;
             data << m[relindex];
             labels << QString("%1").arg(relindex);
             if(ymax < m[relindex])
                 ymax = m[relindex];
        }
    }
    bar->setData(ticks, data);

    customPlot->xAxis->setLabel(tr("Причины отбоя"));
    customPlot->xAxis->setAutoTicks(false);
    customPlot->xAxis->setAutoTickLabels(false);
    customPlot->xAxis->setTickVector(ticks);
    customPlot->xAxis->setTickVectorLabels(labels);
    customPlot->xAxis->setTickLabelRotation(60);
    customPlot->xAxis->setSubTickCount(0);
    customPlot->xAxis->setTickLength(0, 4);
    customPlot->xAxis->grid()->setVisible(true);
    customPlot->xAxis->setRange(0, columnum + 1);

    // prepare y axis:
    customPlot->yAxis->setRange(0, ymax + ymax / 10);
    customPlot->yAxis->setPadding(5); // a bit more space to the left border
    customPlot->yAxis->setLabel(tr("Количество вызовов"));
    customPlot->yAxis->grid()->setSubGridVisible(true);

    QPen gridPen;
    gridPen.setStyle(Qt::SolidLine);
    gridPen.setColor(QColor(0, 0, 0, 25));
    customPlot->yAxis->grid()->setPen(gridPen);
    gridPen.setStyle(Qt::DotLine);
    customPlot->yAxis->grid()->setSubGridPen(gridPen);


    //qDebug() << columnum;

    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    /*QCPBars *regen = new QCPBars(customPlot->xAxis, customPlot->yAxis);
    QCPBars *nuclear = new QCPBars(customPlot->xAxis, customPlot->yAxis);
    QCPBars *fossil = new QCPBars(customPlot->xAxis, customPlot->yAxis);
    customPlot->addPlottable(regen);
    customPlot->addPlottable(nuclear);
    customPlot->addPlottable(fossil);
    // set names and colors:
    QPen pen;
    pen.setWidthF(1.2);
    fossil->setName("Fossil fuels");
    pen.setColor(QColor(255, 131, 0));
    fossil->setPen(pen);
    fossil->setBrush(QColor(255, 131, 0, 50));
    nuclear->setName("Nuclear");
    pen.setColor(QColor(1, 92, 191));
    nuclear->setPen(pen);
    nuclear->setBrush(QColor(1, 92, 191, 50));
    regen->setName("Regenerative");
    pen.setColor(QColor(150, 222, 0));
    regen->setPen(pen);
    regen->setBrush(QColor(150, 222, 0, 70));
    // stack bars ontop of each other:
    nuclear->moveAbove(fossil);
    regen->moveAbove(nuclear);

    // prepare x axis with country labels:
    QVector<double> ticks;
    QVector<QString> labels;
    ticks << 1 << 2 << 3 << 4 << 5 << 6 << 7;
    labels << "USA" << "Japan" << "Germany" << "France" << "UK" << "Italy" << "Canada";
    customPlot->xAxis->setAutoTicks(false);
    customPlot->xAxis->setAutoTickLabels(false);
    customPlot->xAxis->setTickVector(ticks);
    customPlot->xAxis->setTickVectorLabels(labels);
    customPlot->xAxis->setTickLabelRotation(60);
    customPlot->xAxis->setSubTickCount(0);
    customPlot->xAxis->setTickLength(0, 4);
    customPlot->xAxis->grid()->setVisible(true);
    customPlot->xAxis->setRange(0, 8);

    // prepare y axis:
    customPlot->yAxis->setRange(0, 12.1);
    customPlot->yAxis->setPadding(5); // a bit more space to the left border
    customPlot->yAxis->setLabel("Power Consumption in\nKilowatts per Capita (2007)");
    customPlot->yAxis->grid()->setSubGridVisible(true);
    QPen gridPen;
    gridPen.setStyle(Qt::SolidLine);
    gridPen.setColor(QColor(0, 0, 0, 25));
    customPlot->yAxis->grid()->setPen(gridPen);
    gridPen.setStyle(Qt::DotLine);
    customPlot->yAxis->grid()->setSubGridPen(gridPen);

    // Add data:
    QVector<double> fossilData, nuclearData, regenData;
    fossilData  << 0.86*10.5 << 0.83*5.5 << 0.84*5.5 << 0.52*5.8 << 0.89*5.2 << 0.90*4.2 << 0.67*11.2;
    nuclearData << 0.08*10.5 << 0.12*5.5 << 0.12*5.5 << 0.40*5.8 << 0.09*5.2 << 0.00*4.2 << 0.07*11.2;
    regenData   << 0.06*10.5 << 0.05*5.5 << 0.04*5.5 << 0.06*5.8 << 0.02*5.2 << 0.07*4.2 << 0.25*11.2;
    fossil->setData(ticks, fossilData);
    nuclear->setData(ticks, nuclearData);
    regen->setData(ticks, regenData);

    // setup legend:
    customPlot->legend->setVisible(true);
    customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
    customPlot->legend->setBrush(QColor(255, 255, 255, 200));
    QPen legendPen;
    legendPen.setColor(QColor(130, 130, 130, 200));
    customPlot->legend->setBorderPen(legendPen);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    customPlot->legend->setFont(legendFont);
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);*/
    show();
}

void Graph::buildReportSucessCallsDate(QSqlTableModel *cdrModel)
{
    //QString title = "Успешность вызовов за" + date.toString(" MMM yyyy"); // тут можно распечатать параметры выборки
    //setWindowTitle(title);
    QCustomPlot* plot = ui->Plot;
    QMap <QDate,int> sucals;
    QMap <QDate,int> othercals;
    int reasIndex = cdrModel->fieldIndex("relreason");
    int dateIndex = cdrModel->fieldIndex("date");
    QCPBars *sucbar = new QCPBars(plot->xAxis, plot->yAxis);
    QCPBars *otherbar = new QCPBars(plot->xAxis, plot->yAxis);
    cdrModel->select();
    int ymax = 0;

    while (cdrModel->canFetchMore())
        cdrModel->fetchMore();
    for (int i = 0; i < cdrModel->rowCount(); i++) {
        QDate index = QDate::fromJulianDay(cdrModel->data(cdrModel->index(i, dateIndex), Qt::EditRole).toInt());
        if (!sucals.contains(index)) { // если кулюча нет, добавляем
            sucals[index] = 0;
            othercals[index] = 0;
        }

        cdrModel->data(cdrModel->index(i, reasIndex), Qt::EditRole).toInt() == 16  ? sucals[index]++ : othercals[index]++;
    }

    QVector<double> sucdata;
    QVector<double> otherdata;
    QVector<double> ticks;
    QVector<QString> labels;
    plot->addPlottable(sucbar);
    plot->addPlottable(otherbar);
    int tickscount = 0;

    for (auto val : sucals.toStdMap()) {
        sucdata << val.second;
        ticks << ++tickscount;
        labels << val.first.toString("dd.MM.yyyy");
    }

    for (auto val : othercals.toStdMap()) {
        otherdata << val.second;
    }

    for (auto v : sucals.keys()) {
        if (sucals[v] + othercals[v] > ymax)
            ymax = sucals[v] + othercals[v];
    }

    otherbar->setData(ticks, otherdata);
    sucbar->setData(ticks, sucdata);
//раскраска столбцов
    otherbar->moveAbove(sucbar);
    QPen pen;
    pen.setWidthF(1.2);
    sucbar->setName(tr("Успешные вызовы"));
    pen.setColor(QColor(1, 92, 191));
    sucbar->setPen(pen);
    sucbar->setBrush(QColor(1, 92, 191, 100));
    otherbar->setName("Все вызовы");
    pen.setColor(QColor(255, 131, 0));
    otherbar->setPen(pen);
    otherbar->setBrush(QColor(255, 131, 0, 100));

    customPlot->xAxis->setLabel(tr("Число"));
    customPlot->xAxis->setAutoTicks(false);
    customPlot->xAxis->setAutoTickLabels(false);
    customPlot->xAxis->setTickVector(ticks);
    customPlot->xAxis->setTickVectorLabels(labels);
    customPlot->xAxis->setTickLabelRotation(60);
    customPlot->xAxis->setSubTickCount(0);
    customPlot->xAxis->setTickLength(0, 4);
    customPlot->xAxis->grid()->setVisible(true);
    customPlot->xAxis->setRange(0, otherdata.size() + 1);

    // prepare y axis:
    customPlot->yAxis->setRange(0, ymax + ymax / 10);
    customPlot->yAxis->setPadding(5);
    customPlot->yAxis->setLabel(tr("Количество вызовов в день"));
    customPlot->yAxis->grid()->setSubGridVisible(true);
    QPen gridPen;
    gridPen.setStyle(Qt::SolidLine);
    gridPen.setColor(QColor(0, 0, 0, 25));
    customPlot->yAxis->grid()->setPen(gridPen);
    gridPen.setStyle(Qt::DotLine);
    customPlot->yAxis->grid()->setSubGridPen(gridPen);

    //легенда
    customPlot->legend->setVisible(true);
    customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop | Qt::AlignRight);
    customPlot->legend->setBrush(QColor(255, 255, 255, 200));
    QPen legendPen;
    legendPen.setColor(QColor(130, 130, 130, 200));
    customPlot->legend->setBorderPen(legendPen);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    customPlot->legend->setFont(legendFont);

    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    show();
}

void Graph::buildReportSucessCallsTime(QSqlTableModel *cdrModel)
{
    /*Не стал делать график слошной линни, так как для более-менее адекватного отображения с точностью хотя бы до минут нужно ~ несколько сотен
     более-менее равномерных вызовов, иначе график будет выглядеть как горная панорама, постояяно падая до нулевых отметок.
     Если нужен именно вид линии - нужно уменьшать точность, пока сделал столбиками
     */
    setWindowTitle(tr("Успешность вызовов по часам"));
    cdrModel->select();
    QCustomPlot* plot = ui->Plot;
    int reasIndex = cdrModel->fieldIndex("relreason");
    int timeIndex = cdrModel->fieldIndex("time");
    int suchours[24];
    int unsuchours[24];
    memset(suchours, 0, sizeof(suchours));
    memset(unsuchours, 0, sizeof(unsuchours));

    while (cdrModel->canFetchMore())
        cdrModel->fetchMore();

    for (int i = 0; i < cdrModel->rowCount(); i++){
        if(cdrModel->data(cdrModel->index(i, reasIndex), Qt::EditRole).toInt() == 16){
            //В базе лежат секунды с начала дня, а методу нужно на вход количество милисекунд, поэтому * 1000
            suchours[QTime::fromMSecsSinceStartOfDay(cdrModel->data(cdrModel->index(i, timeIndex), Qt::EditRole).toInt() * 1000).hour() - 1]++;
        }
        else{
            unsuchours[QTime::fromMSecsSinceStartOfDay(cdrModel->data(cdrModel->index(i, timeIndex), Qt::EditRole).toInt() * 1000).hour() - 1]++;
        }
    }
    QCPBars *sucbar = new QCPBars(plot->xAxis, plot->yAxis);
    QCPBars *otherbar = new QCPBars(plot->xAxis, plot->yAxis);

    int ymax = 0;
    QVector<double> sucdata;
    QVector<double> unsucdata;
    QVector<double> ticks;
    QVector<QString> labels;
    plot->addPlottable(sucbar);
    plot->addPlottable(otherbar);
    for(int i = 0; i < 24; i++){
        ticks << i + 1;
        sucdata << suchours[i];
        unsucdata << unsuchours[i];
        if(ymax < suchours[i] + unsuchours[i])
            ymax = suchours[i] + unsuchours[i];
    }
    labels << "0:00"<<"1:00"<<"2:00"<<"3:00"<<"4:00"<<"5:00"<<"6:00"<<"7:00"<<"8:00"<<"9:00"<<"10:00"<<"11:00"<<"12:00"<<"13:00"<<"14:00"<<"15:00"<<"16:00"<<"17:00"<<"18:00"<<"19:00"<<"20:00"<<"21:00"<<"22:00"<<"23:00";

    otherbar->moveAbove(sucbar);
    QPen pen;
    pen.setWidthF(1.2);
    sucbar->setName(tr("Успешные вызовы"));
    pen.setColor(QColor(1, 92, 191));
    sucbar->setPen(pen);
    sucbar->setBrush(QColor(1, 92, 191, 100));
    otherbar->setName("Все вызовы");
    pen.setColor(QColor(255, 131, 0));
    otherbar->setPen(pen);
    otherbar->setBrush(QColor(255, 131, 0, 100));
    sucbar->setData(ticks, sucdata);
    otherbar->setData(ticks, unsucdata);

    customPlot->xAxis->setLabel(tr("Время суток"));
    customPlot->xAxis->setAutoTicks(false);
    customPlot->xAxis->setAutoTickLabels(false);
    customPlot->xAxis->setTickVector(ticks);
    customPlot->xAxis->setTickVectorLabels(labels);
    customPlot->xAxis->setTickLabelRotation(60);
    customPlot->xAxis->setSubTickCount(0);
    customPlot->xAxis->setTickLength(0, 4);
    customPlot->xAxis->grid()->setVisible(true);
    customPlot->xAxis->setRange(0, 25);

    // prepare y axis:
    customPlot->yAxis->setRange(0, ymax + ymax / 10);
    customPlot->yAxis->setPadding(5); // a bit more space to the left border
    customPlot->yAxis->setLabel(tr("Количество вызовов"));
    customPlot->yAxis->grid()->setSubGridVisible(true);

    customPlot->legend->setVisible(true);
    customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop | Qt::AlignRight);
    customPlot->legend->setBrush(QColor(255, 255, 255, 200));
    QPen legendPen;
    legendPen.setColor(QColor(130, 130, 130, 200));
    customPlot->legend->setBorderPen(legendPen);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    customPlot->legend->setFont(legendFont);

    //qDebug() << suchours[0];

    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    show();
}

void Graph::buildReportSucessCallsWeekDay(QSqlTableModel *cdrModel)
{
    setWindowTitle(tr("Успешность вызовов по дням недели"));
    cdrModel->select();
    QCustomPlot* plot = ui->Plot;
    const int dayOfWeek = 7;
    int reasIndex = cdrModel->fieldIndex("relreason");
    int dateIndex = cdrModel->fieldIndex("date");
    int sucdays[dayOfWeek]; //QMap не имеет смысла, так как .dayOfWeek() возвращает int, то есть целое служило бы ключом к целому
    int unsucdays[dayOfWeek];
    memset(sucdays, 0, sizeof(sucdays));
    memset(unsucdays, 0, sizeof(unsucdays));

    while (cdrModel->canFetchMore())
        cdrModel->fetchMore();

    for (int i = 0; i < cdrModel->rowCount(); i++) {
        int index = QDate::fromJulianDay(cdrModel->data(cdrModel->index(i, dateIndex), Qt::EditRole).toInt()).dayOfWeek() - 1;
        cdrModel->data(cdrModel->index(i, reasIndex), Qt::EditRole).toInt() == 16 ? sucdays[index]++ : unsucdays[index]++;
    }

    QCPBars *sucbar = new QCPBars(plot->xAxis, plot->yAxis);
    QCPBars *otherbar = new QCPBars(plot->xAxis, plot->yAxis);

    int ymax = 0;
    QVector<double> sucdata;
    QVector<double> unsucdata;
    QVector<double> ticks;
    QVector<QString> labels;
    plot->addPlottable(sucbar);
    plot->addPlottable(otherbar);
    for(int i = 0; i < dayOfWeek; i++) {
        ticks << i + 1;
        sucdata << sucdays[i];
        unsucdata << unsucdays[i];
        if(ymax < sucdays[i] + unsucdays[i])
            ymax = sucdays[i] + unsucdays[i];
    }
    labels <<  "Понедельник" << "Вторник" << "Среда" << "Четверг" << "Пятница" << "Суббота" << "Воскресенье";

    otherbar->moveAbove(sucbar);
    QPen pen;
    pen.setWidthF(1.2);
    sucbar->setName(tr("Успешные вызовы"));
    pen.setColor(QColor(1, 92, 191));
    sucbar->setPen(pen);
    sucbar->setBrush(QColor(1, 92, 191, 100));
    otherbar->setName("Все вызовы");
    pen.setColor(QColor(255, 131, 0));
    otherbar->setPen(pen);
    otherbar->setBrush(QColor(255, 131, 0, 100));
    sucbar->setData(ticks, sucdata);
    otherbar->setData(ticks, unsucdata);

    customPlot->xAxis->setLabel(tr("День недели"));
    customPlot->xAxis->setAutoTicks(false);
    customPlot->xAxis->setAutoTickLabels(false);
    customPlot->xAxis->setTickVector(ticks);
    customPlot->xAxis->setTickVectorLabels(labels);
    customPlot->xAxis->setTickLabelRotation(60);
    customPlot->xAxis->setSubTickCount(0);
    customPlot->xAxis->setTickLength(0, 4);
    customPlot->xAxis->grid()->setVisible(true);
    customPlot->xAxis->setRange(0, 8);

    // prepare y axis:
    customPlot->yAxis->setRange(0, ymax + ymax / 10);
    customPlot->yAxis->setPadding(5); // a bit more space to the left border
    customPlot->yAxis->setLabel(tr("Количество вызовов"));
    customPlot->yAxis->grid()->setSubGridVisible(true);

    customPlot->legend->setVisible(true);
    customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop | Qt::AlignRight);
    customPlot->legend->setBrush(QColor(255, 255, 255, 200));
    QPen legendPen;
    legendPen.setColor(QColor(130, 130, 130, 200));
    customPlot->legend->setBorderPen(legendPen);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    customPlot->legend->setFont(legendFont);

    //qDebug() << columnum;

    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    show();
}

void Graph::buildReportAbonents(QSqlTableModel *cdrModel){
    QCustomPlot* plot = ui->Plot;
    QMap <QString,int> sucals;
    QMap <QString,int> othercals;
    int reasIndex = cdrModel->fieldIndex("relreason");
    int intypeIndex = cdrModel->fieldIndex("intype");
    int ininc1Index = cdrModel->fieldIndex("ininc1");
    int ininc2Index = cdrModel->fieldIndex("ininc2");
    int ininc3Index = cdrModel->fieldIndex("ininc3");

    QCPBars *sucbar = new QCPBars(plot->xAxis, plot->yAxis);
    QCPBars *otherbar = new QCPBars(plot->xAxis, plot->yAxis);
    cdrModel->select();
    int ymax = 0;

    while (cdrModel->canFetchMore())
        cdrModel->fetchMore();
    for (int i = 0; i < cdrModel->rowCount(); i++) {
        QString index = cdrModel->data(cdrModel->index(i, intypeIndex), Qt::EditRole).toString() + cdrModel->data(cdrModel->index(i, ininc1Index), Qt::EditRole).toString();
        if(cdrModel->data(cdrModel->index(i, ininc2Index), Qt::EditRole).toString() != "0")
        {
            index += cdrModel->data(cdrModel->index(i, ininc2Index), Qt::EditRole).toString() + cdrModel->data(cdrModel->index(i, ininc3Index), Qt::EditRole).toString();
        }
        if (!sucals.contains(index)) { // если ключа нет, добавляем
            sucals[index] = 0;
            othercals[index] = 0;
        }

        cdrModel->data(cdrModel->index(i, reasIndex), Qt::EditRole).toInt() == 16  ? sucals[index]++ : othercals[index]++;
    }

    QVector<double> sucdata;
    QVector<double> otherdata;
    QVector<double> ticks;
    QVector<QString> labels;
    plot->addPlottable(sucbar);
    plot->addPlottable(otherbar);
    int tickscount = 0;

    for (auto val : sucals.toStdMap()) {
        sucdata << val.second;
        ticks << ++tickscount;
        labels << val.first;
    }

    for (auto val : othercals.toStdMap()) {
        otherdata << val.second;
    }

    for (auto v : sucals.keys()) {
        if (sucals[v] + othercals[v] > ymax)
            ymax = sucals[v] + othercals[v];
    }

    otherbar->setData(ticks, otherdata);
    sucbar->setData(ticks, sucdata);
//раскраска столбцов
    otherbar->moveAbove(sucbar);
    QPen pen;
    pen.setWidthF(1.2);
    sucbar->setName(tr("Успешные вызовы"));
    pen.setColor(QColor(1, 92, 191));
    sucbar->setPen(pen);
    sucbar->setBrush(QColor(1, 92, 191, 100));
    otherbar->setName("Все вызовы");
    pen.setColor(QColor(255, 131, 0));
    otherbar->setPen(pen);
    otherbar->setBrush(QColor(255, 131, 0, 100));

    customPlot->xAxis->setLabel(tr("Абоненты или линии"));
    customPlot->xAxis->setAutoTicks(false);
    customPlot->xAxis->setAutoTickLabels(false);
    customPlot->xAxis->setTickVector(ticks);
    customPlot->xAxis->setTickVectorLabels(labels);
    customPlot->xAxis->setTickLabelRotation(60);
    customPlot->xAxis->setSubTickCount(0);
    customPlot->xAxis->setTickLength(0, 4);
    customPlot->xAxis->grid()->setVisible(true);
    customPlot->xAxis->setRange(0, otherdata.size() + 2);

    // prepare y axis:
    customPlot->yAxis->setRange(0, ymax + ymax / 10);
    customPlot->yAxis->setPadding(5);
    customPlot->yAxis->setLabel(tr("Количество вызовов в день"));
    customPlot->yAxis->grid()->setSubGridVisible(true);
    QPen gridPen;
    gridPen.setStyle(Qt::SolidLine);
    gridPen.setColor(QColor(0, 0, 0, 25));
    customPlot->yAxis->grid()->setPen(gridPen);
    gridPen.setStyle(Qt::DotLine);
    customPlot->yAxis->grid()->setSubGridPen(gridPen);

    //легенда
    customPlot->legend->setVisible(true);
    customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop | Qt::AlignRight);
    customPlot->legend->setBrush(QColor(255, 255, 255, 200));
    QPen legendPen;
    legendPen.setColor(QColor(130, 130, 130, 200));
    customPlot->legend->setBorderPen(legendPen);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    customPlot->legend->setFont(legendFont);

    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    show();
}
