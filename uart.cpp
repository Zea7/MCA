#include "serial.h"

UartCommunicator::UartCommunicator(const std::string &portName) : portName(portName){
    hSerial = CreateFileA(portName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    if (hSerial == INVALID_HANDLE_VALUE) {
        throw std::runtime_error("Error opening serial port");
    }

    setupSerialParams();
    isUart();
}

bool UartCommunicator::testCommand(){
    std::stringstream ss;

    ss << "m" << "\n\r";

    std::string fullCmd = ss.str();

    return sendConstructedCommand(fullCmd);
}

bool UartCommunicator::sendCommand(const std::string &cmd){
    std::stringstream ss;

    ss << "$" << cmd << "\n\r";

    std::string fullCmd = ss.str();

    return sendConstructedCommand(fullCmd);
}

bool UartCommunicator::sendCommand(const std::string &cmd, const int &param1){
    std::stringstream ss;

    ss << "$" << cmd << " " << param1 << "\n\r";

    std::string fullCmd = ss.str();

    return sendConstructedCommand(fullCmd);
}

bool UartCommunicator::sendCommand(const std::string &cmd, const std::string &param1){
    std::stringstream ss;

    ss << "$" << cmd << " " << param1 << "\n\r";

    std::string fullCmd = ss.str();

    return sendConstructedCommand(fullCmd);
}

bool UartCommunicator::sendCommand(const std::string &cmd, const std::string &param1, const std::string &param2){
    std::stringstream ss;

    ss << "$" << cmd << " " << param1 << " " << param2 << "\n\r";

    std::string fullCmd = ss.str();

    return sendConstructedCommand(fullCmd);
}

std::string UartCommunicator::receiveResponse() {
    char buffer[256];
    clock_t start = clock();
    DWORD bytesRead;
    std::string response;
    bool go = false;

    do {
        if (!ReadFile(hSerial, buffer, sizeof(buffer), &bytesRead, NULL)){
            throw std::runtime_error("Error reading from serial port");
        }
        response.append(buffer, bytesRead);
        
    } while (bytesRead > 0);
    qDebug() << "Elapsed Time : " << ((float)clock() - start)/CLOCKS_PER_SEC;


    return response;
}

std::vector<int> UartCommunicator::parseBinary(const std::vector<uint8_t> &buffer){
    std::vector<int> retVector;
    for(size_t i = 0; i + BINARY_PACKET_SIZE <= buffer.size(); i+=BINARY_PACKET_SIZE){
        std::string header (buffer.begin() + i, buffer.begin() + i + 4);
        if(header=="RISI"){
            uint16_t first = buffer[i+4] | buffer[i+5] << 8;
            uint8_t second = buffer[i+6];
            uint8_t third = buffer[i+7];
            qDebug() << "First : " << first << " Second : " << second << " Third : " << third;
            for(int j=0;j<first;j++){
                uint16_t data = buffer[i+8+j*2] | buffer[i+7+j*2] << 8;
                retVector.push_back((int)data);
                // qDebug() << "Data : " << data;
            }
        }
    }

    return retVector;

}

std::string UartCommunicator::receiveUDP() {
    clock_t start = clock();
    char buffer[256];
    DWORD bytesRead;
    std::string response;
    bool go = false;

    do {
        if (!ReadFile(hSerial, buffer, sizeof(buffer), &bytesRead, NULL)){
            throw std::runtime_error("Error reading from serial port");
        }
        response.append(buffer, bytesRead);
        
    } while (bytesRead > 0);
    clock_t end = clock();
    qDebug() << ((float)end-start)/CLOCKS_PER_SEC;
    auto it = std::search(response.begin(), response.end(), PACKET_HEADER.begin(), PACKET_HEADER.end());

    std::vector<uint8_t> udp_buffer(it, response.end());
    parseBinary(udp_buffer);

    return response;
}


/* 

    private

*/

void UartCommunicator::setupSerialParams() {
    dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hSerial, &dcbSerialParams)) {
        CloseHandle(hSerial);
        throw std::runtime_error("Error getting serial port state");
    }

    dcbSerialParams.BaudRate = DEFAULT_BAUD_RATE;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        CloseHandle(hSerial);
        throw std::runtime_error("Error setting serial port state");
    }

    timeouts = { 0 };
    timeouts.ReadIntervalTimeout = 5;
    timeouts.ReadTotalTimeoutConstant = 10;
    timeouts.ReadTotalTimeoutMultiplier = 1;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hSerial, &timeouts)){
        CloseHandle(hSerial);
        throw std::runtime_error("Error setting timeouts");
    }

    live = true;
}

bool UartCommunicator::sendConstructedCommand(const std::string &command){
    DWORD bytesWritten;
    
    if(!WriteFile(hSerial, command.c_str(), command.length(), &bytesWritten, NULL)){
        throw std::runtime_error("Error writing to serial port.");
    }
    qDebug() << "Sent: " << command;

    return true;
    
}

QString UartCommunicator::parsedResponse(){
    std::string response = this->receiveResponse();
    auto it = std::search(response.begin(), response.end(), PACKET_HEADER.begin(), PACKET_HEADER.end());

    std::vector<uint8_t> udp_buffer(it, response.end());
    std::vector<int> data = parseBinary(udp_buffer);

    QString ret = "";
    for(int i=0; i<data.size();i++){
        ret += QString::number(i) + " " + QString::number(data[i]) + "\n\r ";
    }
    qDebug() << ret;
    return ret;
}

bool UartCommunicator::isUart(){
    // return true;
    try{
        // if(testCommand()){
        //     QString response = QString::fromUtf8(receiveResponse());

        //     qDebug() << response;
        // }
        // if(sendCommand("ID")){
        //     QString response = QString::fromUtf8(receiveResponse());

        //     qDebug() << response;
        //     if(sendCommand("FWV")){
        //         QString response = QString::fromUtf8(receiveResponse());

        //         qDebug() << response;

                // if(sendCommand("FPV")){
                //     QString response = QString::fromUtf8(receiveResponse());

                //     qDebug() << response;
                // }
                // if(sendCommand("TRI", 33300)){
                //     QString response = QString::fromUtf8(receiveResponse());

                //     qDebug() << response;
                // }

                // if(sendCommand("Status")){
                //     QString response = QString::fromUtf8(receiveResponse());

                //     qDebug() << response;
                // }
        if(sendCommand("PD 8000")){
            clock_t start = clock();
            std::string response = receiveUDP();
            qDebug() << response;
            // qDebug() << response;
            clock_t end = clock();
        }   

        this->live = false;
        return true;

        //     }
        //     return true;
        
        // }
        this->live = false;
        qDebug() << "Is not UART";
        return false;
    } catch (const std::runtime_error& error) {
        this->live = false;
        qDebug() << "Is not UART";
        return false;
    }

}