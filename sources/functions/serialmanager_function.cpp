#include "dialogs.h"

void SerialManager::setSignalSlotConnection() {
    QObject::connect(this->cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    QObject::connect(this->OKButton, &QPushButton::clicked, this, &QDialog::accept);
    QObject::connect(this->OKButton, &QPushButton::clicked, this, &SerialManager::okSignal);
    QObject::connect(this->backgroundSubstractFileSelectButton, &QPushButton::clicked, this, &SerialManager::chooseBackgroundSubstractFile);
}

std::vector<std::string> SerialManager::getSerialPorts() {
    MCAScanner mcaScanner;
    std::vector<std::pair<std::string, std::string>> enablePorts = mcaScanner.getEnablePortsAndType();
    std::vector<std::string> ans;
    for(auto portData : enablePorts){
        std::string port = portData.first, type = portData.second;
        this->enablePorts.push_back(port);
        ans.push_back(port + " (" + type + ")");
    }

    return ans;
}

void SerialManager::okSignal() {
    this->portName = this->enablePorts[this->serialPortSettingComboBox->currentIndex()];
    qDebug() << this->portName;
    
    this->baudRate = this->baudRateInput->text().toInt();
    
    if(this->realTimeInput->text() != ""){
        this->realTime = this->realTimeInput->text().toInt();
    }

    SerialSetter setter = {this->portName, this->baudRate, this->realTime, this->backgroundSubstractFilePath};

    emit sendSerialSetter(setter);
}

void SerialManager::chooseBackgroundSubstractFile() {
    QString fileTypeFilter = "MCA file (*.mca *.txt *.csv) ;; All Files (*.*)";
    this->backgroundSubstractFilePath = QFileDialog::getOpenFileName(this, "Open MCA data file", QDir::currentPath(), fileTypeFilter);
}