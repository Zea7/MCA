#include "mainwindow.h"

MainWindow::MainWindow(){
    setMainUI();
}

void MainWindow::setMainUI(){
    // Set Default Window Size
    resize(1700, 1000);

    // Set Main Widget & Layout
    this->mainWidget = new QWidget();
    this->mainLayout = new QGridLayout();

    // Call Serial Setting Dialog & Set Serial setting


    // Make Pannel Objects
    this->bottomControlPannel = new QGridLayout();
    this->sidePannel = new QTabWidget();

    // Set mainWidget's pannels' UI
    this->setMainChartUI();
    this->setSidePannelUI();
    this->setBottomControlPannelUI();

    // Collect every components into mainWidget
    this->mainLayout->addWidget(this->mainChartView, 0, 0, 7, 7);
    this->mainLayout->addWidget(this->sidePannel, 0, 7, 9, 2);
    this->mainLayout->addLayout(this->bottomControlPannel, 7, 0, 2, 7);

    this->mainWidget->setLayout(this->mainLayout);
    setCentralWidget(this->mainWidget);
}

void MainWindow::setMainChartUI() {
    this->mainChart = new MainSpectrumChart();
    this->mainChartView = new QChartView(this->mainChart);
}

void MainWindow::setSidePannelUI() {
    this->basicInformationTab = new QWidget();
    this->regionInformationTab = new ROITabWidget();

    this->sidePannel->addTab(this->basicInformationTab, "Info");
    this->sidePannel->addTab(this->regionInformationTab, "ROI");
}

void MainWindow::setBottomControlPannelUI() {
    QLabel *channel = new QLabel("Channel : ");
    QLabel *level = new QLabel("Level : ");
    QLabel *temp = new QLabel("side");

    this->xData = new QLabel("");
    this->yData = new QLabel("");

    this->setLogScaleCheckBox = new QCheckBox("Set Y Axis Log Scale");

    // Put every widget which associated with bottom pannel into bottom pannel
    this->bottomControlPannel->addWidget(channel, 0, 0, 1, 1);
    this->bottomControlPannel->addWidget(xData, 0, 1, 1, 1);
    this->bottomControlPannel->addWidget(level, 1, 0, 1, 1);
    this->bottomControlPannel->addWidget(yData, 1, 1, 1, 1);
    this->bottomControlPannel->addWidget(this->setLogScaleCheckBox, 0, 2, 1, 1);
}