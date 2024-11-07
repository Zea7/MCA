#include "mainwindow.h"
#include "serial.h"
#include <QApplication>
#include <iostream>
#include <QLoggingCategory>

int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    MainWindow window;

    window.show();

    UartDetector *detector = new UartDetector();
    

    return app.exec();
}