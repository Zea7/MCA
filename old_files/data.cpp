#include "data.h"

MCAData::MCAData(QStringList dataList, QString fileType) : dataList(dataList) {
    if(fileType == "txt" || fileType == "mca") parseData("-");
    else if(fileType == "csv") parseData(",");
}

MCAData::MCAData(){
    this->data = std::vector<int>();
}

QStringList MCAData::getHeaderData() {
    QStringList ret;
    ret << "LIVE_TIME - " + QString::number(liveTime) << "REAL_TIME - " + QString::number(realTime) 
    << "DEAD_TIME - " + QString::number(deadTime) << "START_TIME - " + startTime.toString("dd/MM/yyyy hh:mm:ss");

    return ret;
}

std::vector<int> MCAData::getData() {
    return this->data;
}

void MCAData::parseData(QString parser){
    bool isSpectrum = false;
    for(const auto& i: this->dataList){
        if(i.contains(parser) && i.split(parser)[1] != ""){
            QString head = i.split(parser)[0].trimmed();
            QString tail = i.split(parser)[1].trimmed();

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
        else if(i.contains("DATA")) isSpectrum = true;        

        if(isSpectrum) {
            QString input = i;
            if(i.split(parser).size() > 1)
                input = i.split(parser)[0].trimmed();
            this->data.push_back(input.toInt());
        }

    }

    if (liveTime && realTime) this->deadTime = realTime - liveTime;
    qDebug() << deadTime;
    qDebug() << this->startTime;
    qDebug() << this->data;
}

void MCAData::saveAsCSV(QString fileName){
    QFile file;
    file.setFileName(fileName);

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){

    }

    QTextStream out(&file);

    out << "<<MCA SPECTRUM>>\n";
    
    QString header;

    header = QString("LIVE_TIME,%1\nREAL_TIME,%2\nDEAD_TIME,%3\nSTART_TIME,%4\n").arg(liveTime).arg(realTime).arg(deadTime).arg(startTime.toString("dd/MM/yyyy hh:mm:ss"));

   
    out << header << "<<MCA DATA>>\n";
    for(int i : data){
        out << i << ",\n";
    }

    file.close();
}

void MCAData::saveAsTXT(QString fileName){
    QFile file;
    file.setFileName(fileName);

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){

    }

    QTextStream out(&file);

    out << "<<MCA SPECTRUM>>\n";
    
    QString header;

    header = QString("LIVE_TIME-%1\nREAL_TIME-%2\nDEAD_TIME-%3\nSTART_TIME-%4\n").arg(liveTime).arg(realTime).arg(deadTime).arg(startTime.toString("dd/MM/yyyy hh:mm:ss"));

   
    out << header << "<<MCA DATA>>\n";
    for(int i : data){
        out << i << "\n";
    }

    file.close();
}