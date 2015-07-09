#ifndef CALLSTATISTIC_H
#define CALLSTATISTIC_H

#include <QWidget>

namespace Ui {
class CallStatistic;
}

class CallStatistic : public QWidget
{
    Q_OBJECT

public:
    explicit CallStatistic(QWidget *parent = 0);
    ~CallStatistic();

private slots:
    void on_pushButton_clicked();

private:
    Ui::CallStatistic *ui;
    void loadTextFile();
};

#endif // CALLSTATISTIC_H
