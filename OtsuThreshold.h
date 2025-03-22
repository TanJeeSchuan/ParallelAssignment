#pragma once
#include "ImageData.h"
#include <vector>
#include <map>


namespace OtsuThreshold
{
    extern void greyscaleImage(ImageData* inputData, ImageData* out);

    //parallize
    extern void generateHistogram(ImageData* inputData, std::map<int, int>* histogram);

    extern double otsuThreshold(std::map<int, int> hist, int imageSize);

    //assumes image is greyscale
    extern void applyThresholding(ImageData* inputData, ImageData* outputData, double thresholdValue, std::vector<int> low, std::vector<int> high);
};

namespace OtsuThreshold_v2
{
    extern void greyscaleImage(ImageData* inputData, ImageData* out);

    //parallize
    extern void generateHistogram(ImageData* inputData, std::map<int, int>* histogram);

    extern double otsuThreshold(std::map<int, int> hist, int imageSize);

    //assumes image is greyscale
    extern void applyThresholding(ImageData* inputData, ImageData* outputData, double thresholdValue, std::vector<int> low, std::vector<int> high);
};
