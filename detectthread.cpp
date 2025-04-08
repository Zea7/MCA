#include "qth.h"
#include "windows.h"

DetectThread::DetectThread(UartCommunicator *uart) {
    this->portName = uart->getPort();
    uart->closeConnection();

    QObject::connect(this->serial, &QSerialPort::readyRead, this, &DetectThread::readSerial);

    serial->setPortName(QString::fromUtf8(portName)); // 실제 포트 이름으로 변경
    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
}

void DetectThread::stop() {
    if(serial->isOpen()) serial->close();
    running = false;
}

void DetectThread::start(){
    if(!serial->isOpen()){
        if(!serial->open(QIODevice::ReadWrite)){
            return;
        }
    }
    running = true;

    if(serial->isOpen()){
        serial->write(QString("$PD 8000\n\r").toUtf8());
        serial->flush();
    }
}

void DetectThread::readSerial(){
    if (!running || !serial->isOpen()) return;
    while(running  && serial->bytesAvailable()){
        try{
           
            QString response = QString::fromUtf8(serial->readAll());
            qDebug() << response;
            // if(response.split(" ")[0] == "PD"){
            //     int value = response.split(" ")[1].toStdString().c_str()[0] << 8 + response.split(" ")[1].toStdString().c_str()[1];
            //     value /= 16;
            //     data[value]++;
            // }
            // emit setData(data);

            qDebug() << response;
            QStringList list;
            list = response.split("\n\r ");
            if(list.size() > 1){
                for(auto line : list){
                    int value = line.trimmed().split(" ")[1].toInt();
                    if(value >= 0 && value < 65536)
                        value = ((value - 32768) / 32) > 1023 ? 1023 : ((value - 32768) / 32);
                        data[value]++;
                }
            }

            std::vector<int> sendData(data, data + DATA_MAX_SIZE);
            emit setData(sendData);
            
        } catch (std::runtime_error &error){
            continue;
        }
    }
}