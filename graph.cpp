#include "graph.h"
#include "ui_graph.h"
#include <QDebug>
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
             qDebug() << relindex << "=" << m[relindex] << columnum;
             data << m[relindex];
             labels << QString("%1").arg(relindex);
             if(ymax < m[relindex])
                 ymax = m[relindex];
        }
    }
    bar->setData(ticks, data);

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
    customPlot->yAxis->setRange(0, ymax);
    customPlot->yAxis->setPadding(5); // a bit more space to the left border
    customPlot->yAxis->setLabel("Количество вызовов");
    customPlot->yAxis->grid()->setSubGridVisible(true);

    QPen gridPen;
    gridPen.setStyle(Qt::SolidLine);
    gridPen.setColor(QColor(0, 0, 0, 25));
    customPlot->yAxis->grid()->setPen(gridPen);
    gridPen.setStyle(Qt::DotLine);
    customPlot->yAxis->grid()->setSubGridPen(gridPen);


    qDebug() << columnum;

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
 }


void Graph::buildReportSucessCalls(QSqlTableModel *cdrModel)
{
    Q_UNUSED(cdrModel);
}
