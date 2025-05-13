#ifndef SERIAL_H
#define SERIAL_H

#pragma comment (lib, "Setupapi.lib")

#include <windows.h>
#include <string>
#include <cstring>
#include <sstream>
#include <QDebug>
#include <vector>
#include <ctime>
#include <iostream>
#include <setupapi.h>

#include <QString>
#include <QStringList>

#define DEFAULT_BAUD_RATE CBR_115200
#define DEFAULT_CHANNEL 16384

const std::string PACKET_HEADER = "RISI";
const size_t BINARY_PACKET_SIZE = 2;

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

    std::string getPort() {return portName;}
    void closeConnection() {if(hSerial != INVALID_HANDLE_VALUE) CloseHandle(hSerial);}

    std::string receiveResponse();
    std::string receiveUDP();
    QString parsedResponse();
    std::vector<int> parseBinary(const std::vector<uint8_t>& buffer);

private:
    void setupSerialParams();
    bool sendConstructedCommand(const std::string &command);
    bool isUart();

    std::string portName;
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
            GUID guid = GUID_DEVINTERFACE_COMPORT;
            HDEVINFO hDevInfo = SetupDiGetClassDevs(&guid, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

            if (hDevInfo == INVALID_HANDLE_VALUE) {
                std::cerr << "SetupDiGetClassDevs failed." << std::endl;
            }

            SP_DEVINFO_DATA devInfoData;
            devInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

            for (DWORD i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &devInfoData); ++i) {
                char friendlyName[256];
                if (SetupDiGetDeviceRegistryPropertyA(
                        hDevInfo, &devInfoData, SPDRP_FRIENDLYNAME,
                        NULL, (PBYTE)friendlyName, sizeof(friendlyName), NULL)) {

                    std::string name(friendlyName);
                    // Friendly Name 예시: "USB Serial Device (COM3)"
                    if (name.find(port) != std::string::npos) {
                        qDebug() << "Port: " << port;
                        qDebug() << "Friendly Name: " << name;
                        ans.push_back(port);
                    }
                }
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
