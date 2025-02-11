#include "mainwindow.h"
#include "serial.h"
#include "dialogs.h"
#include <QApplication>
#include <iostream>
#include <QLoggingCategory>
#include <QString>

int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    MainWindow window;

    window.show();

    return app.exec();
}