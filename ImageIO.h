#pragma once
#include "ImageData.h"
#include <memory>
namespace ImageIO
{
    void loadImage(const char* fileName, ImageData* outputData);
    void saveImage(const char* fileName, ImageData* inputData);
    std::unique_ptr<std::vector<unsigned char>> interweavePixels(const ImageData* inputData);
};

