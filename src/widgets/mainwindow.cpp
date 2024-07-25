#include "mainwindow.h"

MainWindow::MainWindow(){
    setUI();
}

void MainWindow::setUI(){
    resize(1500, 800);
    this->mainWidget = new QWidget();
    this->mainLayout = new QGridLayout();

    this->bottomRemoteControl = new QHBoxLayout();
    this->sidePannel = new QVBoxLayout();
    QLabel *temp1 = new QLabel("bottom");
    QLabel *temp2 = new QLabel("side");

    this->bottomRemoteControl->addWidget(temp1);
    this->sidePannel->addWidget(temp2);



    this->mainWidget->setLayout(mainLayout);
    setCentralWidget(this->mainWidget);

    setWindowTitle("MCA");


    setMenuBar();

    setChartView();

    this->mainLayout->addWidget(this->mainChartView, 0, 0, 3, 3);
    this->mainLayout->addLayout(this->bottomRemoteControl, 0, 3, 3, 1);
    this->mainLayout->addLayout(this->sidePannel, 3, 0, 1, 4);
}

void MainWindow::setMenuBar(){
    QMenuBar *menuBar = this->menuBar();

    QMenu *file = new QMenu("File");
    file->addMenu("Open");
    file->addMenu("New");

    menuBar->addMenu(file);
}

void MainWindow::setChartView(){
    this->mainChart = new QChart();
    this->mainChartView = new QChartView(this->mainChart);
    this->mainSeries = new QLineSeries();

    this->mainChart->addSeries(this->mainSeries);

    auto axisX = new QValueAxis;
    axisX->setRange(0, sampleCount);
    axisX->setLabelFormat("%g");
    axisX->setTitleText("Samples");

    auto axisY = new QValueAxis;
    axisY->setRange(0, maxMagnitude);
    axisY->setTitleText("Level");

    this->mainChart->addAxis(axisX, Qt::AlignBottom);
    this->mainSeries->attachAxis(axisX);
    this->mainChart->addAxis(axisY, Qt::AlignLeft);
    this->mainSeries->attachAxis(axisY);
    this->mainChart->legend()->hide();


}