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
    this->mainLayout->addLayout(this->bottomRemoteControl, 0, 7, 7, 2);
    this->mainLayout->addLayout(this->sidePannel, 7, 0, 2, 9);
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
    this->mainChart = new QChart();
    this->mainChartView = new QChartView(this->mainChart);
    this->mainSeries = new QBarSeries();

    this->mainChart->addSeries(this->mainSeries);

    this->axisX = new QValueAxis;
    axisX->setRange(startSample, endSample);
    axisX->setLabelFormat("%g");
    axisX->setTitleText("Samples");

    this->axisY = new QValueAxis;
    axisY->setRange(0, maxMagnitude);
    axisY->setTitleText("Level");

    this->mainChart->addAxis(axisX, Qt::AlignBottom);
    this->mainSeries->attachAxis(axisX);
    this->mainChart->addAxis(axisY, Qt::AlignLeft);
    this->mainSeries->attachAxis(axisY);
    this->mainChart->legend()->hide();

    this->mainChartView->setRenderHint(QPainter::Antialiasing);
    this->mainSeries->setBarWidth(1.0);

}

void MainWindow::setSignalFromMenuBar(){
    QObject::connect(this->fileMenu, SIGNAL(openFile()), this, SLOT(openMCAFile()));
    QObject::connect(this->toolbar, SIGNAL(openFile()), this, SLOT(openMCAFile()));

    QObject::connect(this->toolbar, SIGNAL(resizeXAxis()), this, SLOT(resizeXAxis()));
    QObject::connect(this->toolbar, SIGNAL(resizeYAxis()), this, SLOT(resizeYAxis()));

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
    }

    QTextStream openFile(&dataFile);
    QString data;

    while(!openFile.atEnd()){
        data = openFile.readLine();
        dataList.append(data.trimmed());
        // qDebug() << data;
    }

    this->data = new MCAData(dataList);

    dataFile.close();

    std::vector<int> spectrumData = this->data->getData();
    QBarSet *testSet = new QBarSet("MCA Data");
    testSet->setColor(Qt::blue);
    testSet->setBorderColor(Qt::transparent);
    for(auto i : spectrumData){
        *testSet << i;
    }
    this->endSample = spectrumData.size();
    this->maxMagnitude = *std::max_element(spectrumData.begin(), spectrumData.end()); 
    this->mainSeries->append(testSet);

    resizeXAxis();
    resizeYAxis();
}

void MainWindow::resizeXAxis() {
    bool start = false;
    std::vector<int> data = this->data->getData();
    for(int i = 0; i<data.size();i++){
        if(data[i] >= this->threshold) {
            if(start){
                endSample = i;
            }
            else {
                start = true;
                startSample = i;
            }
            
        }
    }

    this->axisX->setRange(startSample, endSample);
    this->axisX->setLabelFormat("%g");
    this->axisX->setTitleText("Samples");
}


void MainWindow::resizeYAxis() {
    this->axisY->setRange(0, getClosestInt(this->maxMagnitude));
    this->axisY->setTitleText("Level");
}