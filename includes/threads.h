#ifndef THREADS_H
#define THREADS_H

#include <QThread>
#include <QString>
#include <QStringList>
#include <vector>
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
    void receivedData(const QByteArray& data);

private slots:
    void readSerial();

private:
    SerialStream *serial;
    bool running = false;

    std::string portName;
};

#endif