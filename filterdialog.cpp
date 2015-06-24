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


void FilterDialog::getfil(QString &abfd1)
{
    abfd1 = ui->abin->text();
    qDebug() << abfd1 << "+";
}
