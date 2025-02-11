#ifndef QTH_H
#define QTH_H

#define DATA_MAX_SIZE 1024

#include <QThread>
#include <vector>
#include <cstring>

#include "serial.h"

class DetectThread : public QThread {
    Q_OBJECT

public:
    explicit DetectThread(UartCommunicator *uart);

    void stop();

private:
    void run();

    bool running = true;
    int data[16384];
    UartCommunicator *uart;

signals:
    void setData(std::vector<int> data);
};

#endif