#include "utils.h"

template <typename T>
std::pair<double, double> getLinearFunctionUsingLSM(std::vector<std::pair<T, T>> points){
    double xsum = 0, x2sum = 0, ysum = 0, xysum = 0;
    
    double x, y;

    int n = points.size();

    for(int i=0;i<n;i++){
        x = (double)points[i].first;
        y = (double)points[i].second;

        xsum += x;
        ysum += y;
        x2sum += x*x;
        xysum += x*y;
    }

    double a, b;    // y = ax + b;

    a = (n * xysum - xsum * ysum) / (n*x2sum - xsum*xsum);
    b = (x2sum * ysum - xsum * xysum) / (x2sum * n - xsum * xsum);

    return std::make_pair(a, b);
}

double getGaussianValue(double x, double mean, double sigma, double amplitude) {
    double exponent = -0.5 * std::pow((x - mean) / sigma, 2);
    return amplitude * std::exp(exponent);
}

template <typename T>
double computeMeanFromData(const std::vector<T>& data){
    std::vector<double> newData;
    for(auto i : data){
        newData.push_back((double)i);
    }

    double total_hist = std::accumulate(newData.begin(), newData.end(), 0.0);
    double expect = 0;

    for(int i=0; i < newData.size(); i++){
        expect += newData[i] * (i + 0.5);
    }
    return expect / total_hist;
}

template <typename T>
double computeStandardDeviationFromData(const std::vector<T>& data, double mean){
    std::vector<double> newData;
    for(auto i : data){
        newData.push_back((double)i);
    }

    double sum = 0.0;
    double total_hist = std::accumulate(newData.begin(), newData.end(), 0.0);

    for(int i=0; i < newData.size(); i++){
        sum += newData[i] * (i - mean) * (i - mean);
    }

    return std::sqrt(sum / total_hist);
}

template <>
double computeMeanFromData(const std::vector<double>& data){
    double total_hist = std::accumulate(data.begin(), data.end(), 0.0);
    double expect = 0;

    for(int i=0; i < data.size(); i++){
        expect += data[i] * (i + 0.5);
    }
    return expect / total_hist;
}

template <>
double computeStandardDeviationFromData(const std::vector<double>& data, double mean){
    double sum = 0.0;
    double total_hist = std::accumulate(data.begin(), data.end(), 0.0);

    for(int i=0; i < data.size(); i++){
        sum += data[i] * (i - mean) * (i - mean);
    }

    return std::sqrt(sum / total_hist);
}

template <typename T>
std::pair<double, double> getGaussianDistributionUsingLSM(const std::vector<T>& data){
    if(data.empty()) {
        throw std::invalid_argument("Data vector is empty");
    }

    std::vector<double> newData;

    double sum = 0;
    for(int i=0; i < data.size(); i++){
        sum += (double)data[i];
    }

    std::vector<double> normalizedData;
    for(int i=0; i < data.size(); i++){
        normalizedData.push_back((double)data[i] / sum);
    }

    double mean = computeMeanFromData<double>(normalizedData);
    double stddev = computeStandardDeviationFromData<double>(normalizedData, mean);

    return { mean, stddev };
}