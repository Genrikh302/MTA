#include "callstatistic.h"
#include "ui_callstatistic.h"
#include <QFile>
#include <QTextStream>
#include <QFileDialog>

CallStatistic::CallStatistic(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CallStatistic)
{
    ui->setupUi(this);
    loadTextFile();
}

CallStatistic::~CallStatistic()
{
    delete ui;
}
void CallStatistic::loadTextFile()
{
    QString fileway = QFileDialog::getOpenFileName(0, "Open Dialog", "", "*.log *.txt");
    QFile inputFile(fileway);
    inputFile.open(QIODevice::ReadOnly);
    QTextStream in(&inputFile);
    QString line = in.readAll();
    //QString line = str.readAll();
    inputFile.close();
    ui->textFile->setPlainText(line);
    QTextCursor cursor = ui->textFile->textCursor();
    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 1);
}
void CallStatistic::on_pushButton_clicked()
{
    QString searchString = ui->lineEdit->text();
    ui->textFile->find(searchString, QTextDocument::FindWholeWords);
}
