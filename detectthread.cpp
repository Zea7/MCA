#include "qth.h"
#include "windows.h"

DetectThread::DetectThread(UartCommunicator *uart) : uart(uart) {
    std::memset(data, 0, sizeof(data));
}

void DetectThread::stop() {
    running = false;
    quit();
    wait(1);
}

void DetectThread::run(){
    running = true;
    int getNum = 9654000;
    while(running  && this->uart != nullptr && getNum > 0){
        try{
            if(this->uart->sendCommand("PD", 8000)){
                Sleep(5000);
                QString response = QString::fromUtf8(this->uart->receiveResponse());
                qDebug() << response;
                // if(response.split(" ")[0] == "PD"){
                //     int value = response.split(" ")[1].toStdString().c_str()[0] << 8 + response.split(" ")[1].toStdString().c_str()[1];
                //     value /= 16;
                //     data[value]++;
                // }
                // emit setData(data);
                QStringList list;
                list = response.split("\n\r ");
                if(list.size() > 1){
                    for(auto line : list){
                        int value = line.trimmed().split(" ")[1].toInt();
                        if(value >= 32768 && value < 65536){
                            value = (int)((value - 32768) / 32);
                            data[value]++;
                        }

                    }
                }

                std::vector<int> sendData (data, data + DATA_MAX_SIZE * sizeof(data[0]));
                emit setData(sendData);

                
            }
        } catch (std::runtime_error &error){
            continue;
        }
    }
}