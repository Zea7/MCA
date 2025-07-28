#include "threads.h"

SerialWorker::SerialWorker(std::string &portName) : portName(portName){
    this->serial = new SerialStream(this->portName);

}

SerialWorker::~SerialWorker() {
    stop();
}

void SerialWorker::start() {
    running = true;
    QElapsedTimer timer;
    timer.start();

    int count = 0;

    while (true) {
        if (this->serial->sendCommand("PD", this->countPerCycle)) {
            std::string response = this->serial->receiveResponse();

            qDebug() << response;

            auto it = std::search(response.begin(), response.end(), CDC_PACKET_HEADER.begin(), CDC_PACKET_HEADER.end());
            std::vector<uint8_t> udp_buffer(it, response.end());

            std::vector<int> responsedData = this->serial->parseDWORDData(udp_buffer);

            qDebug()  << responsedData;

            for(int i=0; i < responsedData.size(); i++){
                int index = responsedData[i] - 32768;
                if (index >= 0 && index < 32768) {
                    this->rawData[index]++;
                    count++;
                } else {
                    this->rawData[0]++;
                    count++;
                }
            }
        }

        /* 적합한 데이터가 한 사이클보다 많이 들어오면
        현재 사이클을 종료하고 count 초기화 및 data를 MainWindow로 전송 
        */
        if(count >= this->countPerCycle) {
            count = 0;
            std::vector<int> dataBus;
            for (int i=0;i<DATA_MAX_SIZE; i++){
                dataBus.push_back(this->rawData[i]);
            }
            qDebug() << "DataBUS" << dataBus;
            emit sendData(dataBus);

            /* running state를 확인한 후,
            running == false라면 통신 종료 
            */
            if(!running) {
                return;
            }
        }
    }
}

void SerialWorker::stop() {
    running = false;
}

std::vector<int> SerialWorker::readSerial(){
    std::string response = this->serial->receiveResponse();

    auto it = std::search(response.begin(), response.end(), CDC_PACKET_HEADER.begin(), CDC_PACKET_HEADER.end());
    std::vector<uint8_t> udp_buffer(it, response.end());

    return this->serial->parseDWORDData(udp_buffer);
}