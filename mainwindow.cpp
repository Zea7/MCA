#include "mainwindow.h"


MainWindow::MainWindow(){
    setUI();
    this->data = new MCAData();
}

void MainWindow::setUI(){
    this->tempData = std::vector<int> (1024,0);
    for(int i=0;i<1024;i++){
        this->tempData[i] = 0;
    }
    resize(1700, 1000);
    this->mainWidget = new QWidget();
    this->mainLayout = new QGridLayout();

    SerialSettingDialog *dialog = new SerialSettingDialog();
    QObject::connect(dialog, &SerialSettingDialog::setSerialPort, this, &MainWindow::getSerialPort);
    dialog->exec();

    this->bottomRemoteControl = new QGridLayout();
    this->sidePannel = new QTabWidget();

    QLabel *channel = new QLabel("Channel : ");
    QLabel *level = new QLabel("Level : ");
    QLabel *temp2 = new QLabel("side");

    this->xData = new QLabel("");
    this->yData = new QLabel("");

    this->setLogScaleCheckBox = new QCheckBox("Set Y Axis Log Scale");


    this->bottomRemoteControl->addWidget(channel, 0, 0, 1, 1);
    this->bottomRemoteControl->addWidget(xData, 0, 1, 1, 1);
    this->bottomRemoteControl->addWidget(level, 1, 0, 1,1);
    this->bottomRemoteControl->addWidget(yData, 1, 1, 1, 1);
    this->bottomRemoteControl->addWidget(this->setLogScaleCheckBox, 0, 2, 1, 1);


    this->mainWidget->setLayout(mainLayout);
    setCentralWidget(this->mainWidget);

    setWindowTitle("iMCA");

    setMCAMenuBar();

    setSidePannel();

    setChartView();

    setSignalFromMenuBar();

    this->mainLayout->addWidget(this->mainChartView, 0, 0, 7, 7);
    this->mainLayout->addWidget(this->sidePannel, 0, 7, 9, 2);
    this->mainLayout->addLayout(this->bottomRemoteControl, 7, 0, 2, 7);

}

void MainWindow::setMCAMenuBar(){
    QMenuBar *menubar = new QMenuBar();
    this->toolbar = new ToolBar();

    menubar->addMenu(this->fileMenu = new FileMenu());
    menubar->addMenu(this->viewMenu = new ViewMenu());
    menubar->addMenu(this->acquisitionMenu = new AcquisitionMenu());
    menubar->addMenu(this->displayMenu = new DisplayMenu());
    menubar->addMenu(this->analyzeMenu = new AnalyzeMenu());
    menubar->addMenu(this->helpMenu = new HelpMenu());
    
    this->setMenuBar(menubar);
    this->addToolBar(toolbar);
}

void MainWindow::setChartView(){
    this->mainChart = new SpectoChart();
    this->mainChartView = new QChartView(this->mainChart);
    QObject::connect(this, SIGNAL(setSampleRange(int, int)), this->mainChart, SLOT(changeSampleRange(int, int)));
    QObject::connect(this, SIGNAL(setMaxMagnitude(int)), this->mainChart, SLOT(changeMaxMagnitude(int)));
    
    QObject::connect(this, SIGNAL(resizeXAxis()), this->mainChart, SLOT(resizeXAxis()));
    QObject::connect(this, SIGNAL(resizeYAxis()), this->mainChart, SLOT(resizeYAxis()));

    QObject::connect(this->mainChart, SIGNAL(GetHoveredData(bool, int, int)), this, SLOT(ShowHoveredData(bool, int, int)));

    QObject::connect(this, SIGNAL(selectedROIRegionToShow(std::vector<std::pair<int, int>>)), this->mainChart, SLOT(ROIRegionChange(std::vector<std::pair<int, int>>)));

    QObject::connect(this->setLogScaleCheckBox, &QCheckBox::clicked, this->mainChart, &SpectoChart::changeYAxisScale);
}

