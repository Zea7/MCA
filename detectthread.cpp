#include "qth.h"

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
            if(this->uart->sendCommand("PD", 8191)){
                QString response = QString::fromUtf8(this->uart->receiveResponse());

                if(response.split(" ")[0] == "PD"){
                    int value = response.split(" ")[1].toStdString().c_str()[0] << 8 + response.split(" ")[1].toStdString().c_str()[1];
                    value /= 16;
                    data[value]++;
                }
                emit setData(data);
            }
        } catch (std::runtime_error &error){
            break;
        }
    }
}