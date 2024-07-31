#ifndef DATA_H
#define DATA_H

#include <QStringList>
#include <QString>
#include <QDateTime>
#include <vector>

class MCAData {
public:
    MCAData(QStringList dataList);
    std::vector<int> getData();

private:
    QStringList dataList;
    std::vector<int> data;
    double liveTime = 0;
    double realTime = 0;
    double deadTime;
    QDateTime startTime;
    void parseData();
};

#endif