#ifndef SERIAL_H
#define SERIAL_H

#include <windows.h>
#include <string>
#include <cstring>
#include <sstream>
#include <QDebug>

#include <QString>

#define DEFAULT_BAUD_RATE CBR_115200
#define DEFAULT_CHANNEL 16384


class UartCommunicator {
    HANDLE hSerial;
    DCB dcbSerialParams;
    COMMTIMEOUTS timeouts;

    const std::string ACK = "ACK\n\r";

    bool live;

    QString FPGAVersion = "0";
    QString FWV = "0";

public:
    UartCommunicator(const std::string &portName);
    ~UartCommunicator(){
        if(hSerial != INVALID_HANDLE_VALUE){
            CloseHandle(hSerial);
        }
    }

    bool isLive(){return live;}

    bool testCommand();

    bool sendCommand(const std::string &cmd);
    bool sendCommand(const std::string &cmd, const std::string &param1);
    bool sendCommand(const std::string &cmd, const int &param1);
    bool sendCommand(const std::string &cmd, const std::string &param1, const std::string &param2);



    std::string receiveResponse();

private:
    void setupSerialParams();
    bool sendConstructedCommand(const std::string &command);
    bool isUart();
};

class UartDetector {
private:
    std::vector<std::string> port;

    

    std::vector<std::string> detectUartDevice() {
        std::vector<std::string> ports;
        std::vector<std::string> ans;
        for (int i=1;i<=256;i++){
            std::string port = "COM" + std::to_string(i);
            HANDLE hComm = CreateFileA(port.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

            if(hComm != INVALID_HANDLE_VALUE) {
                ports.push_back(port);
                CloseHandle(hComm);
            }
        }

        for (const auto& port : ports){
            HANDLE hComm = CreateFileA(port.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

            if (hComm != INVALID_HANDLE_VALUE) {
                DCB dcbSerialParams = {0};
                dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

                if(GetCommState(hComm, &dcbSerialParams)) {
                    dcbSerialParams.BaudRate = DEFAULT_BAUD_RATE;
                    dcbSerialParams.ByteSize = 8;
                    dcbSerialParams.StopBits = ONESTOPBIT;
                    dcbSerialParams.Parity = NOPARITY;

                    if (SetCommState(hComm, &dcbSerialParams)) {
                        CloseHandle(hComm);
                        qDebug() << port;
                        ans.push_back(port);
                    }
                }

                CloseHandle(hComm);
            }
        }
        return ans;
    }

public:
    UartDetector() {
        port = detectUartDevice();
    }

    ~UartDetector() {
    }

    std::vector<std::string> getPort(){return port;}

};

class Uart {
private:
    int PORT;
    void setTrigger(int num);
    
    

public:
    Uart(int port);
};

#endif
