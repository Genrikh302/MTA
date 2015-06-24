#include "filterdialog.h"
#include "ui_filterdialog.h"
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

void FilterDialog::on_buttonBox_accepted()
{
    ui->abin->text();
    qDebug() << ui->abin->text();
}
