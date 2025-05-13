#include "qth.h"
#include "windows.h"

DetectThread::DetectThread(QString port) {
    this->portName = port.toStdString();
    this->uart = new UartCommunicator(port.toStdString());
}

void DetectThread::stop() {
    running = false;
}

void DetectThread::start(){
    running = true;

    qDebug() << "Start";

    QByteArray buffer;
    int count = 0;

    while(true) {
        clock_t start = clock();
        if(this->uart->sendCommand("PD", this->spectrumCounter)) {
            QString response = this->uart->parsedResponse();
            // qDebug() << response;
            QStringList list = response.split("\n\r ");
            qDebug() << list.size();
            if(list.size() > this->spectrumCounter){
                qDebug() << "Data Get";
                for(auto line : list){
                    if(line == "") continue;
                    int value = line.trimmed().split(" ")[1].toInt();
                    // if(value >= 0 && value < 65536){
                    //     value = ((value - 32768) / 32) > 1023 ? 1023 : ((value - 32768) / 32);
                    //     data[value]++;
                    //     count++;
                    // }
                    // qDebug() << line;

                    data[value]++;
                    count++;

                }
            }
        }

        if(count >= this->spectrumCounter){
            std::vector<int> sendData(data, data + DATA_MAX_SIZE);
            emit setData(sendData);
            qDebug() << "Elapsed Time For analyzing : " << ((float)clock() - start)/CLOCKS_PER_SEC;
            qDebug() << "End";
            count = 0;
            buffer.clear();

            qDebug() << "Restart";
            // if(time.elapsed() >= 6000000) running = false;
            if(!running) {
                qDebug() << "Stop";
                return;
            }
        }
    }
}

void DetectThread::readSerial(){
    // if (!running || !serial->isOpen()) return;
    // while(running  && serial->bytesAvailable()){
    //     try{
           
    //         QString response = QString::fromUtf8(serial->readAll());
    //         qDebug() << response;
    //         // if(response.split(" ")[0] == "PD"){
    //         //     int value = response.split(" ")[1].toStdString().c_str()[0] << 8 + response.split(" ")[1].toStdString().c_str()[1];
    //         //     value /= 16;
    //         //     data[value]++;
    //         // }
    //         // emit setData(data);

    //         qDebug() << response;
    //         QStringList list;
    //         list = response.split("\n\r ");
    //         if(list.size() > 1){
    //             for(auto line : list){
    //                 int value = line.trimmed().split(" ")[1].toInt();
    //                 if(value >= 0 && value < 65536)
    //                     value = ((value - 32768) / 32) > 1023 ? 1023 : ((value - 32768) / 32);
    //                     data[value]++;
    //             }
    //         }

    //         std::vector<int> sendData(data, data + DATA_MAX_SIZE);
    //         emit setData(sendData);
            
    //     } catch (std::runtime_error &error){
    //         continue;
    //     }
    // }
}