#include "mainwindow.h"

MainWindow::MainWindow(){
    setUI();
}

void MainWindow::setUI(){
    // Set Default Window Size
    resize(1700, 1000);

    // Set Main Widget & Layout
    this->mainWidget = new QWidget();
    this->mainLayout = new QGridLayout();

    // Call Serial Setting Dialog & Set Serial setting


    // Set Bottom & Side Pannel
    this->bottomRemoteControl = new QGridLayout();
    this->sidePannel = new QTabWidget();

    QLabel *channel = new QLabel("Channel : ");
    QLabel *level = new QLabel("Level : ");
    QLabel *temp = new QLabel("side");

    this->xData = new QLabel("");
    this->yData = new QLabel("");

    this->setLogScaleCheckBox = new QCheckBox("Set Y Axis Log Scale");
}