#pragma once
#include "ImageData.h"
#include <memory>
namespace ImageIO
{
    extern void loadImage(const char* fileName, ImageData* outputData);
    extern void saveImage(const char* fileName, ImageData* inputData);
    extern std::unique_ptr<std::vector<unsigned char>> interweavePixels(const ImageData* inputData);
};

