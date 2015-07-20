#include "successdialog.h"
#include "ui_successdialog.h"
#include <QDate>

SuccessDialog::SuccessDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SuccessDialog)
{
    ui->setupUi(this);
}

SuccessDialog::~SuccessDialog()
{
    delete ui;
}

 QDate SuccessDialog::getdate(void){
    //date = QDate::fromString(ui->dateEdit->text(), "MM.yy").addYears(100).toJulianDay();
     date = ui->dateEdit->date();
    return date;
 }

void SuccessDialog::on_SuccessDialog_accepted()
{

}
