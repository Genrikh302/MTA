#ifndef QSPEECHLOGIN_H
#define QSPEECHLOGIN_H


#include <QDialog>
#include "ui_qsshlogindialog.h"

namespace Ui {
class qsshlogindialog;
}

class QSSHLoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QSSHLoginDialog(QWidget *parent = 0);
    ~QSSHLoginDialog();


private:
    Ui::qsshlogindialog *ui;

public:

    // получаем введенный ip адрес
    QString getIP() {return ui->lineEditIP->text();}

    // получаем введенный login
    QString getUser() {return ui->lineEditUser->text();}

    // получаем введенный password
    QString getPassword() {return ui->lineEditPassword->text();}
};

#endif // QSPEECHLOGIN_H
