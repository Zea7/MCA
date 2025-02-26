#include "types.h"

LevelSeriesData::LevelSeriesData() {

}

LevelSeriesData::LevelSeriesData(int channelSize) : channelSize(channelSize) {

}

LevelSeriesData::LevelSeriesData(std::vector<int> rawDataSeries) : rawDataSeries(rawDataSeries){

}

LevelSeriesData::LevelSeriesData(int channelSize, std::vector<int> rawDataSeries) :
   channelSize(channelSize), rawDataSeries(rawDataSeries) {

}

LevelSeriesData::~LevelSeriesData() {
}

/* 
    public functions
*/

void setLevelSeriesWithRawData(std::vector<int> levelSeries) {

}

/* 
    private functions
*/

void setLevelSeries(){

}