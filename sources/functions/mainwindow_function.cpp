#include "mainwindow.h"

void MainWindow::dialogTest() {
    // SpectrumListManager *dialog = new SpectrumListManager();
    // dialog->setModal(true);

    // dialog->show();

    // ROIRegionManager *dialog = new ROIRegionManager();
    // dialog->setModal(true);

    // dialog->show();

    SerialManager *dialog = new SerialManager();
    dialog->setModal(true);

    dialog->show();

    QObject::connect(dialog, &SerialManager::sendSerialSetter, this, &MainWindow::getSerialSetter);
}

void MainWindow::setUISignalSlotConnection() {
    /* 
        Signal Slot connection between MenuBar and Window
    */
    QObject::connect(this->file_open, &QAction::triggered, this, &MainWindow::openMCAFile);
    QObject::connect(this->file_saveAs, &QAction::triggered, this, &MainWindow::saveAsMCAFile);

    QObject::connect(this->view_spectraList, &QAction::triggered, this, &MainWindow::showSpectrumListManager);
    QObject::connect(this->acquisition_acquisitionSetup, &QAction::triggered, this, &MainWindow::dialogTest);

    QObject::connect(this->acquisition_changeStartStop, &QAction::triggered, [this]{
        if(this->isLiveMeasuring) this->stopDetection();
        else this->startDetection();
    });

    QObject::connect(this->analyze_defineROI, &QAction::triggered, this, &MainWindow::showROIRegionManager);
    /* 
        Signal Slot Connection between ToolBar and Window 
    */
    QObject::connect(this->toolbar_open, &QAction::triggered, this, &MainWindow::openMCAFile);
    QObject::connect(this->toolbar_save, &QAction::triggered, this, &MainWindow::saveAsMCAFile);

    /* 
        Signal Slot connection between Tab widgets and Window
    */
    QObject::connect(this->regionInformationTab, &ROITabWidget::sendArgumentsToCalculateGaussianDistribution, this, &MainWindow::calculateGaussianDistributionWithArguments);
    QObject::connect(this, &MainWindow::sendGaussianDistributionData, this->regionInformationTab, &ROITabWidget::setInformationFromDistributionData);

    QObject::connect(this, &MainWindow::roiRegionChanged, this->regionInformationTab, &ROITabWidget::getROIRegions);
    QObject::connect(this->regionInformationTab, &ROITabWidget::openROIDialog, this, &MainWindow::showROIRegionManager);
    /* 
        Signal Slot connection between widgets
    */
    // SpectrumChart & SpectrumChartView
    QObject::connect(this->mainChartView, &SpectrumChartView::sendClickedXCoordinate, this, &MainWindow::showClickedData);
}

void MainWindow::setSerialWorkerSignalSlotConnection() {
    QObject::connect(this->serialWorker, &SerialWorker::sendData, this, &MainWindow::getSerialData);
    QObject::connect(this->mainWorker, &QThread::started, this->serialWorker, &SerialWorker::start);
    QObject::connect(this, &QMainWindow::destroyed, [this] {
        this->serialWorker->stop();
        this->mainWorker->quit();
        this->mainWorker->wait();
    });
}

void MainWindow::showSpectrumListManager() {
    // qDebug() << this->mainMCAData[0]->getLevelSeries();
    SpectrumListManager *dialog = new SpectrumListManager(this->mainMCAData);
    dialog->setModal(true);

    dialog->show();
}

void MainWindow::openMCAFile() {
    this->mainMCADataIndex ++;
    QString fileTypeFilter = "MCA file (*.mca *.txt *.csv) ;; All Files (*.*)";
    QString fileName = QFileDialog::getOpenFileName(this, "Open MCA Data File", QDir::currentPath(), fileTypeFilter);

    QStringList dataList;
    qDebug() << fileName;
    QFile dataFile(fileName);

    if(!dataFile.open(QFile::ReadOnly | QFile::Text)){
        if(!dataFile.exists()){
            qDebug() << "File Open Error!\n File Does Not Exists.";
        } else {
            qDebug() << "File Open Error!\n Unknown Error occured.";
        }
        return ;
    }

    QTextStream openFile(&dataFile);
    QString data;

    while(!openFile.atEnd()) {
        data = openFile.readLine();
        dataList.append(data.trimmed());
    }

    MCAFileStream *openMCAFile = new MCAFileStream(dataList, fileName.split(".")[1]);
    
    dataFile.close();

    this->dataCarrier = std::make_shared<LevelSeriesData>();
    this->dataCarrier->deepcopy(openMCAFile->getData());
    qDebug() << this->dataCarrier->getName();

    // Setting LevelSeriesData's Name
    QStringList nameList = fileName.split("/");
    this->dataCarrier->setName(nameList[nameList.size() - 1].split(".")[0]);

    this->mainChart->setChartWithLevelSeries(this->dataCarrier);

    this->mainMCAData.push_back(this->dataCarrier);

    this->basicInformationTab->getLevelSeriesData(this->mainMCAData[0]);

    qDebug() << "Check";
    qDebug() << this->mainMCAData[0]->getLevelSeries();
    qDebug() << this->mainMCAData[0]->getName();
}

