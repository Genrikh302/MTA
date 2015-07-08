#include "callstatistic.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
//    QString v = "A100200300";
//    QRegularExpression r("([C,c](\\d{3,3})(\\d{3,3})(\\d{3,3}))");
//    QRegularExpressionMatch m = r.match(v);
//    if (m.hasMatch()) {
//        qDebug() << m.captured(2);
//        qDebug() << m.captured(3);
//        qDebug() << m.captured(4);
//    }

//    return 0;
    QApplication app(argc, argv);
    MainWindow w;
    w.show();

    return app.exec();
}
