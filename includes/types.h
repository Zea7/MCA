#ifndef TYPES_H
#define TYPES_H

#define DATA_MAX_SIZE 1<<16  // 65536
#define DATA_MIN_SIZE 1<<10  // 1024
#define DEFAULT_BAUD_RATE CBR_115200
#define DEFAULT_CHANNEL 16384

#include <vector>

#include <QDebug>
#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QDate>
#include <QFile>

#include <windows.h>
#include <string>
#include <cstring>
#include <sstream>

#include <QSerialPort>

class LevelSeriesData {
/* 
    MCA 프로그램의 기본적인 데이터를 다루는 클래스.
    각 채녈 별 레벨 데이터를 시리즈로 저장하여 다루며, 
    데이터 관련 기능들을 갖추는 클래스

    모든 데이터의 입력은 raw data로 받으며,
    해당 rqw data를 원하는 채널 수에 맞추어 가공을 거친 뒤
    보여주는 방식으로 진행.

    * Params *
    int channelSize
    std::vector<int> levelSeries
    std::vector<int> rawDataSeries
    
    std::vector<std::pair<int, int>> roiRegions
    channel 변환 데이터는 가지고 있어야할지는 나중에 TODO

    * Functions *

*/

public:
    LevelSeriesData();
    LevelSeriesData(int channelSize);
    LevelSeriesData(std::vector<int> rawDataSeries);
    LevelSeriesData(int channelSize, std::vector<int> rawDataSeries);
    LevelSeriesData(std::vector<int> rawDataSeries, double liveTime, double realTime, double deadTime, QDateTime startTime);
    LevelSeriesData(std::vector<int> rawDataSeries, double liveTime, double realTime, QDateTime startTime);

    ~LevelSeriesData();

    void setRawDataSeriesWithLevelSeries(std::vector<int> rawDataSeries);

    /* 
        On Line Functions

        void setChannelSize(int channelSize)    :   
        int getChannelSize()                    :   
        std::vector<int> getLevelSeries()       : 
    */
    void setChannelSize(int channelSize) {this->channelSize = channelSize;}
    int getChannelSize() {return this->channelSize;}
    int getRawChannelSize() {return this->rawChannelSize;}
    std::vector<int> getLevelSeries() {return this->levelSeries;}
    std::vector<int> getRawDataSeries() {return this->rawDataSeries;}
    double getLiveTime() {return this->liveTime;}
    double getDeadTime() {return this->deadTime;}
    double getRealTime() {return this->realTime;}
    QDateTime getStartTime() {return this->startTime;}
    void setLevelSeries(std::vector<int> levelSeries) {this->levelSeries = levelSeries;}
    void deepcopy(LevelSeriesData *data) {
        this->channelSize = data->getChannelSize();
        this->rawChannelSize = data->getRawChannelSize();
        this->rawDataSeries = data->getRawDataSeries();
        this->levelSeries = data->getLevelSeries();
        this->startTime = data->getStartTime();
        this->liveTime = data->getLiveTime();
        this->realTime = data->getRealTime();
        this->deadTime = data->getDeadTime();
    }

    QStringList getHeaderData();

private:
    int channelSize = DATA_MIN_SIZE;
    int rawChannelSize = 0;
    std::vector<int> levelSeries;
    std::vector<int> rawDataSeries;

    std::vector<std::pair<int, int>> roiRegions;

    void setLevelSeries();
    bool checkSizePolicy(int size);
    int getSizeFromPolicy(int size);

    double liveTime = 0;
    double realTime = 0;
    double deadTime = 0;
    QDateTime startTime;
};

class MCAFileStream {
public:
    MCAFileStream(QStringList dataList, QString fileType);
    MCAFileStream(LevelSeriesData *seriesData);
    
    void saveAsCSV(QString fileName);
    void saveAsTXT(QString fileName);
    void setData(std::vector<int> data) {this->data = data;}
    LevelSeriesData *getData() {return this->seriesData;}

private:
    QStringList dataList;
    LevelSeriesData *seriesData;
    std::vector<int> data;

    void parseData(QString parser);

    double liveTime = 0;
    double realTime = 0;
    double deadTime;
    QDateTime startTime;
};

class UARTStream {
    HANDLE hSerial;
    DCB dcbSerialParams;
    COMMTIMEOUTS timeouts;

    const std::string ACK = "ACK\n\r";

    bool live;

    QString FPGAVersion = "0";
    QString FWV = "0";

    std::string portName;

public:
    UARTStream(const std::string &portName);
    ~UARTStream() {
        if(hSerial != INVALID_HANDLE_VALUE){
            CloseHandle(hSerial);
        }
    }

    bool isLive() {return live;}

    bool sendCommand(const std::string &cmd);
    bool sendCommand(const std::string &cmd, const std::string &param1);
    bool sendCommand(const std::string &cmd, const int &param1);
    bool sendCommand(const std::string &cmd, const std::string &param1, const std::string &param2);

    std::string getPortName() {return this->portName;}

    std::string receiveResponse();

private:
    void setupSerialParams();
    bool sendConstructedCommand(const std::string &command);
    bool isMCADevice();
};

class PortScanner {
protected:
    std::vector<std::string> port;

public:
    PortScanner(){}

    std::vector<std::string> getPort() {return port;}
};

class UARTScanner : public PortScanner {
public:
    UARTScanner();

private:
    void detectUARTDevice();
};

class UARTWorker : public QObject {
    Q_OBJECT

public:
    UARTWorker(std::string &portName);
    ~UARTWorker();

    bool isRunning() {return running;}

    std::string getPortName() {return this->portName;}

public slots:
    void start();
    void stop();

signals:
    void receivedData(const QByteArray& data);

private slots:
    void readSerial();

private:
    QSerialPort *serial;
    bool running = false;

    std::string portName;
};

#endif