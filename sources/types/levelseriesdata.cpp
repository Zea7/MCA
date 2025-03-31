#include "types.h"

LevelSeriesData::LevelSeriesData() {

}

LevelSeriesData::LevelSeriesData(int channelSize) : channelSize(getSizeFromPolicy(channelSize)) {
    // qDebug() << "Is channel size right? " << this->checkSizePolicy(channelSize);
    qDebug () << this->channelSize;
    
}

LevelSeriesData::LevelSeriesData(std::vector<int> rawDataSeries){
    this->rawChannelSize = getSizeFromPolicy(rawDataSeries.size());
    
    std::vector<int> data;

    for(int i=0; i<rawChannelSize; i++){
        data.push_back(rawDataSeries[i]);
    }

    this->rawDataSeries = data;
}

LevelSeriesData::LevelSeriesData(int channelSize, std::vector<int> rawDataSeries) :
   channelSize(getSizeFromPolicy(channelSize)) {
    this->rawChannelSize = getSizeFromPolicy(rawDataSeries.size());

    std::vector<int> data;
    for(int i=0; i<rawChannelSize; i++){
        data.push_back(rawDataSeries[i]);
    }

    this->rawDataSeries = data;

    this->setLevelSeries();
}

LevelSeriesData::~LevelSeriesData() {
    delete &rawDataSeries;
    delete &levelSeries;
}

/* 
    public functions
*/

void LevelSeriesData::setRawDataSeriesWithLevelSeries(std::vector<int> rawDataSeries) {
    this->rawChannelSize = getSizeFromPolicy(rawDataSeries.size());

}

/* 
    private functions
*/

void LevelSeriesData::setLevelSeries(){
    assert(checkSizePolicy(this->channelSize) && checkSizePolicy(this->rawChannelSize));

    int channelSizeRate = rawChannelSize / channelSize;
    if (channelSizeRate <= 1) {
        qDebug() << "Error! Input channel size is larger than original data channel size.";
        return ;
    }

    std::vector<int> data;
    for(int i=0;i<rawChannelSize;i+=channelSizeRate){
        int dataPoint = 0;
        for(int j=0;j<channelSizeRate;j++){
            dataPoint += this->rawDataSeries[i+j];
        }

        data.push_back(dataPoint);
    }

    this->levelSeries = data;
}

bool LevelSeriesData::checkSizePolicy(int size){
    int counterPosition = 0, binarySize = size;
    while(binarySize > 1){
        counterPosition++;
        binarySize = binarySize >> 1;
    }

    int resultSize = 1 << counterPosition;
    return (resultSize == size) && (resultSize >= DATA_MIN_SIZE) && (resultSize <= DATA_MAX_SIZE);
}

int LevelSeriesData::getSizeFromPolicy(int size){
    int counterPosition = 0;
    while(size > 1){
        counterPosition++;
        size = size >> 1;
    }

    int ans = 1 << counterPosition;

    return ans > DATA_MIN_SIZE ? (ans > DATA_MAX_SIZE ? DATA_MAX_SIZE : ans) : DATA_MIN_SIZE;
}