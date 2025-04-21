#include "mainwindow.h"
#include "dialogs.h"

void MainWindow::dialogTest() {
    SpectrumListManager *dialog = new SpectrumListManager();
    dialog->setModal(true);

    dialog->show();
}

void MainWindow::setSignalSlotConnection() {
    /* 
        Signal Slot connection between MenuBar and Window
    */
    QObject::connect(this->file_open, &QAction::triggered, this, &MainWindow::openMCAFile);
    QObject::connect(this->file_saveAs, &QAction::triggered, this, &MainWindow::saveAsMCAFile);

    /* 
        Signal Slot connection between Tab widgets and Window
    */
    QObject::connect(this->regionInformationTab, &ROITabWidget::sendArgumentsToCalculateGaussianDistribution, this, &MainWindow::calculateGaussianDistributionWithArguments);
}

void MainWindow::openMCAFile() {
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

    LevelSeriesData *mcaData = new LevelSeriesData();
    mcaData->deepcopy(openMCAFile->getData());

    this->mainMCAData.push_back(mcaData);

    qDebug() << "Check";
    qDebug() << this->mainMCAData[0]->getLevelSeries();
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
    if(this->isLiveMeasuring) {
        std::vector<int> recentData = this->liveMCAData->getLevelSeries();

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

        
    } 
    else {

    }
}