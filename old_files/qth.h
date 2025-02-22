#ifndef QTH_H
#define QTH_H

#define DATA_MAX_SIZE 1024

#include <QThread>
#include <QString>
#include <QStringList>
#include <vector>

#include "serial.h"

class DetectThread : public QThread {
    Q_OBJECT

public:
    explicit DetectThread(UartCommunicator *uart);

    void stop();

private:
    void run();

    bool running = true;
    int data[DATA_MAX_SIZE] = {0};
    UartCommunicator *uart;
    QStringList responseList;

signals:
    void setData(std::vector<int> data);
};

#endif