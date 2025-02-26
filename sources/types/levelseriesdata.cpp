#include "types.h"

LevelSeriesData::LevelSeriesData() {

}

LevelSeriesData::LevelSeriesData(int channelSize) : channelSize(getSizeFromPolicy(channelSize)) {
    // qDebug() << "Is channel size right? " << this->checkSizePolicy(channelSize);
    // qDebug () << this->channelSize;
    
}

LevelSeriesData::LevelSeriesData(std::vector<int> rawDataSeries) : rawDataSeries(rawDataSeries){

}

LevelSeriesData::LevelSeriesData(int channelSize, std::vector<int> rawDataSeries) :
   channelSize(channelSize), rawDataSeries(rawDataSeries) {
    this->rawChannelSize = rawDataSeries.size();

}

LevelSeriesData::~LevelSeriesData() {
}

/* 
    public functions
*/

void LevelSeriesData::setRawDataSeriesWithLevelSeries(std::vector<int> rawDataSeries) {

}

/* 
    private functions
*/

void LevelSeriesData::setLevelSeries(){
    
}

bool LevelSeriesData::checkSizePolicy(int size){
    int counterPosition = 0, binarySize = size;
    while(binarySize > 1){
        counterPosition++;
        binarySize = binarySize >> 1;
    }
    return ((1 << counterPosition) == size);
}

int LevelSeriesData::getSizeFromPolicy(int size){
    int counterPosition = 0;
    while(size > 1){
        counterPosition++;
        size = size >> 1;
    }

    return 1 << counterPosition;
}