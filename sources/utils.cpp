#include "utils.h"

void fft(std::vector<Complex>& input, bool inverse){
    int N = input.size();

    for (int i=1, j=0; i < N; i++){
        int bit = N >> 1;

        for(; j & bit; bit >>= 1)
            j ^= bit;
        j ^= bit;

        if (i < j)
            std::swap(input[i], input[j]);
    }

    for (int len = 2; len <= N; len <<= 1) {
        double angle = 2 * M_PI / len * (inverse ? -1 : 1);
        Complex wlen(cos(angle), sin(angle));

        for (int i=0; i < N; i+=len){
            Complex w(1);
            for (int j = 0; j < len / 2; j++){
                Complex u = input[i + j];
                Complex v = input[i + j + len / 2] * w;
                input[i + j] = u + v;
                input[ i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }

    if (inverse) {
        for (Complex& x : input){
            x /= N;
        }
    }
}

void applyLowPassFilter(std::vector<Complex>& input, int cutoff){
    int N = input.size();

    for (int i = cutoff; i < N - cutoff; i++){
        input[i] = 0;
    }
}

std::vector<double> smoothHistogramFFT(const std::vector<double> &input, int cutoff){
    int N = input.size();

    std::vector<Complex> data(input.begin(), input.end());

    fft(data, false);

    applyLowPassFilter(data, cutoff);

    fft(data, true);

    std::vector<double> result(N);

    for (int i = 0; i < N; i++){
        result[i] = data[i].real();
    }

    return result;
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

std::vector<double> smoothHistogramMedian(const std::vector<double>& input, int windowSize) {
    std::vector<double> output(input.size());
    for (int i = 0; i < input.size(); ++i) {
        std::vector<double> window;
        for (int j = -windowSize; j <= windowSize; ++j) {
            int idx = i + j;
            if (idx >= 0 && idx < input.size()) {
                window.push_back(input[idx]);
            }
        }
        std::sort(window.begin(), window.end());
        output[i] = window[window.size() / 2];
    }
    return output;
}

std::vector<double> smoothHistogramGaussian(const std::vector<double>& input, double sigma) {
    int radius = std::ceil(3 * sigma);
    int size = 2 * radius + 1;
    std::vector<double> kernel(size);
    double sum = 0.0;

    for (int i = -radius; i <= radius; ++i) {
        kernel[i + radius] = std::exp(-(i * i) / (2 * sigma * sigma));
        sum += kernel[i + radius];
    }
    for (auto& k : kernel) k /= sum;

    std::vector<double> output(input.size(), 0.0);
    for (int i = 0; i < input.size(); ++i) {
        double value = 0.0;
        for (int j = -radius; j <= radius; ++j) {
            int idx = i + j;
            if (idx >= 0 && idx < input.size()) {
                value += input[idx] * kernel[j + radius];
            }
        }
        output[i] = value;
    }
    return output;
}