#include "threads.h"

SerialWorker::SerialWorker(std::string &portName) : portName(portName){
    this->serial = new SerialStream(this->portName);
}

SerialWorker::~SerialWorker() {
    stop();
}

void SerialWorker::start() {
    // running = true;
    // QElapsedTimer time;
    // time.start();

    // qDebug() << "Start";

    // QByteArray buffer;
    // int count = 0;

    // while(true) {
    //     if(this->serial->sendCommand("PD", this->spectrumCounter)) {
    //         QString response = this->serial->parsedResponse();
    //         // qDebug() << response;
    //         QStringList list = response.split("\n\r ");
    //         qDebug() << list.size();
    //         if(list.size() > this->spectrumCounter){
    //             qDebug() << "Data Get";
    //             for(auto line : list){
    //                 if(line == "") continue;
    //                 int value = line.trimmed().split(" ")[1].toInt();
    //                 // if(value >= 0 && value < 65536){
    //                 //     value = ((value - 32768) / 32) > 1023 ? 1023 : ((value - 32768) / 32);
    //                 //     data[value]++;
    //                 //     count++;
    //                 // }
    //                 qDebug() << line;

    //                 data[value]++;
    //                 count++;

    //             }
    //         }
    //     }

    //     if(count >= this->spectrumCounter){
    //         std::vector<int> sendData(data, data + DATA_MAX_SIZE);
    //         emit setData(sendData);
    //         qDebug() << time.elapsed() / 1000;
    //         qDebug() << "End";
    //         count = 0;
    //         buffer.clear();

    //         qDebug() << "Restart";
    //         if(time.elapsed() >= 6000000) running = false;
    //         if(!running) {
    //             qDebug() << "Stop";
    //             return;
    //         }
    //     }
    // }
}

void SerialWorker::stop() {
    running = false;
}

void SerialWorker::readSerial(){

}