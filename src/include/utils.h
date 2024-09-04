#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <QLayout>
#include <QWidget>

#include <cmath>
#include <limits>
#include <numeric>

int getClosestInt(int a);

std::pair<int, int> getBorderByThreshold(std::vector<int> data, int threshold);

void clearLayout(QLayout *layout);

struct PeakInfo 
{
    int pos;
    int left_size;
    int right_size;
    int value;
};

PeakInfo peakInfo(int pos, int left_size, int right_size, int value);

std::vector<PeakInfo> findPeaks(std::vector<int> data, int w_size = 5);

template <typename T>
std::pair<double, double> linearFitMethodWithLSM(std::vector<std::pair<T, T>> points);

template <typename T>
std::pair<double, double> linearFitMethodWithLSM(std::vector<std::pair<T, T>> points){
    double xsum=0, x2sum=0, ysum=0, xysum=0;
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

    double a, b;        // y = ax + b;
    a = (n * xysum - xsum * ysum) / (n*x2sum - xsum*xsum);
    b = (x2sum*ysum - xsum * xysum) / (x2sum * n - xsum * xsum);
    return std::make_pair(a, b);
}


double gaussian(double x, double mean, double sigma, double amplitude);

double computeMean(const std::vector<double>& data);
// Function to compute the standard deviation of a vector
double computeStandardDeviation(const std::vector<double>& data, double mean);
// Function to fit a Gaussian distribution to the given data
std::pair<double, double> fitGaussianDistribution(const std::vector<double>& data);

#endif