#include "dialogs.h"

SerialManager::SerialManager() {
    setUI();
    setSignalSlotConnection();
}

SerialManager::~SerialManager() {

}

void SerialManager::setUI() {
    this->resize(800, 150);
    
    this->mainLayout = new QGridLayout();
    this->serialSettingLayout = new QGridLayout();
    this->experimentalSettingLayout = new QGridLayout();
    this->OKCancelLayout = new QHBoxLayout();

    setOKCancelLayoutUI();
    setSerialSettingLayoutUI();
    setExperimentalSettingLayoutUI();

    this->mainLayout->addLayout(this->serialSettingLayout, 0, 0);
    this->mainLayout->addLayout(this->experimentalSettingLayout, 0, 1);
    this->mainLayout->addLayout(this->OKCancelLayout, 1, 1);

    this->setLayout(this->mainLayout);
}

void SerialManager::setSerialSettingLayoutUI() {
    this->serialTypeSettingComboBox = new QComboBox();
    this->serialPortSettingComboBox = new QComboBox();
    this->baudRateInput = new QLineEdit("115200");

    QStringList serialTypeList;
    serialTypeList << "UART" << "USB-C (CDC)" << "Ethernet";

    for(auto item : serialTypeList) {
        this->serialTypeSettingComboBox->addItem(item);
    }

    std::vector<std::string> portNames = getSerialPorts();
    for(auto item : portNames) {
        this->serialPortSettingComboBox->addItem(QString::fromUtf8(item));
    }

    QLabel *serialSettingHintLabel = new QLabel("Serial Settings");
    QLabel *serialTypeSettingHintLabel = new QLabel("Select Serial Connect Type");
    QLabel *serialPortSettingHintLabel = new QLabel("Select COM Port");
    QLabel *baudRateHintLabel = new QLabel("Eneter Baud Rate");

    this->serialSettingLayout->addWidget(serialSettingHintLabel, 0, 0, 1, 1);
    this->serialSettingLayout->addWidget(serialTypeSettingHintLabel, 1, 0, 1, 1);
    this->serialSettingLayout->addWidget(this->serialTypeSettingComboBox, 1, 1, 1, 5);
    this->serialSettingLayout->addWidget(serialPortSettingHintLabel, 2, 0, 1, 1);
    this->serialSettingLayout->addWidget(this->serialPortSettingComboBox, 2, 1, 1, 5);
    this->serialSettingLayout->addWidget(baudRateHintLabel, 3, 0, 1, 1);
    this->serialSettingLayout->addWidget(this->baudRateInput, 3, 1, 1, 5);
    this->serialSettingLayout->setColumnStretch(0, 1);
    this->serialSettingLayout->setAlignment(Qt::AlignCenter);
}

void SerialManager::setExperimentalSettingLayoutUI() {
    this->channelSizeSettingComboBox = new QComboBox();
    this->realTimeInput = new QLineEdit();
    this->backgroundSubstractFileSelectButton = new QPushButton("Choose File");

    QStringList channelSizeList;
    channelSizeList << "1024" << "2048" << "4096" << "8192" << "16384" << "32768";

    for(auto item : channelSizeList) {
        this->channelSizeSettingComboBox->addItem(item);
    }

    QLabel *experimentalSettingHintLabel = new QLabel("Experimental Settings");
    QLabel *realTimeHintLabel = new QLabel("Set Measurement Time");
    QLabel *backgroundSubstractFileSelectHintLabel = new QLabel("Select Background Data File");
    QLabel *channelSizeSettingHintLabel = new QLabel("Select Channel Size");

    this->experimentalSettingLayout->addWidget(experimentalSettingHintLabel, 0, 0, 1, 1);
    this->experimentalSettingLayout->addWidget(realTimeHintLabel, 1, 0, 1, 1);
    this->experimentalSettingLayout->addWidget(this->realTimeInput, 1, 1, 1, 5);
    this->experimentalSettingLayout->addWidget(channelSizeSettingHintLabel, 2, 0, 1, 1);
    this->experimentalSettingLayout->addWidget(this->channelSizeSettingComboBox, 2, 1, 1, 5);
    this->experimentalSettingLayout->addWidget(backgroundSubstractFileSelectHintLabel, 3, 0, 1, 1);
    this->experimentalSettingLayout->addWidget(this->backgroundSubstractFileSelectButton, 3, 1, 1, 5);
    this->experimentalSettingLayout->setColumnStretch(0, 1);
    this->experimentalSettingLayout->setAlignment(Qt::AlignCenter);
}

void SerialManager::setOKCancelLayoutUI() {
    this->OKButton = new QPushButton("&OK");
    this->cancelButton = new QPushButton("&Cancel");

    this->OKCancelLayout->addStretch(2);
    this->OKCancelLayout->addWidget(OKButton);
    this->OKCancelLayout->addWidget(cancelButton);
}