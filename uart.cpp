#include "serial.h"

UartCommunicator::UartCommunicator(const std::string &portName){
    hSerial = CreateFileA(portName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    if (hSerial == INVALID_HANDLE_VALUE) {
        throw std::runtime_error("Error opening serial port");
    }

    setupSerialParams();
    isUart();
}

bool UartCommunicator::sendCommand(const std::string &cmd){
    std::stringstream ss;

    ss << "$" << cmd << "\n\r";

    std::string fullCmd = ss.str();

    return sendConstructedCommand(fullCmd);
}

bool UartCommunicator::sendCommand(const std::string &cmd, const int &param1){
    std::stringstream ss;

    ss << "$" << cmd << " " << std::hex << param1 << "\n\r";

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
    DWORD bytesRead;
    std::string response;

    do {
        if (!ReadFile(hSerial, buffer, sizeof(buffer), &bytesRead, NULL)){
            throw std::runtime_error("Error reading from serial port");
        }

        response.append(buffer, bytesRead);
    } while (bytesRead > 0);

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
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
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

    std::string response = receiveResponse();
    if (response == ACK) {
        qDebug() << "Received ACK";
        return true;
    } else {
        qDebug() << "Unknown Error";
        return false;
    }
    
}

bool UartCommunicator::isUart(){
    try{
        if(sendCommand("FPV")){
            QString response = QString::fromUtf8(receiveResponse());

            if(response.split(" ")[0] == "FPV"){
                this->FPGAVersion = response.split(" ")[1];
                if(sendCommand("FWV")){
                    response = QString::fromUtf8(receiveResponse());
                    this->FWV = response.split(" ")[1];
                }
                qDebug() << FPGAVersion;
                qDebug() << FWV;
                return true;
            }
        }
        this->live = false;
        qDebug() << "Is not UART";
        return false;
    } catch (const std::runtime_error& error) {
        this->live = false;
        qDebug() << "Is not UART";
        return false;
    }
}