#ifndef DATA_H
#define DATA_H

#include <QStringList>
#include <QString>
#include <QDateTime>
#include <QFileDialog>
#include <vector>

class MCAData {
public:
    MCAData(QStringList dataList, QString fileType);
    MCAData();
    std::vector<int> getData();
    QStringList getHeaderData();
    void saveAsCSV(QString fileName);
    void saveAsTXT(QString fileName);
    void setData(std::vector<int> data){this->data = data;}

private:
    QStringList dataList;
    std::vector<int> data;
    double liveTime = 0;
    double realTime = 0;
    double deadTime;
    QDateTime startTime;
    void parseData(QString parser);
};



#endif