#pragma once
#include "ImageData.h"
#include <vector>
#include <map>


namespace OtsuThreshold
{
    extern void greyscaleImage(ImageData* inputData, ImageData* out);

    //parallize
    extern std::map<int, int> generateHistogram(ImageData* inputData, bool isGrayScale = true);

    extern double otsuThreshold(std::map<int, int> hist, int imageSize);

    //assumes image is greyscale
    extern void applyThresholding(ImageData* inputData, ImageData* outputData, double thresholdValue, std::vector<int> low, std::vector<int> high);
};

