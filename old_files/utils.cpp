#include "utils.h"

int getClosestInt(int a){
    int first;

    first = (a/100 +1) * 100;

    return first;
}

std::pair<int, int> getBorderByThreshold(std::vector<int> data, int threshold){
    int end, start;
    bool check = false;
    for(int i = 0; i<data.size();i++){
        if(data[i] >= threshold) {
            if(check){
                end = i;
            }
            else {
                check = true;
                start = i;
            }
            
        }
    }

    return std::make_pair(start, end);
}

void clearLayout(QLayout *layout) {
    QLayoutItem *item;
    while((item= layout->takeAt(0))) {
        if(item->layout()){
            clearLayout(item->layout());
        }
        if(item->widget()){
            delete item->widget();
        }
        delete item;
    }
}

PeakInfo peakInfo(int pos, int left_size, int right_size, int value)
{
    PeakInfo output;
    output.pos = pos;
    output.left_size = left_size;
    output.right_size = right_size;
    output.value = value;
    return output;
}

std::vector<PeakInfo> findPeaks(std::vector<int> data, int w_size){
    int size = w_size / 2;
    int up_1, up_2, down_1, down_2;

    std::vector<PeakInfo> output;

    int pre_state = 0;
    for(int i=size; i<data.size() - size; i++){
        int cur_state = data[i+size] - data[i-size];

        if(cur_state == 0)
            cur_state = 0;
        else  
            cur_state = cur_state > 0 ? 2 : 1;

        if(pre_state == 0 && cur_state == 2)
            up_1 = i;       // 단조 증가 시작점
        else if(pre_state == 2 && cur_state == 1){
            // 단조 감소 시작점
            up_2 = i-1;
            down_1 = i;
        }

        if(pre_state == 1 && (cur_state == 2 || cur_state == 0)){
            down_2 = i-1;
            int max_pos = up_2;
            if(data[up_2] < data[down_1])
                max_pos = down_1;

            PeakInfo peak = peakInfo(max_pos, up_2 - up_1 + 1, down_2 - down_1 + 1, data[max_pos]);

            output.push_back(peak);
            up_1 = i;
        }

        pre_state = cur_state;
    }

    return output;
}


double gaussian(double x, double mean, double sigma, double amplitude) {
    double exponent = -0.5 * std::pow((x - mean) / sigma, 2);
    return amplitude * std::exp(exponent);
}

double computeMean(const std::vector<double>& data) {
    double total_hist = std::accumulate(data.begin(), data.end(), 0.0);
    double expect = 0;
    for(int i=0; i < data.size(); i++){
        expect += data[i] * (i + 0.5);
    }
    return expect / total_hist;
}

// Function to compute the standard deviation of a vector
double computeStandardDeviation(const std::vector<double>& data, double mean) {
    double sum = 0.0;
    double total_hist = std::accumulate(data.begin(), data.end(), 0.0);
    for(int i=0;i<data.size();i++){
        sum += data[i] * (i - mean) * (i - mean);
    }
    return std::sqrt(sum / total_hist);
}

// Function to fit a Gaussian distribution to the given data
std::pair<double, double> fitGaussianDistribution(const std::vector<double>& data) {
    if (data.empty()) {
        throw std::invalid_argument("Data vector is empty.");
    }

    double sum = 0;
    for(int i=0;i<data.size();i++){
        sum += data[i];
    }
    std::vector<double> normalizedData;
    for(int i=0;i<data.size();i++){
        normalizedData.push_back(data[i] / sum);
    }

    double mean = computeMean(normalizedData);
    double stddev = computeStandardDeviation(normalizedData, mean);

    return { mean, stddev };
}