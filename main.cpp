#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qDebug() << "QDebug test from main";
    MainWindow w;
    w.show();
    
    return a.exec();
}
