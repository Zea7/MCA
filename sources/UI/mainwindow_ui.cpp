#include "mainwindow.h"

MainWindow::MainWindow(){
    setMainUI();

    dialogTest();
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

    // Set MenuBar & ToolBar
    setMenuAndToolBar();
}

void MainWindow::setMainChartUI() {
    this->mainChart = new SpectrumChart();
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

void MainWindow::setMenuAndToolBar() {
    // File Menu Setup
    this->fileMenu = new QMenu("File");
    this->file_open = new QAction("&Open");
    this->file_closeAll = new QAction("Close All");
    this->file_save = new QAction("&Save");
    this->file_saveAs = new QAction("Save &As");
    this->file_preferences = new QAction("Preferences");
    this->file_exit = new QAction("Exit");

    QList<QAction*> actions;
    actions << this->file_open << this->file_closeAll << this->file_save 
        << this->file_saveAs << this->file_preferences << this->file_exit;

    this->fileMenu->addActions(actions);

    // View Menu Setup
    this->viewMenu = new QMenu("View");
    this->view_toolBar = new QAction("&Toolbar");
    this->view_statusBar = new QAction("&Status Bar");
    this->view_spectraList = new QAction("Spectra &List");
    
    actions.clear();
    actions << this->view_toolBar << this->view_statusBar << this->view_spectraList;
    
    this->viewMenu->addActions(actions);

    // Acquisition Menu Setup
    this->acquisitionMenu = new QMenu("Acquisition");
    this->acquisition_acquisitionSetup = new QAction("&Acquisition Setup");
    this->acquisition_changeStartStop = new QAction("&Start");
    this->acquisition_deleteDatasetTime = new QAction("Delete Data and Reset Time");
    this->acquisition_repeatMesureTest = new QAction("Repeat Measure Test");
    this->acquisition_disconnect = new QAction("Disconnect");

    actions.clear();
    actions << this->acquisition_acquisitionSetup << this->acquisition_changeStartStop
        << this->acquisition_repeatMesureTest << this->acquisition_deleteDatasetTime
        << this->acquisition_disconnect;
    
    this->acquisitionMenu->addActions(actions);

    // Display Menu Setup
    this->displayMenu = new QMenu("Display");
    this->display_scale = new QAction("Scale");
    this->display_calibration = new QAction("Calibration");

    actions.clear();
    actions << this->display_scale << this->display_calibration;

    this->displayMenu->addActions(actions);

    // Analyze Menu Setup
    this->analyzeMenu = new QMenu("Analyze");
    this->analyze_calibrate = new QAction("&Calibrate");
    this->analyze_peakSearch = new QAction("&Peak Search");
    this->analyze_defineROI = new QAction("Define ROI");

    actions.clear();
    actions << this->analyze_calibrate << this->analyze_peakSearch << this->analyze_defineROI;

    this->analyzeMenu->addActions(actions);

    // Help Menu Setup
    this->helpMenu = new QMenu("Help");
    this->help_help = new QAction("Help");

    actions.clear();
    this->helpMenu->addAction(this->help_help);

    QMenuBar *menubar = new QMenuBar();
    
    menubar->addMenu(this->fileMenu);
    menubar->addMenu(this->viewMenu);
    menubar->addMenu(this->acquisitionMenu);
    menubar->addMenu(this->displayMenu);
    menubar->addMenu(this->analyzeMenu);
    menubar->addMenu(this->helpMenu);

    this->setMenuBar(menubar);

    // Setup Tool Bar
    this->toolBar = new QToolBar();

    this->addToolBar(this->toolBar);
}