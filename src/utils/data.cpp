#include "data.h"

MCAData::MCAData(QStringList dataList) : dataList(dataList) {
    parseData();
}

std::vector<int> MCAData::getData() {
    return this->data;
}

void MCAData::parseData(){
    bool isSpectrum = false;
    for(const auto& i: this->dataList){
        if(i.contains("-")){
            QString head = i.split("-")[0].trimmed();
            QString tail = i.split("-")[1].trimmed();

            qDebug() << head << tail;
            QStringList compare;
            compare << "LIVE_TIME" << "REAL_TIME" << "START_TIME";

            QString timeFormat = "dd/MM/yyyy HH:mm:ss";
            switch(compare.indexOf(head)) {
                case 0:
                    this->liveTime = tail.toDouble();
                    break;

                case 1:
                    this->realTime = tail.toDouble();
                    break;

                case 2: 
                    this->startTime = QDateTime::fromString(tail, timeFormat);
                    break;

                default:
                    qDebug() << "File Header Error! Could not Find Right Header.";
                    break;

            }
        }
        else if(i == "<<MCA DATA>>") isSpectrum = true;
        

        if(isSpectrum) {
            this->data.push_back(i.toInt());
        }

    }

    if (liveTime && realTime) this->deadTime = realTime - liveTime;
    qDebug() << deadTime;
    qDebug() << this->startTime;
    qDebug() << this->data;
}