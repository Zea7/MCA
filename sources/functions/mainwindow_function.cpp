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