#include "qth.h"
#include "windows.h"

DetectThread::DetectThread(UartCommunicator *uart) : uart(uart) {

}

void DetectThread::stop() {
    running = false;
    quit();
    wait(1);
}

void DetectThread::run(){
    running = true;
    while(running  && this->uart != nullptr){
        try{
            if(this->uart->sendCommand("PD", 8000)){
                QString response = QString::fromUtf8(this->uart->receiveResponse());
                qDebug() << response;
                // if(response.split(" ")[0] == "PD"){
                //     int value = response.split(" ")[1].toStdString().c_str()[0] << 8 + response.split(" ")[1].toStdString().c_str()[1];
                //     value /= 16;
                //     data[value]++;
                // }
                // emit setData(data);

                Sleep(5000);

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
            }
        } catch (std::runtime_error &error){
            break;
        }
    }
}