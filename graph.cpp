#include "graph.h"
#include "ui_graph.h"
#include <QDebug>
#include <QMap>
#include <QDateTime>
#include <QAction>
#define customPlot ui->Plot

Graph::Graph(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Graph)
{
    ui->setupUi(this);

#ifdef Q_OS_WIN
    setWindowFlags(windowFlags() | Qt::CustomizeWindowHint | Qt::WindowMinMaxButtonsHint);
#endif
//    ui->Plot->addAction(new QAction(QObject::tr("Сохранить"),ui->Plot));
//    ui->Plot->setContextMenuPolicy(Qt::ActionsContextMenu);
    QAction *save;
    save = new QAction("Сохранить", ui->Plot);
    ui->Plot->addAction(save);
    ui->Plot->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->Plot->connect(save, SIGNAL(triggered()), this, SLOT(save()));
}

Graph::~Graph()
{
    delete ui;
}

void Graph::save(void){
//    QPixmap pixmap(ui->Plot->size());
//    ui->Plot->render(&pixmap);
//    pixmap.save("test.png");
    QPixmap pixmap(ui->Plot->size());
    QPainter pixPainter(&pixmap);
    ui->Plot->render(&pixPainter);
    //pixmap.save("savetest.png", "PNG"); // writes pixmap into bytes in PNG format
    QString format = "png";
    QString initialPath = QDir::currentPath() + tr("/untitled.") + format;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                                   initialPath,
                                   tr("%1 Files (*.%2);;All Files (*)")
                                   .arg(format.toUpper())
                                   .arg(format));
    if (!fileName.isEmpty())
            pixmap.save(fileName, "PNG");
    //qDebug() << "saved" << ui->Plot->size() << pixmap;
}

