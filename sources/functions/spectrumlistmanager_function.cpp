#include "dialogs.h"

void SpectrumListManager::setSignalSlotConnection() {
    QObject::connect(this->addSpectrumButton, &QPushButton::clicked, this, &SpectrumListManager::openMCAFile);
}

void SpectrumListManager::openMCAFile(){
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

    this->data->deepcopy(openMCAFile->getData());

    qDebug() << this->data->getLevelSeries();

    this->previewChart->setChartWithLevelSeries(*this->data);
}
