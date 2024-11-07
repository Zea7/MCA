#include "dialogs.h"

SerialSettingDialog::SerialSettingDialog(){
    setUI();
    setComboBoxes();
}

SerialSettingDialog::~SerialSettingDialog(){}

void SerialSettingDialog::setUI(){
    this->resize(800, 150);
    this->mainLayout = new QHBoxLayout();
    this->serialSettingLayout = new QGridLayout();
    this->experimentalSettingLayout = new QGridLayout();

    this->typeSelectComboBox = new QComboBox();
    this->channelCountSelectComboBox = new QComboBox();
    this->portBox = new QComboBox();
    this->speedEnterLine = new QLineEdit("115200");
    this->realTimeEnterLine = new QLineEdit();
    this->selectBackgroundSubstractFile = new QPushButton("Find File");

    this->okButton = new QPushButton("&OK");
    this->cancelButton = new QPushButton("&Cancel");

    QObject::connect(this->okButton, &QPushButton::clicked, this, &QDialog::accept);
    QObject::connect(this->cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    QObject::connect(this->okButton, &QPushButton::clicked, this, &SerialSettingDialog::sendSerialPort);

    this->okCancelLayout = new QHBoxLayout();
    this->okCancelLayout->addStretch(2);
    this->okCancelLayout->addWidget(okButton);
    this->okCancelLayout->addWidget(cancelButton);

    this->outerLayout = new QVBoxLayout();

    QLabel *serialSettingLabel = new QLabel("Serial Settings");
    QLabel *typeLabel = new QLabel("Select Connect Type");
    QLabel *speedLabel = new QLabel("Enter Connection Speed");
    QLabel *portLabel = new QLabel("Selected COM Port");

    QLabel *experimentalSettingLabel = new QLabel("Experimental Settings");
    QLabel *realTimeLabel = new QLabel("Enter Measurement Time");
    QLabel *backgroundSubstractLabel = new QLabel("Choose Background Data File");
    QLabel *channelLabel = new QLabel("Select Channel Count");

    this->serialSettingLayout->addWidget(serialSettingLabel, 0, 0, 1, 1);
    this->serialSettingLayout->addWidget(typeLabel, 1, 0, 1 ,1);
    this->serialSettingLayout->addWidget(this->typeSelectComboBox, 1, 1, 1, 5);
    this->serialSettingLayout->addWidget(portLabel, 2, 0, 1, 1);
    this->serialSettingLayout->addWidget(this->portBox, 2, 1, 1, 5);
    this->serialSettingLayout->addWidget(speedLabel, 3, 0, 1 ,1);
    this->serialSettingLayout->addWidget(this->speedEnterLine, 3, 1, 1 ,5);
    this->serialSettingLayout->setColumnStretch(0, 1);
    this->serialSettingLayout->setAlignment(Qt::AlignCenter);

    this->experimentalSettingLayout->addWidget(experimentalSettingLabel, 0, 0, 1, 1);
    this->experimentalSettingLayout->addWidget(realTimeLabel, 1, 0, 1, 1);
    this->experimentalSettingLayout->addWidget(this->realTimeEnterLine, 1, 1, 1, 5);
    this->experimentalSettingLayout->addWidget(backgroundSubstractLabel, 2, 0, 1, 1);
    this->experimentalSettingLayout->addWidget(this->selectBackgroundSubstractFile, 2, 1, 1, 1);
    this->experimentalSettingLayout->addWidget(channelLabel, 3, 0, 1, 1);
    this->experimentalSettingLayout->addWidget(this->channelCountSelectComboBox, 3, 1, 1, 5);
    this->experimentalSettingLayout->setColumnStretch(0, 1);
    this->experimentalSettingLayout->setAlignment(Qt::AlignCenter);

    this->mainLayout->addLayout(this->serialSettingLayout);
    this->mainLayout->addLayout(this->experimentalSettingLayout);
    
    this->outerLayout->addLayout(this->mainLayout);
    this->outerLayout->addLayout(this->okCancelLayout);


    this->setLayout(this->outerLayout);
}

void SerialSettingDialog::setComboBoxes(){
    QStringList typeList;
    typeList << "UART" << "USB" << "Ethernet";

    for(auto item : typeList){
        this->typeSelectComboBox->addItem(item);
    }

    QStringList channelList;
    channelList << "1024" << "2048" << "4096" << "8192" << "16384" << "32768";
    for(auto item : channelList){
        this->channelCountSelectComboBox->addItem(item);
    }

    std::string port = getUARTPort();
    this->portBox->addItem(QString::fromUtf8(port));
}

std::string SerialSettingDialog::getUARTPort(){
    UartDetector *detector = new UartDetector();

    std::string port = detector->getPort();
    return port;
}

void SerialSettingDialog::sendSerialPort(){
    QString port = this->portBox->currentText();
    qDebug() << port;
    emit setSerialPort(port);
}