void Graph::buildReportReleaseCause(QSqlTableModel *cdrModel)
{
    setWindowTitle(tr("Причины отбоя"));
    //cdrModel->select();
    QCustomPlot* plot = ui->Plot;
    //int idIndex = cdrModel->fieldIndex("relreason");
    int m[256];

    int columnum = 0;
    memset(m, 0, sizeof(m));

    QSqlQuery q;
    q.setForwardOnly(true);
    if (!q.exec(QString("SELECT relreason FROM logbase %1").arg(cdrModel->filter().isEmpty() ? "" : QString(" WHERE %1").arg(cdrModel->filter()))))
        qDebug() << "ReleaseCause select error" << q.lastError();

    //for (int i = 0; i < cdrModel->rowCount(); i++)
    //    m[cdrModel->data(cdrModel->index(i, idIndex), Qt::EditRole).toInt()]++;
    while (q.next())
        m[q.value(0).toInt()]++;

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
    //int reasIndex = cdrModel->fieldIndex("relreason");
    //int dateIndex = cdrModel->fieldIndex("date");
    QCPBars *sucbar = new QCPBars(plot->xAxis, plot->yAxis);
    QCPBars *otherbar = new QCPBars(plot->xAxis, plot->yAxis);
//    cdrModel->select();
    int ymax = 0;

//    while (cdrModel->canFetchMore())
//        cdrModel->fetchMore();

    QSqlQuery q;
    q.setForwardOnly(true);
    if (!q.exec(QString("SELECT relreason, date FROM logbase %1").arg(cdrModel->filter().isEmpty() ? "" : QString(" WHERE %1").arg(cdrModel->filter()))))
        qDebug() << "ReleaseCause select error" << q.lastError();


    while (q.next()) {
        QDate index = QDate::fromJulianDay(q.value(1).toInt());
        if (!sucals.contains(index)) { // если ключа нет, добавляем
            sucals[index] = 0;
            othercals[index] = 0;
        }
        q.value(0).toInt() == 16  ? sucals[index]++ : othercals[index]++;
    }

//    for (int i = 0; i < cdrModel->rowCount(); i++) {
//        QDate index = QDate::fromJulianDay(cdrModel->data(cdrModel->index(i, dateIndex), Qt::EditRole).toInt());
//        if (!sucals.contains(index)) { // если ключа нет, добавляем
//            sucals[index] = 0;
//            othercals[index] = 0;
//        }

//        cdrModel->data(cdrModel->index(i, reasIndex), Qt::EditRole).toInt() == 16  ? sucals[index]++ : othercals[index]++;
//    }

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
//    cdrModel->select();
    QCustomPlot* plot = ui->Plot;
    //int reasIndex = cdrModel->fieldIndex("relreason");
    //int timeIndex = cdrModel->fieldIndex("time");
    int suchours[24];
    int unsuchours[24];
    memset(suchours, 0, sizeof(suchours));
    memset(unsuchours, 0, sizeof(unsuchours));

//    while (cdrModel->canFetchMore())
//        cdrModel->fetchMore();

    QSqlQuery q;
    q.setForwardOnly(true);
    if (!q.exec(QString("SELECT relreason, time FROM logbase %1").arg(cdrModel->filter().isEmpty() ? "" : QString(" WHERE %1").arg(cdrModel->filter()))))
        qDebug() << "ReleaseCause select error" << q.lastError();


    while (q.next()) {
        if (q.value(0).toInt() == 16)
            suchours[QTime::fromMSecsSinceStartOfDay(q.value(1).toInt() * 1000).hour()]++;
        else
            unsuchours[QTime::fromMSecsSinceStartOfDay(q.value(1).toInt() * 1000).hour()]++;
    }

//    for (int i = 0; i < cdrModel->rowCount(); i++){
//        if(cdrModel->data(cdrModel->index(i, reasIndex), Qt::EditRole).toInt() == 16){
//            //В базе лежат секунды с начала дня, а методу нужно на вход количество милисекунд, поэтому * 1000
//            suchours[QTime::fromMSecsSinceStartOfDay(cdrModel->data(cdrModel->index(i, timeIndex), Qt::EditRole).toInt() * 1000).hour()]++;
//        }
//        else {
//            unsuchours[QTime::fromMSecsSinceStartOfDay(cdrModel->data(cdrModel->index(i, timeIndex), Qt::EditRole).toInt() * 1000).hour()]++;
//        }
//    }
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
//    cdrModel->select();
    QCustomPlot* plot = ui->Plot;
    const int dayOfWeek = 7;
    //int reasIndex = cdrModel->fieldIndex("relreason");
    //int dateIndex = cdrModel->fieldIndex("date");
    int sucdays[dayOfWeek]; //QMap не имеет смысла, так как .dayOfWeek() возвращает int, то есть целое служило бы ключом к целому
    int unsucdays[dayOfWeek];
    memset(sucdays, 0, sizeof(sucdays));
    memset(unsucdays, 0, sizeof(unsucdays));

//    while (cdrModel->canFetchMore())
//        cdrModel->fetchMore();

    QSqlQuery q;
    q.setForwardOnly(true);
    if (!q.exec(QString("SELECT relreason, date FROM logbase %1").arg(cdrModel->filter().isEmpty() ? "" : QString(" WHERE %1").arg(cdrModel->filter()))))
        qDebug() << "ReleaseCause select error" << q.lastError();


    while (q.next()) {
        int index = QDate::fromJulianDay(q.value(1).toInt()).dayOfWeek() - 1;
        q.value(0) == 16 ? sucdays[index]++ : unsucdays[index]++;
    }

//    for (int i = 0; i < cdrModel->rowCount(); i++) {
//        int index = QDate::fromJulianDay(cdrModel->data(cdrModel->index(i, dateIndex), Qt::EditRole).toInt()).dayOfWeek() - 1;
//        cdrModel->data(cdrModel->index(i, reasIndex), Qt::EditRole).toInt() == 16 ? sucdays[index]++ : unsucdays[index]++;
//    }

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
    //Успешность вызовов по абонентам
    QCustomPlot* plot = ui->Plot;
    QMap <QString,int> sucals;
    QMap <QString,int> othercals;
    int reasIndex = cdrModel->fieldIndex("relreason");
    int intypeIndex = cdrModel->fieldIndex("intype");

    QCPBars *sucbar = new QCPBars(plot->xAxis, plot->yAxis);
    QCPBars *otherbar = new QCPBars(plot->xAxis, plot->yAxis);
    cdrModel->select();
    int ymax = 0;
    while (cdrModel->canFetchMore())
        cdrModel->fetchMore();


    for (int i = 0; i < cdrModel->rowCount(); i++) {
        QString index = cdrModel->data(cdrModel->index(i, intypeIndex), Qt::DisplayRole).toString();// + cdrModel->data(cdrModel->index(i, ininc1Index), Qt::EditRole).toString();
        if (!sucals.contains(index)) {
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
    customPlot->yAxis->setRange(0, ymax + ymax / 5);
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

void Graph::buildReportLoad(QSqlTableModel *cdrModel){
    setWindowTitle(tr("Нагрузка"));
    cdrModel->select();
    int timeIndex = cdrModel->fieldIndex("time");
    int dateIndex = cdrModel->fieldIndex("date");
    int lenIndex = cdrModel->fieldIndex("linelen");
    QDateTime dateto = QDateTime::fromString("01-01-1980 00:00:00", "dd-MM-yyyy hh:mm:ss");
    QDateTime datefrom = QDateTime::fromString("01-01-3000 00:00:00", "dd-MM-yyyy hh:mm:ss");
    //QDateTime rowdate;
    //QTime rowtime;
    double sumlinelen = 0;
    QCustomPlot* plot = ui->Plot;
    while (cdrModel->canFetchMore())
        cdrModel->fetchMore();
    int rowCount = cdrModel->rowCount();

    //QDateTime datefrom(QDate::fromJulianDay(cdrModel->data(cdrModel->index(0, dateIndex), Qt::EditRole).toInt()), QTime::fromMSecsSinceStartOfDay(cdrModel->data(cdrModel->index(0, timeIndex), Qt::EditRole).toInt() * 1000));
    //QDateTime dateto(QDate::fromJulianDay(cdrModel->data(cdrModel->index(rowCount - 1, dateIndex), Qt::EditRole).toInt()), QTime::fromMSecsSinceStartOfDay(cdrModel->data(cdrModel->index(rowCount - 1, timeIndex), Qt::EditRole).toInt() * 1000));
    for (int i = 0; i < rowCount; i++) {
        sumlinelen += cdrModel->data(cdrModel->index(i, lenIndex), Qt::EditRole).toInt();
        QDateTime rowdate(QDate::fromJulianDay(cdrModel->data(cdrModel->index(i, dateIndex), Qt::EditRole).toInt()), QTime::fromMSecsSinceStartOfDay(cdrModel->data(cdrModel->index(i, timeIndex), Qt::EditRole).toInt() * 1000));
        //rowtime = QTime::fromMSecsSinceStartOfDay(cdrModel->data(cdrModel->index(0, timeIndex), Qt::EditRole).toInt() * 1000);
        if(dateto < rowdate){
             dateto = rowdate;
        }
        if (datefrom > rowdate){
            datefrom = rowdate;
        }
    }
    //qDebug() << datefrom.toString("dd-MM-yyyy hh:mm:ss") << dateto;
     QCPBars *loadbar = new QCPBars(plot->xAxis, plot->yAxis);
    QVector<double> loaddata;
    QVector<double> ticks;
    ticks << 1 << 2;
    loaddata <<  sumlinelen / datefrom.msecsTo(dateto) * 1000;
    //loaddata <<  datefrom.msecsTo(dateto) * 1000 / sumlinelen;
    plot->addPlottable(loadbar);
    loadbar->setData(ticks, loaddata);

    //customPlot->xAxis->setLabel(tr("Нагрузка в эрлангах"));
    customPlot->xAxis->setAutoTicks(false);
    customPlot->xAxis->setAutoTickLabels(false);
    customPlot->xAxis->setTickVector(ticks);
    //customPlot->xAxis->setTickVectorLabels(labels);
    customPlot->xAxis->setTickLabelRotation(60);
    customPlot->xAxis->setSubTickCount(0);
    customPlot->xAxis->setTickLength(0, 4);
    customPlot->xAxis->grid()->setVisible(true);
    customPlot->xAxis->setRange(0, 2);

    // prepare y axis:
    customPlot->yAxis->setRange(0, loaddata[0] + loaddata[0] / 10);
    customPlot->yAxis->setPadding(5);
    customPlot->yAxis->setLabel(tr("Нагрузка в эрлангах"));
    customPlot->yAxis->grid()->setSubGridVisible(true);

    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    show();
}

void Graph::buildReportCallens(QSqlTableModel *cdrModel){
    //Длительность разгововров и занятия линии абонентами

    QCustomPlot* plot = ui->Plot;
    QMap <QString,int> callen;
    QMap <QString,int> linelen;
    int lineLenIndex = cdrModel->fieldIndex("linelen");
    int calLenIndex = cdrModel->fieldIndex("callen");
    int intypeIndex = cdrModel->fieldIndex("intype");
    int dateIndex = cdrModel->fieldIndex("date");
    int maxdate = 0;
    qint64 mindate = 3000000;

    QCPBars *callenbar = new QCPBars(plot->xAxis, plot->yAxis);
    QCPBars *linelenbar = new QCPBars(plot->xAxis, plot->yAxis);
    cdrModel->select();
    int ymax = 0;
    while (cdrModel->canFetchMore())
        cdrModel->fetchMore();


    for (int i = 0; i < cdrModel->rowCount(); i++) {
        QString index = cdrModel->data(cdrModel->index(i, intypeIndex), Qt::DisplayRole).toString();// + cdrModel->data(cdrModel->index(i, ininc1Index), Qt::EditRole).toString();
        if (!callen.contains(index)) {
            callen[index] = 0;
            linelen[index] = 0;
        }
        int rowcal = cdrModel->data(cdrModel->index(i, calLenIndex), Qt::EditRole).toInt();
        int rowline = cdrModel->data(cdrModel->index(i, lineLenIndex), Qt::EditRole).toInt();
        int rowdate = cdrModel->data(cdrModel->index(i, dateIndex), Qt::EditRole).toInt();;
        callen[index] += rowcal;
        linelen[index] += rowline - rowcal; //Так как стобцы будут стоять друг на друге
        maxdate = rowdate > maxdate ? rowdate : maxdate;
        mindate = rowdate < mindate ? rowdate : mindate;

    }

    QVector<double> caldata;
    QVector<double> linedata;
    QVector<double> ticks;
    QVector<QString> labels;
    plot->addPlottable(callenbar);
    plot->addPlottable(linelenbar);
    int tickscount = 0;

    for (auto val : callen.toStdMap()) {
        caldata << val.second;
        ticks << ++tickscount;
        labels << val.first;
    }

    for (auto val : linelen.toStdMap()) {
        linedata << val.second;
    }

    for (auto v : callen.keys()) {
        if (callen[v] + linelen[v] > ymax)
            ymax = callen[v] + linelen[v];
    }

    linelenbar->setData(ticks, linedata);
    callenbar->setData(ticks, caldata);
//раскраска столбцов
    linelenbar->moveAbove(callenbar);
    QPen pen;
    pen.setWidthF(1.2);
    callenbar->setName(tr("Время разговоров"));
    pen.setColor(QColor(1, 92, 191));
    callenbar->setPen(pen);
    callenbar->setBrush(QColor(1, 92, 191, 100));
    linelenbar->setName("Время занятия линии");
    pen.setColor(QColor(255, 131, 0));
    linelenbar->setPen(pen);
    linelenbar->setBrush(QColor(255, 131, 0, 100));

    customPlot->xAxis->setLabel(tr("Абоненты или линии"));
    customPlot->xAxis->setAutoTicks(false);
    customPlot->xAxis->setAutoTickLabels(false);
    customPlot->xAxis->setTickVector(ticks);
    customPlot->xAxis->setTickVectorLabels(labels);
    customPlot->xAxis->setTickLabelRotation(60);
    customPlot->xAxis->setSubTickCount(0);
    customPlot->xAxis->setTickLength(0, 4);
    customPlot->xAxis->grid()->setVisible(true);
    customPlot->xAxis->setRange(0, linedata.size() + 2);

    // prepare y axis:
    customPlot->yAxis->setRange(0, ymax + ymax / 5);
    customPlot->yAxis->setPadding(5);
    customPlot->yAxis->setLabel(tr("Время в секундах"));
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

    QString title = "Выборка с " + QDate::fromJulianDay(mindate).toString("dd-MM-yyyy") + " по " + QDate::fromJulianDay(maxdate).toString("dd-MM-yyyy");
    setWindowTitle(title);
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    show();
}

