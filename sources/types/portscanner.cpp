#include "types.h"

MCAScanner::MCAScanner() {
    detectMCADevice();
}

void MCAScanner::detectMCADevice() {
    std::vector<std::string> ports;
    char targetPath[256];
    /* 
        연결 가능한 Window Port만 골라 UART인지 확인
    */
    for (int i=1; i<=256; i++){
        std::string port = "COM" + std::to_string(i);
        HANDLE hComm = CreateFileA(port.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

        if(hComm != INVALID_HANDLE_VALUE) {
            ports.push_back(port);
            CloseHandle(hComm);
        }


    }
    std::vector<std::pair<std::string, std::string>> ans;
    for (const auto& portName : ports){
        GUID guid = GUID_DEVINTERFACE_COMPORT;
        HDEVINFO hDevInfo = SetupDiGetClassDevs(&guid, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

        if (hDevInfo == INVALID_HANDLE_VALUE) {
            
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
                if (name.find(portName) != std::string::npos) {
                    qDebug() << "Port: " << portName;
                    qDebug() << "Friendly Name: " << name;
                    if(name.find("USB") != std::string::npos)
                        ans.push_back({portName, "USB"});
                    else if(name.find("UART") != std::string::npos)
                        ans.push_back({portName, "UART"});
                    else
                        ans.push_back({portName, "Unknown"});
                }
            }
        }
    }

    this->enablePorts = ans;
}