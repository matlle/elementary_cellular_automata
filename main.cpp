#include <iostream>
#include <QApplication>
#include <headers/mainwindow.h>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    return a.exec();
}