void MainWindow::setSignalFromMenuBar(){
    QObject::connect(this->fileMenu, SIGNAL(openFile()), this, SLOT(openMCAFile()));
    QObject::connect(this->toolbar, SIGNAL(openFile()), this, SLOT(openMCAFile())); 

    QObject::connect(this->fileMenu, SIGNAL(saveFile()), this, SLOT(saveMCAFile()));
    QObject::connect(this->toolbar, SIGNAL(saveFile()), this, SLOT(saveMCAFile()));

    QObject::connect(this->toolbar, SIGNAL(resizeXAxis()), this, SLOT(autoResizeXAxis()));
    QObject::connect(this->toolbar, SIGNAL(resizeYAxis()), this, SLOT(autoResizeYAxis()));

    QObject::connect(this->analyzeMenu, SIGNAL(openROIDialog()), this, SLOT(openROIDialog()));
    QObject::connect(this->toolbar, SIGNAL(openROIDialog()), this, SLOT(openROIDialog()));
    QObject::connect(this->analyzeMenu, SIGNAL(openAutoPeakDialog()), this, SLOT(openAutoPeakDialog()));

    QObject::connect(this->toolbar, SIGNAL(showSpecificRegion()), this, SLOT(showSpecificRegion()));

    QObject::connect(this->acquisitionMenu, &AcquisitionMenu::startDetection, this, &MainWindow::startDetection);
    QObject::connect(this->acquisitionMenu, &AcquisitionMenu::stopDetection, this, &MainWindow::stopDetection);
}

void MainWindow::setSidePannel(){
    this->infoTab = new QWidget();
    this->roiTab = new ROITab();
    this->sidePannel->addTab(this->infoTab, "Info");
    this->sidePannel->addTab(this->roiTab, "ROI");

    QObject::connect(this, SIGNAL(setROIRegion(std::vector<std::pair<int, int>>)), this->roiTab, SLOT(getROIRegions(std::vector<std::pair<int ,int>>)));
    QObject::connect(this->roiTab, SIGNAL(openROIDialog()), this, SLOT(openROIDialog()));
    QObject::connect(this->roiTab, &ROITab::sendArgumentsToCalculateGaussian, this, &MainWindow::getArgumentsToCalculateGaussian);
    QObject::connect(this, &MainWindow::sendGaussianDistributionData, this->roiTab, &ROITab::getGaussianDistributionData);
}

/* 
    functions for get information of roi Region
*/

// std::pair<double, double> MainWindow::getLinearFuncFromData(std::vector<std::pair<int, int>> points){

// }

double MainWindow::getAreaSizeFromData(std::vector<double> dataset){
    double sum = 0.0;

    for(int i=0;i<dataset.size();i++){
        sum += dataset[i];
    }

    return sum;
}

void MainWindow::setInfoTab(QStringList list){
    QVBoxLayout *newLayout = new QVBoxLayout();
    for(auto &i : list){
        newLayout->addWidget(new QLabel(i));
    }

    infoTab->setLayout(newLayout);
}

/* 
    SLOTS

    openMCAFile()   :   

*/

void MainWindow::openMCAFile(){
    QString fileTypeFilter = "MCA file (*.mca *.txt *.csv) ;; All Files (*.*)";
    QString fileName = QFileDialog::getOpenFileName(this, "Open MCA data file", QDir::currentPath(), fileTypeFilter);

    QStringList dataList;
    qDebug() << fileName;
    QFile dataFile(fileName);

    if(!dataFile.open(QFile::ReadOnly|QFile::Text)){
        if(!dataFile.exists()){
            qDebug() << "File Open Error!\n File Does Not Exists.";
        }
        else{
            qDebug() << "File Open Error!\n Unknown Error occured.";
        }
        return ;
    }

    QTextStream openFile(&dataFile);
    QString data;

    while(!openFile.atEnd()){
        data = openFile.readLine();
        dataList.append(data.trimmed());
        // qDebug() << data;
    }

    this->data = new MCAData(dataList, fileName.split(".")[1]);

    dataFile.close();

    std::vector<int> spectrumData = this->data->getData();

    this->endSample = spectrumData.size();
    this->maxMagnitude = *std::max_element(spectrumData.begin(), spectrumData.end()); 
    this->mainChart->addData(spectrumData);
    
    emit setSampleRange(startSample, endSample);
    emit setMaxMagnitude(maxMagnitude);
    QStringList list = this->data->getHeaderData();
    setInfoTab(list);

    autoResizeXAxis();
    autoResizeYAxis();
}

