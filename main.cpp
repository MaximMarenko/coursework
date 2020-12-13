#include "mainwindow.h"

#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.setWindowTitle("Ежедневник");
    w.show();
    a.exec();
}
