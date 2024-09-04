#include "mainwindow.h"


MainWindow::MainWindow(){
    setUI();
}

void MainWindow::setUI(){
    resize(1700, 1000);
    this->mainWidget = new QWidget();
    this->mainLayout = new QGridLayout();

    this->bottomRemoteControl = new QGridLayout();
    this->sidePannel = new QVBoxLayout();

    QLabel *channel = new QLabel("Channel : ");
    QLabel *level = new QLabel("Level : ");
    QLabel *temp2 = new QLabel("side");

    this->xData = new QLabel("");
    this->yData = new QLabel("");


    this->bottomRemoteControl->addWidget(channel, 0, 0, 1, 1);
    this->bottomRemoteControl->addWidget(xData, 0, 1, 1,1);
    this->bottomRemoteControl->addWidget(level, 1, 0, 1,1);
    this->bottomRemoteControl->addWidget(yData, 1, 1, 1, 1);
    this->sidePannel->addWidget(temp2);



    this->mainWidget->setLayout(mainLayout);
    setCentralWidget(this->mainWidget);

    setWindowTitle("iMCA");


    setMCAMenuBar();

    setChartView();

    setSignalFromMenuBar();

    this->mainLayout->addWidget(this->mainChartView, 0, 0, 7, 7);
    this->mainLayout->addLayout(this->sidePannel, 0, 7, 7, 2);
    this->mainLayout->addLayout(this->bottomRemoteControl, 7, 0, 2, 9);
}

void MainWindow::setMCAMenuBar(){
    QMenuBar *menubar = new QMenuBar();
    this->toolbar = new ToolBar();

    menubar->addMenu(this->fileMenu = new FileMenu());
    menubar->addMenu(this->viewMenu = new ViewMenu());
    menubar->addMenu(this->acquisitionMenu = new AcquisitionMenu());
    menubar->addMenu(this->displayMenu = new DisplayMenu());
    menubar->addMenu(this->analyzeMenu = new AnalyzeMenu());
    menubar->addMenu(this->dppMenu = new DPPMenu());
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

    QObject::connect(this, SIGNAL(setROIRegion(std::vector<std::pair<int, int>>)), this->mainChart, SLOT(ROIRegionChange(std::vector<std::pair<int, int>>)));
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



}

void MainWindow::setSidePannel(QStringList list){
    clearLayout(this->sidePannel);
    for(auto &i : list){
        this->sidePannel->addWidget(new QLabel(i));
    }
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
    setSidePannel(list);

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

    emit setSampleRange(startSample, endSample);
    emit resizeXAxis();
}


void MainWindow::autoResizeYAxis() {
    emit setMaxMagnitude(maxMagnitude);
    emit resizeYAxis();
}







void MainWindow::openROIDialog(){
    DefineROIDialog *dialog = new DefineROIDialog(roiRegions);
    QObject::connect(dialog, SIGNAL(sendROIRegions(std::vector<std::pair<int, int>>)), this, SLOT(getROIRegions(std::vector<std::pair<int, int>>)));
    QObject::connect(dialog, SIGNAL(sendROIRegions(std::vector<std::pair<int, int>>)), this->mainChart, SLOT(ROIRegionChange(std::vector<std::pair<int, int>>)));
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
    emit setSampleRange(start, end);
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
    double areaSum = 0;
    for(int i=start;i<end;i++){
        double dataPoint = (double)data->getData()[i] - (linearFunc.first * i + linearFunc.second);
        areaSum += dataPoint;
    }
    for(int i=start;i<end;i++){
        double dataPoint = (double)data->getData()[i] - (linearFunc.first * i + linearFunc.second);
        dataset.push_back(dataPoint/areaSum * 1000);
    }

    std::pair<double, double> gaussian = fitGaussianDistribution(dataset);

    double FHWM = 2.3548200 * gaussian.second;

    this->roiRegions.push_back({start, end});
    setROIRegion(this->roiRegions);

    qDebug() << gaussian.first;
    qDebug() << gaussian.second;
    qDebug() << FHWM;

}