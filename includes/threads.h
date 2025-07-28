#ifndef THREADS_H
#define THREADS_H

#include <QThread>
#include <QString>
#include <QStringList>
#include <vector>
#include <algorithm>
#include "types.h"
class SerialWorker : public QObject {
    Q_OBJECT

public:
    SerialWorker(std::string &portName);
    ~SerialWorker();

    bool isRunning() {return running;}

    std::string getPortName() {return this->portName;}

public slots:
    void start();
    void stop();

signals:
    void sendData(const std::vector<int>& data);

private slots:

private:
    SerialStream *serial;
    bool running = false;

    std::vector<int> readSerial();
    int rawData[DATA_MAX_SIZE] = {0};

    std::string portName;
    int countPerCycle = 1000;
};

#endif