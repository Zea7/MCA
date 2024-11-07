#ifndef QTH_H
#define QTH_H

#include <QThread>
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
    std::vector<int> data = std::vector<int>(1024, 0);
    UartCommunicator *uart;

signals:
    void setData(std::vector<int> data);
};

#endif