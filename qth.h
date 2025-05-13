#ifndef QTH_H
#define QTH_H

#define DATA_MAX_SIZE 1024

#include <QThread>
#include <QString>
#include <QStringList>
#include <vector>
#include <QSerialPort>
#include <QObject>
#include <QElapsedTimer>

#include "serial.h"

class DetectThread : public QObject {
    Q_OBJECT

public:
    explicit DetectThread(QString port);
    ~DetectThread(){stop();}

public slots:
    void start();
    void stop();

private slots:
    void readSerial();

private:
    bool running = true;
    int data[DATA_MAX_SIZE] = {0};
    std::string portName;
    QStringList responseList;
    UartCommunicator *uart;
    int spectrumCounter = 500;

signals:
    void setData(std::vector<int> data);
};

#endif