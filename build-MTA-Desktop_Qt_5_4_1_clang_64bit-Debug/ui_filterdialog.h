/********************************************************************************
** Form generated from reading UI file 'filterdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILTERDIALOG_H
#define UI_FILTERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTimeEdit>

QT_BEGIN_NAMESPACE

class Ui_FilterDialog
{
public:
    QDialogButtonBox *buttonBox;
    QLineEdit *abin;
    QLabel *abinlabel;
    QLabel *aboutlabel;
    QLineEdit *about;
    QLabel *datelabel;
    QDateEdit *datesince;
    QLabel *datesincelabel;
    QLabel *datetolabel;
    QDateEdit *dateto;
    QLabel *timelabel;
    QLabel *timesincelanel;
    QLabel *timetolable;
    QTimeEdit *timesince;
    QTimeEdit *timeto;
    QLabel *busylenlabel;
    QLineEdit *busyfrom;
    QLabel *busyfromlabel;
    QLabel *busytolabel;
    QLineEdit *busyto;
    QLabel *talklenlabel;
    QLabel *talklenfromlabel;
    QLineEdit *talklenfrom;
    QLabel *talklentolabel;
    QLineEdit *talklento;
    QComboBox *type;
    QLabel *typelabel;
    QLabel *reasonlabel;
    QComboBox *reason;
    QLabel *inaonlabel;
    QLabel *outaonlabel;
    QLineEdit *inaon;
    QLineEdit *outaon;
    QLabel *innumlabel;
    QLabel *outnumlabel;
    QLineEdit *innum;
    QLineEdit *outnum;

    void setupUi(QDialog *FilterDialog)
    {
        if (FilterDialog->objectName().isEmpty())
            FilterDialog->setObjectName(QStringLiteral("FilterDialog"));
        FilterDialog->resize(800, 600);
        buttonBox = new QDialogButtonBox(FilterDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(500, 460, 171, 31));
        buttonBox->setLayoutDirection(Qt::LeftToRight);
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        abin = new QLineEdit(FilterDialog);
        abin->setObjectName(QStringLiteral("abin"));
        abin->setGeometry(QRect(20, 40, 141, 21));
        abinlabel = new QLabel(FilterDialog);
        abinlabel->setObjectName(QStringLiteral("abinlabel"));
        abinlabel->setGeometry(QRect(20, 10, 141, 31));
        aboutlabel = new QLabel(FilterDialog);
        aboutlabel->setObjectName(QStringLiteral("aboutlabel"));
        aboutlabel->setGeometry(QRect(20, 70, 141, 31));
        about = new QLineEdit(FilterDialog);
        about->setObjectName(QStringLiteral("about"));
        about->setGeometry(QRect(20, 100, 141, 21));
        datelabel = new QLabel(FilterDialog);
        datelabel->setObjectName(QStringLiteral("datelabel"));
        datelabel->setGeometry(QRect(20, 130, 141, 31));
        datesince = new QDateEdit(FilterDialog);
        datesince->setObjectName(QStringLiteral("datesince"));
        datesince->setGeometry(QRect(40, 160, 111, 21));
        datesincelabel = new QLabel(FilterDialog);
        datesincelabel->setObjectName(QStringLiteral("datesincelabel"));
        datesincelabel->setGeometry(QRect(20, 160, 16, 20));
        datetolabel = new QLabel(FilterDialog);
        datetolabel->setObjectName(QStringLiteral("datetolabel"));
        datetolabel->setGeometry(QRect(160, 160, 21, 20));
        dateto = new QDateEdit(FilterDialog);
        dateto->setObjectName(QStringLiteral("dateto"));
        dateto->setGeometry(QRect(190, 160, 111, 21));
        timelabel = new QLabel(FilterDialog);
        timelabel->setObjectName(QStringLiteral("timelabel"));
        timelabel->setGeometry(QRect(20, 190, 141, 31));
        timesincelanel = new QLabel(FilterDialog);
        timesincelanel->setObjectName(QStringLiteral("timesincelanel"));
        timesincelanel->setGeometry(QRect(20, 220, 16, 20));
        timetolable = new QLabel(FilterDialog);
        timetolable->setObjectName(QStringLiteral("timetolable"));
        timetolable->setGeometry(QRect(160, 220, 21, 20));
        timesince = new QTimeEdit(FilterDialog);
        timesince->setObjectName(QStringLiteral("timesince"));
        timesince->setGeometry(QRect(40, 220, 118, 24));
        timeto = new QTimeEdit(FilterDialog);
        timeto->setObjectName(QStringLiteral("timeto"));
        timeto->setGeometry(QRect(190, 220, 118, 24));
        busylenlabel = new QLabel(FilterDialog);
        busylenlabel->setObjectName(QStringLiteral("busylenlabel"));
        busylenlabel->setGeometry(QRect(20, 250, 191, 31));
        busyfrom = new QLineEdit(FilterDialog);
        busyfrom->setObjectName(QStringLiteral("busyfrom"));
        busyfrom->setGeometry(QRect(40, 280, 71, 21));
        busyfromlabel = new QLabel(FilterDialog);
        busyfromlabel->setObjectName(QStringLiteral("busyfromlabel"));
        busyfromlabel->setGeometry(QRect(20, 280, 21, 16));
        busytolabel = new QLabel(FilterDialog);
        busytolabel->setObjectName(QStringLiteral("busytolabel"));
        busytolabel->setGeometry(QRect(120, 280, 21, 16));
        busyto = new QLineEdit(FilterDialog);
        busyto->setObjectName(QStringLiteral("busyto"));
        busyto->setGeometry(QRect(150, 280, 71, 21));
        talklenlabel = new QLabel(FilterDialog);
        talklenlabel->setObjectName(QStringLiteral("talklenlabel"));
        talklenlabel->setGeometry(QRect(20, 310, 211, 31));
        talklenfromlabel = new QLabel(FilterDialog);
        talklenfromlabel->setObjectName(QStringLiteral("talklenfromlabel"));
        talklenfromlabel->setGeometry(QRect(20, 350, 21, 16));
        talklenfrom = new QLineEdit(FilterDialog);
        talklenfrom->setObjectName(QStringLiteral("talklenfrom"));
        talklenfrom->setGeometry(QRect(50, 350, 71, 21));
        talklentolabel = new QLabel(FilterDialog);
        talklentolabel->setObjectName(QStringLiteral("talklentolabel"));
        talklentolabel->setGeometry(QRect(130, 350, 21, 16));
        talklento = new QLineEdit(FilterDialog);
        talklento->setObjectName(QStringLiteral("talklento"));
        talklento->setGeometry(QRect(160, 350, 71, 21));
        type = new QComboBox(FilterDialog);
        type->setObjectName(QStringLiteral("type"));
        type->setGeometry(QRect(20, 420, 104, 26));
        typelabel = new QLabel(FilterDialog);
        typelabel->setObjectName(QStringLiteral("typelabel"));
        typelabel->setGeometry(QRect(20, 400, 59, 16));
        reasonlabel = new QLabel(FilterDialog);
        reasonlabel->setObjectName(QStringLiteral("reasonlabel"));
        reasonlabel->setGeometry(QRect(170, 400, 101, 16));
        reason = new QComboBox(FilterDialog);
        reason->setObjectName(QStringLiteral("reason"));
        reason->setGeometry(QRect(170, 420, 104, 26));
        inaonlabel = new QLabel(FilterDialog);
        inaonlabel->setObjectName(QStringLiteral("inaonlabel"));
        inaonlabel->setGeometry(QRect(300, 10, 141, 31));
        outaonlabel = new QLabel(FilterDialog);
        outaonlabel->setObjectName(QStringLiteral("outaonlabel"));
        outaonlabel->setGeometry(QRect(300, 70, 141, 31));
        inaon = new QLineEdit(FilterDialog);
        inaon->setObjectName(QStringLiteral("inaon"));
        inaon->setGeometry(QRect(290, 40, 141, 21));
        outaon = new QLineEdit(FilterDialog);
        outaon->setObjectName(QStringLiteral("outaon"));
        outaon->setGeometry(QRect(290, 100, 141, 21));
        innumlabel = new QLabel(FilterDialog);
        innumlabel->setObjectName(QStringLiteral("innumlabel"));
        innumlabel->setGeometry(QRect(630, 10, 141, 31));
        outnumlabel = new QLabel(FilterDialog);
        outnumlabel->setObjectName(QStringLiteral("outnumlabel"));
        outnumlabel->setGeometry(QRect(630, 70, 141, 31));
        innum = new QLineEdit(FilterDialog);
        innum->setObjectName(QStringLiteral("innum"));
        innum->setGeometry(QRect(620, 40, 141, 21));
        outnum = new QLineEdit(FilterDialog);
        outnum->setObjectName(QStringLiteral("outnum"));
        outnum->setGeometry(QRect(620, 100, 141, 21));

        retranslateUi(FilterDialog);
        QObject::connect(buttonBox, SIGNAL(rejected()), FilterDialog, SLOT(reject()));
        QObject::connect(buttonBox, SIGNAL(accepted()), FilterDialog, SLOT(accept()));

        QMetaObject::connectSlotsByName(FilterDialog);
    } // setupUi

    void retranslateUi(QDialog *FilterDialog)
    {
        FilterDialog->setWindowTitle(QApplication::translate("FilterDialog", "\320\244\320\270\320\273\321\214\321\202\321\200", 0));
        abinlabel->setText(QApplication::translate("FilterDialog", "\320\220\320\261\320\276\320\275\320\265\320\275\321\202 \320\220/\320\222\321\205. \320\241\320\233", 0));
        aboutlabel->setText(QApplication::translate("FilterDialog", "\320\220\320\261\320\276\320\275\320\265\320\275\321\202 \320\221/\320\230\321\201\321\205. \320\241\320\233", 0));
        datelabel->setText(QApplication::translate("FilterDialog", "\320\224\320\260\321\202\320\260", 0));
        datesincelabel->setText(QApplication::translate("FilterDialog", "\320\241", 0));
        datetolabel->setText(QApplication::translate("FilterDialog", "\320\237\320\276", 0));
        timelabel->setText(QApplication::translate("FilterDialog", "\320\222\321\200\320\265\320\274\321\217", 0));
        timesincelanel->setText(QApplication::translate("FilterDialog", "\320\241", 0));
        timetolable->setText(QApplication::translate("FilterDialog", "\320\237\320\276", 0));
        busylenlabel->setText(QApplication::translate("FilterDialog", "\320\237\321\200\320\276\320\264\320\276\320\273\320\266\320\270\321\202\320\265\320\273\321\214\320\275\320\276\321\201\321\202\321\214 \320\267\320\260\320\275\321\217\321\202\320\270\321\217", 0));
        busyfromlabel->setText(QApplication::translate("FilterDialog", "\320\236\321\202", 0));
        busytolabel->setText(QApplication::translate("FilterDialog", "\320\224\320\276", 0));
        talklenlabel->setText(QApplication::translate("FilterDialog", "\320\237\321\200\320\276\320\264\320\276\320\273\320\266\320\270\321\202\320\265\320\273\321\214\320\275\320\276\321\201\321\202\321\214 \321\200\320\260\320\267\320\263\320\276\320\262\320\276\321\200\320\260", 0));
        talklenfromlabel->setText(QApplication::translate("FilterDialog", "\320\236\321\202", 0));
        talklentolabel->setText(QApplication::translate("FilterDialog", "\320\224\320\276", 0));
        typelabel->setText(QApplication::translate("FilterDialog", "\320\242\320\270\320\277", 0));
        reasonlabel->setText(QApplication::translate("FilterDialog", "\320\237\321\200\320\270\321\207\320\270\320\275\320\260 \320\276\321\202\320\261\320\276\321\217", 0));
        inaonlabel->setText(QApplication::translate("FilterDialog", "\320\222\321\205. \320\220\320\236\320\235", 0));
        outaonlabel->setText(QApplication::translate("FilterDialog", "\320\230\321\201\321\205. \320\220\320\236\320\235", 0));
        innumlabel->setText(QApplication::translate("FilterDialog", "\320\222\321\205. \320\275\320\276\320\274\320\265\321\200", 0));
        outnumlabel->setText(QApplication::translate("FilterDialog", "\320\230\321\201\321\205. \320\275\320\276\320\274\320\265\321\200", 0));
    } // retranslateUi

};

namespace Ui {
    class FilterDialog: public Ui_FilterDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILTERDIALOG_H
