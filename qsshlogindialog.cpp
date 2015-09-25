#include "qsshlogindialog.h"
#include "ui_qsshlogindialog.h"

QSSHLoginDialog::QSSHLoginDialog(const QString &host, const QString &port, const QString &user, const QString &password, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::qsshlogindialog)
{
    ui->setupUi(this);

    QRegExp regExp = QRegExp("((1{0,1}[0-9]{0,2}|2[0-4]{1,1}[0-9]{1,1}|25[0-5]{1,1})\\.){3,3}(1{0,1}[0-9]{0,2}|2[0-4]{1,1}[0-9]{1,1}|25[0-5]{1,1}):[0-9]{1,5}");
    QRegExpValidator *validator = new QRegExpValidator(regExp, this);

    ui->lineEditIP->setValidator(validator);

    ui->lineEditIP->setText(QString(host + (port == "22" ? "" : QString(":%1").arg(port))));
    ui->lineEditUser->setText(user);
    ui->lineEditPassword->setText(password);

}

QSSHLoginDialog::~QSSHLoginDialog()
{
    delete ui;
}
