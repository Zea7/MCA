#include "qth.h"
#include "windows.h"

DetectThread::DetectThread(UartCommunicator *uart) {
    this->portName = uart->getPort();
    uart->closeConnection();
    this->serial = new QSerialPort(this);
    serial->setPortName(QString::fromUtf8(portName)); // 실제 포트 이름으로 변경
    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
}

void DetectThread::stop() {
    running = false;
}

void DetectThread::start(){
    if(!serial->isOpen()){
        if(!serial->open(QIODevice::ReadWrite)){
            return;
        }
    }
    running = true;
    QElapsedTimer time;
    time.start();
    serial->write("$PD 8000\n\r");
    qDebug() << "Start";

    QByteArray buffer;
    int count = 0;

    while(true) {
        if(serial->waitForReadyRead(50)) {
            buffer.append(serial->readAll());
            QStringList list;
            QString response = QString(buffer);
            // qDebug() << response;
            list = response.split("\n\r ");
            // qDebug() << list.size();
            if(list.size() > 8000){
                qDebug() << "Data Get";
                for(auto line : list){
                    int value = line.trimmed().split(" ")[1].toInt();
                    if(value >= 0 && value < 65536){
                        value = ((value - 32768) / 32) > 1023 ? 1023 : ((value - 32768) / 32);
                        data[value]++;
                        count++;
                    }

                }
            }
        }

        if(count >= 8000){
            std::vector<int> sendData(data, data + DATA_MAX_SIZE);
            emit setData(sendData);
            qDebug() << time.elapsed() / 1000;
            qDebug() << "End";
            count = 0;
            buffer.clear();

            qDebug() << "Restart";
            serial->write("$PD 8000\n\r");
            if(time.elapsed() >= 6000000) running = false;
            if(!running) {
                qDebug() << "Stop";
                return;
            }
        }
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