void MainWindow::saveAsMCAFile() {
    QString filter =  "CSV File (*.csv) ;; Text File (*.txt) ;; MCA File (*.mca) ;; All Files (*.*)";
    QString defaultFilter = "CSV File (*.csv)";
    QString fileName = QFileDialog::getSaveFileName(this, "Save File", QDir::currentPath(), filter, &defaultFilter);

    qDebug() << fileName;

    MCAFileStream *saveFile = new MCAFileStream(this->mainMCAData[this->mainMCADataIndex]);

    if(fileName.split(".")[1] == "csv") saveFile->saveAsCSV(fileName);
    else if(fileName.split(".")[1] == "txt") saveFile->saveAsTXT(fileName);
}

void MainWindow::calculateGaussianDistributionWithArguments(int roiRegionIndex, int pointIndex) {
    std::vector<int> recentData;

    if(this->isLiveMeasuring) 
        recentData = this->liveMCAData->getLevelSeries();
    else
        recentData = this->mainMCAData[mainMCADataIndex]->getLevelSeries();

    std::pair<int, int> chosenROIRegion = roiRegions[roiRegionIndex];

    int start = chosenROIRegion.first, end = chosenROIRegion.second;

    std::vector<std::pair<int, int>> points;
    
    if(pointIndex == 0) { // counting points as 3
        for(int i=-1;i<=1;i++){
            points.push_back({start + i, recentData[start+i]});
            points.push_back({end + i, recentData[end+i]});
        }
    } else if(pointIndex == 1){
        for(int i=-2; i<=2; i++){
            points.push_back({start + i, recentData[start+i]});
            points.push_back({end + i, recentData[end+i]});
        }
    }

    std::pair<double, double> lineData = getLinearFunctionUsingLSM<int>(points);
    std::vector<double> normalizedData;
    double value;
    for(int i=start; i <= end; i++){
        value = (double)recentData[i] - (lineData.first * i + lineData.second);

        normalizedData.push_back(value);
    }

    std::pair<double, double> gaussianData = getGaussianDistributionUsingLSM<double>(normalizedData);

    double rawAreaSize = (double)getAreaSize(recentData);
    double areaSize = getAreaSize(normalizedData);

    std::vector<std::pair<double, double>> dataPacket;
    dataPacket.push_back(lineData);
    dataPacket.push_back(gaussianData);
    dataPacket.push_back({areaSize, rawAreaSize});

    emit sendGaussianDistributionData(dataPacket);
    qDebug() << dataPacket;
}

void MainWindow::getSerialSetter(SerialSetter setter){
    this->serialWorker = new SerialWorker(setter.portName);

    this->mainWorker = new QThread(this);
    this->serialWorker->moveToThread(this->mainWorker);

    this->setSerialWorkerSignalSlotConnection();
}

void MainWindow::getSerialData(const std::vector<int>& rawData) {
    this->liveMCAData = new LevelSeriesData(rawData);
    this->mainChart->setChartWithLevelSeries(this->liveMCAData->getLevelSeries());
}

void MainWindow::showClickedData(int xIndex) {
    int level = 0;
    try {
        if(isLiveMeasuring) {
            level = this->liveMCAData->getLevelSeries()[xIndex];
        } else if (this->mainMCADataIndex >= 0){
            level = this->mainMCAData[mainMCADataIndex]->getLevelSeries()[xIndex];
        } else {
            level = 0;
        }
    } catch (const std::exception& e) {
        level = 0;
        qDebug() << e.what();
    }

    
    this->yData->setText(QString::number(level));
    this->xData->setText(QString::number(xIndex));
}

void MainWindow::startDetection() {
    this->mainWorker->start();

    this->isLiveMeasuring = true;
    this->acquisition_changeStartStop->setText("End");
}

void MainWindow::stopDetection() {
    this->serialWorker->stop();
    this->mainWorker->quit();
    this->mainWorker->wait();

    this->isLiveMeasuring = false;
    this->acquisition_changeStartStop->setText("Start");
}

void MainWindow::showROIRegionManager() {
    ROIRegionManager *dialog = new ROIRegionManager(this->roiRegions);
    dialog->setModal(true);

    dialog->show();

    QObject::connect(dialog, &ROIRegionManager::sendROIRegions, this, &MainWindow::getROIRegions);
    QObject::connect(dialog, &ROIRegionManager::sendShowRegion, this, &MainWindow::setShowingROIRegion);
}

void MainWindow::getROIRegions(std::vector<std::pair<int, int>> roiRegions) {
    this->roiRegions = roiRegions;

    emit roiRegionChanged(roiRegions);

    qDebug() << this->roiRegions;
}

void MainWindow::setShowingROIRegion(int index) {
    this->showingROIRegionIndex = index;
}