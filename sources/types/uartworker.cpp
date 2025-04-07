#include "types.h"

UARTWorker::UARTWorker(std::string &portName) : portName(portName){
    this->serial = new QSerialPort();

    QObject::connect(this->serial, &QSerialPort::readyRead, this, &UARTWorker::readSerial);

    serial->setPortName(QString::fromUtf8(portName));
    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
}

UARTWorker::~UARTWorker() {
    stop();
}

void UARTWorker::start() {
    if(!serial->isOpen()) {
        if(!serial->open(QIODevice::ReadWrite)) {
            qWarning() << "Failed to open serial port!";
            return;
        }
    }

    running = true;
    if(serial->isOpen()){
        QString cmd("$PD 80000");
        
        serial->write(cmd.toUtf8());
        serial->flush();
    }
}

void UARTWorker::stop() {
    if(serial->isOpen()) serial->close();
    running = false;
}

void UARTWorker::readSerial(){
    if(!running | !serial->isOpen()) return;

    while(serial->bytesAvailable()){
        QByteArray data = serial->readAll();

        emit receivedData(data);
    }
    start();
}