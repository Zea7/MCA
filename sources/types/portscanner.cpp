#include "types.h"

MCUScanner::MCUScanner() {
    detectMCUDevice();
}

void MCUScanner::detectMCUDevice() {
    std::vector<std::string> ports;
    char targetPath[256];
    /* 
        연결 가능한 Window Port만 골라 UART인지 확인
    */
    for (int i=1; i<=256; i++){
        std::string windowPort = "COM" + std::to_string(i); // Window가 가질 수 있는 외부 통신 port의 종류. COM1 ~ COM256까지 존재한다.
        HANDLE hComm = CreateFileA(windowPort.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

        if(hComm != INVALID_HANDLE_VALUE) {
            /* 
                검증이 필요한 코드
            */
            if(QueryDosDeviceA(windowPort.c_str(), targetPath, sizeof(targetPath))){
                qDebug () << targetPath;

                QString path(targetPath);
                if (path.contains("UART")) ports.push_back(windowPort);
            }
            CloseHandle(hComm);
        }


    }

    for (const auto& portName : ports){
        HANDLE hComm = CreateFileA(portName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

        if (hComm != INVALID_HANDLE_VALUE){
            DCB dcbSerialParams = {0};
            dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

            if(GetCommState(hComm, &dcbSerialParams)) {
                dcbSerialParams.BaudRate = DEFAULT_BAUD_RATE;
                dcbSerialParams.ByteSize = 8;
                dcbSerialParams.StopBits = ONESTOPBIT;
                dcbSerialParams.Parity = NOPARITY;

                if (SetCommState(hComm, &dcbSerialParams)) {
                    CloseHandle(hComm);
                    qDebug() << portName;
                    this->port.push_back(portName);
                }
            }
        }
    }
}