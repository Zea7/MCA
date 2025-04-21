#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <cmath>
#include <limits>
#include <numeric>

template <typename T>
std::pair<double, double> getLinearFunctionUsingLSM(std::vector<std::pair<T, T>> points);

double getGaussinValue(double x, double mean, double sigma, double amplitude);

template <typename T>
double computeMeanFromData(const std::vector<T>& data);

template <typename T>
double computeStandardDeviationFromData(const std::vector<T>& data, double mean);

template <typename T>
std::pair<double, double> getGaussianDistributionUsingLSM(const std::vector<T>& data);

#endif