void MainWindow::saveMCAFile(){
    QString filter =  "CSV File (*.csv) ;; Text File (*.txt) ;; MCA File (*.mca) ;; All Files (*.*)";
    QString defaultFilter = "CSV File (*.csv)";
    QString fileName = QFileDialog::getSaveFileName(this, "Save File", QDir::currentPath(), filter, &defaultFilter);

    qDebug() << fileName;

    if(fileName.split(".")[1] == "csv") this->data->saveAsCSV(fileName);
    else if(fileName.split(".")[1] == "txt") this->data->saveAsTXT(fileName);
}

void MainWindow::autoResizeXAxis() {
    bool start = false;
    std::vector<int> data = this->data->getData();
    std::pair<int, int> border = getBorderByThreshold(data, this->threshold);
    this->startSample = border.first; this->endSample = border.second;

    emit setSampleRange(0, 256);
    emit resizeXAxis();
}


void MainWindow::autoResizeYAxis() {
    emit setMaxMagnitude(maxMagnitude);
    emit resizeYAxis();
}


void MainWindow::openROIDialog(){
    DefineROIDialog *dialog = new DefineROIDialog(roiRegions);
    QObject::connect(dialog, SIGNAL(sendROIRegions(std::vector<std::pair<int, int>>)), this, SLOT(getROIRegions(std::vector<std::pair<int, int>>)));
    QObject::connect(dialog, SIGNAL(sendROIRegions(std::vector<std::pair<int, int>>)), this, SIGNAL(setROIRegion(std::vector<std::pair<int, int>>)));
    QObject::connect(dialog, SIGNAL(sendShowRegion(int, int)), this, SLOT(sendSpecificRegion(int, int)));
    dialog->show();
}

void MainWindow::getROIRegions(std::vector<std::pair<int, int>> roiRegions){
    this->roiRegions = roiRegions;
    qDebug() << roiRegions;
}

void MainWindow::showSpecificRegion(){
    DefineROIDialog *dialog = new DefineROIDialog(roiRegions);
    QObject::connect(dialog, SIGNAL(sendROIRegions(std::vector<std::pair<int, int>>)), this, SLOT(getROIRegions(std::vector<std::pair<int, int>>)));
    QObject::connect(dialog, SIGNAL(sendROIRegions(std::vector<std::pair<int, int>>)), this->mainChart, SLOT(ROIRegionChange(std::vector<std::pair<int, int>>)));
    QObject::connect(dialog, SIGNAL(sendShowRegion(int, int)), this, SLOT(sendSpecificRegion(int, int)));
    dialog->show();
}

void MainWindow::sendSpecificRegion(int start, int end){
    emit setSampleRange(0, 256);
    emit resizeXAxis();
}

void MainWindow::ShowHoveredData(bool status, int index, int value){
    // qDebug() << index;
    // qDebug() << value;
    this->xData->setText(QString::number(index));
    this->yData->setText(QString::number(value));
}

void MainWindow::openAutoPeakDialog(){
    AutoPeakDialog *dialog = new AutoPeakDialog(startSample, endSample);
    QObject::connect(dialog, SIGNAL(doAutoPeakSearch(int, int, int, int)), this, SLOT(doAutoPeakSearch(int, int, int, int)));
    dialog->show();
}

