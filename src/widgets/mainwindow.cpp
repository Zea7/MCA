#include "mainwindow.h"


MainWindow::MainWindow(){
    setUI();
}

void MainWindow::setUI(){
    resize(1700, 1000);
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
}

void MainWindow::setSignalFromMenuBar(){
    QObject::connect(this->fileMenu, SIGNAL(openFile()), this, SLOT(openMCAFile()));
    QObject::connect(this->toolbar, SIGNAL(openFile()), this, SLOT(openMCAFile())); 

    QObject::connect(this->fileMenu, SIGNAL(saveFile()), this, SLOT(saveMCAFile()));
    QObject::connect(this->toolbar, SIGNAL(saveFile()), this, SLOT(saveMCAFile()));

    QObject::connect(this->toolbar, SIGNAL(resizeXAxis()), this, SLOT(autoResizeXAxis()));
    QObject::connect(this->toolbar, SIGNAL(resizeYAxis()), this, SLOT(autoResizeYAxis()));

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