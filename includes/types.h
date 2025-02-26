#ifndef TYPES_H
#define TYPES_H

#define DATA_MAX_SIZE 1<<16  // 65536
#define DATA_MIN_SIZE 1<<10  // 1024

#include <vector>

class LevelSeriesData {
/* 
    MCA 프로그램의 기본적인 데이터를 다루는 클래스.
    각 채녈 별 레벨 데이터를 시리즈로 저장하여 다루며, 
    데이터 관련 기능들을 갖추는 클래스

    모든 데이터의 입력은 raw data로 받으며,
    해당 rqw data를 원하는 채널 수에 맞추어 가공을 거친 뒤
    보여주는 방식으로 진행.

    * Params *
    int channelSize
    std::vector<int> levelSeries
    std::vector<int> rawDataSeries
    
    std::vector<std::pair<int, int>> roiRegions
    channel 변환 데이터는 가지고 있어야할지는 나중에 TODO

    * Functions *

*/

public:
    LevelSeriesData();
    LevelSeriesData(int channelSize);
    LevelSeriesData(std::vector<int> rawDataSeries);
    LevelSeriesData(int channelSize, std::vector<int> rawDataSeries);
    ~LevelSeriesData();

    void setRawDataSeriesWithLevelSeries(std::vector<int> rawDataSeries);

    /* 
        On Line Functions

        void setChannelSize(int channelSize)    :   
        int getChannelSize()                    :   
        std::vector<int> getLevelSeries()       : 
    */
    void setChannelSize(int channelSize) {this->channelSize = channelSize;}
    int getChannelSize() {return this->channelSize;}
    int getRawChannelSize() {return this->rawChannelSize;}
    std::vector<int> getLevelSeries() {return this->levelSeries;}
    void setLevelSeries(std::vector<int> levelSeries) {this->levelSeries = levelSeries;}

private:
    int channelSize;
    int rawChannelSize;
    std::vector<int> levelSeries;
    std::vector<int> rawDataSeries;

    std::vector<std::pair<int, int>> roiRegions;

    void setLevelSeries();
};

#endif