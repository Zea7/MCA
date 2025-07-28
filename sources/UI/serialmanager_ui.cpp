#include "dialogs.h"
#include "styles.h"

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

    setTooltipUI();
}

void SerialManager::setSerialSettingLayoutUI() {
    this->serialTypeSettingComboBox = new QComboBox();
    this->serialPortSettingComboBox = new QComboBox();
    this->baudRateInput = new QLineEdit("115200");
    this->thresholdInput = new QLineEdit("33300");

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
    serialSettingHintLabel->setStyleSheet(STYLE_SERIALMANAGER_HINT_TITLE);

    QLabel *serialTypeSettingHintLabel = new QLabel("Select Serial Connect Type");
    QLabel *serialPortSettingHintLabel = new QLabel("Select COM Port");
    QLabel *baudRateHintLabel = new QLabel("Eneter Baud Rate");
    QLabel *thresholdHintLabel = new QLabel("Set Threshold Value");

    this->serialSettingLayout->addWidget(serialSettingHintLabel, 0, 0, 1, 1);
    this->serialSettingLayout->addWidget(serialTypeSettingHintLabel, 1, 0, 1, 1);
    this->serialSettingLayout->addWidget(this->serialTypeSettingComboBox, 1, 1, 1, 5);
    this->serialSettingLayout->addWidget(serialPortSettingHintLabel, 2, 0, 1, 1);
    this->serialSettingLayout->addWidget(this->serialPortSettingComboBox, 2, 1, 1, 5);
    this->serialSettingLayout->addWidget(baudRateHintLabel, 3, 0, 1, 1);
    this->serialSettingLayout->addWidget(this->baudRateInput, 3, 1, 1, 5);
    this->serialSettingLayout->addWidget(thresholdHintLabel, 4, 0, 1, 1);
    this->serialSettingLayout->addWidget(this->thresholdInput, 4, 1, 1, 5);
    this->serialSettingLayout->setColumnStretch(0, 1);
    this->serialSettingLayout->setAlignment(Qt::AlignCenter);
}

void SerialManager::setExperimentalSettingLayoutUI() {
    this->channelSizeSettingComboBox = new QComboBox();
    this->realTimeInput = new QLineEdit();
    this->backgroundSubstractFileSelectButton = new QPushButton("Choose File");
    this->countInput = new QLineEdit();

    QStringList channelSizeList;
    for(int i=DATA_MIN_SIZE; i<=DATA_MAX_SIZE; i*=2){
        channelSizeList << QString::number(i);
    }

    for(auto item : channelSizeList) {
        this->channelSizeSettingComboBox->addItem(item);
    }

    QLabel *experimentalSettingHintLabel = new QLabel("Experimental Settings");
    experimentalSettingHintLabel->setStyleSheet(STYLE_SERIALMANAGER_HINT_TITLE);
    
    QLabel *realTimeHintLabel = new QLabel("Set Measurement Time");
    QLabel *backgroundSubstractFileSelectHintLabel = new QLabel("Select Background Data File");
    QLabel *channelSizeSettingHintLabel = new QLabel("Select Channel Size");
    QLabel *countHintLabel = new QLabel("Set Measurement Counts");

    this->experimentalSettingLayout->addWidget(experimentalSettingHintLabel, 0, 0, 1, 1);
    this->experimentalSettingLayout->addWidget(realTimeHintLabel, 1, 0, 1, 1);
    this->experimentalSettingLayout->addWidget(this->realTimeInput, 1, 1, 1, 5);
    this->experimentalSettingLayout->addWidget(countHintLabel, 2, 0, 1, 1);
    this->experimentalSettingLayout->addWidget(this->countInput, 2, 1, 1, 5);
    this->experimentalSettingLayout->addWidget(channelSizeSettingHintLabel, 3, 0, 1, 1);
    this->experimentalSettingLayout->addWidget(this->channelSizeSettingComboBox, 3, 1, 1, 5);
    this->experimentalSettingLayout->addWidget(backgroundSubstractFileSelectHintLabel, 4, 0, 1, 1);
    this->experimentalSettingLayout->addWidget(this->backgroundSubstractFileSelectButton, 4, 1, 1, 5);
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

void SerialManager::setTooltipUI() {
    QIntValidator *thresholdValidator = new QIntValidator(0, 65535);
    this->thresholdInput->setValidator(thresholdValidator);
    this->thresholdInput->setToolTip("Threshold Value : 0 ~ 65535");
}