void MainWindow::doAutoPeakSearch(int start, int end, int left, int right){
    std::vector<std::pair<int, int>> points;

    for(int i=-1; i<=1; i++){
        points.push_back({left+i, data->getData()[left+i]});
        points.push_back({right+i, data->getData()[right+i]});
    }

    std::pair<double, double> linearFunc = linearFitMethodWithLSM<int>(points);

    std::vector<double> dataset;

    for(int i=start;i<end;i++){
        double dataPoint = (double)data->getData()[i] - (linearFunc.first * i + linearFunc.second);
        dataset.push_back(dataPoint);
    }


    std::pair<double, double> gaussian = fitGaussianDistribution(dataset);

    double FHWM = 2.3548200 * gaussian.second;

    this->roiRegions.push_back({start, end});
    emit setROIRegion(this->roiRegions);

    qDebug() << gaussian.first;
    qDebug() << gaussian.second;
    qDebug() << FHWM;

}

void MainWindow::getArgumentsToCalculateGaussian(int roiRegionIndex, int pointIndex){
    if(this->data == NULL){
        qDebug() << "No data";
        return ;
    }
    std::vector<int> recentData = this->tempData;

    std::vector<std::pair<int, int>> chosenRegion;
    chosenRegion.push_back(roiRegions[roiRegionIndex]);

    int start = chosenRegion[0].first, end = chosenRegion[0].second;

    emit selectedROIRegionToShow(chosenRegion);

    std::vector<std::pair<int, int>> points;
    
    if(pointIndex == 0){
        for(int i=-1;i<=1;i++){
            points.push_back({start + i, recentData[start + i]});
            points.push_back({end + i, recentData[end + i]});
        }
    } else if(pointIndex == 1){
        for(int i=-2;i<=2;i++){
            points.push_back({start + i, recentData[start + i]});
            points.push_back({end + i, recentData[end + i]});
        }
    }

    std::pair<double, double> linearFunc = linearFitMethodWithLSM(points);

    std::vector<double> dataset;
    std::vector<double> rawDataset;
    for(int i=start; i<=end;i++){
        double dataPoint = (double)recentData[i] - (linearFunc.first * i + linearFunc.second);
        dataset.push_back(dataPoint);
        rawDataset.push_back((double)recentData[i]);
    }
    
    std::pair<double, double> gaussian = fitGaussianDistribution(dataset);

    double areaSize = getAreaSizeFromData(dataset);
    double rawAreaSize = getAreaSizeFromData(rawDataset);

    std::vector<std::pair<double, double>> sendData;
    sendData.push_back(linearFunc);
    sendData.push_back(gaussian);
    sendData.push_back({areaSize, rawAreaSize});

    emit sendGaussianDistributionData(sendData);
}

void MainWindow::getSerialPort(QString port){
    this->port = port;
    this->uart = new UartCommunicator(port.toStdString());

    
    TempDialog *cmd  = new TempDialog(this->uart);
    cmd->show();

    this->detectThread = new DetectThread(this->uart);
    
    this->work = new QThread(this);
    this->detectThread->moveToThread(this->work);
    QObject::connect(this->detectThread, &DetectThread::setData, this, &MainWindow::setMainChartData);
    QObject::connect(this->work, &QThread::started, this->detectThread, &DetectThread::start);
    QObject::connect(this, &QMainWindow::destroyed, [=]{
        this->detectThread->stop();
        this->work->quit();
        this->work->wait();
    });
}

void MainWindow::startDetection(){
    this->work->start();
    QMetaObject::invokeMethod(this->detectThread, "start", Qt::QueuedConnection);
}

void MainWindow::stopDetection(){
    QMetaObject::invokeMethod(this->detectThread, "stop", Qt::QueuedConnection);
}

void MainWindow::setMainChartData(std::vector<int> data){
    this->data->setData(data);
    this->mainChart->setData(data);
}