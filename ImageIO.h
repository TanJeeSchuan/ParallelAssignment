#pragma once
#include "CImg.h"
#include "ImageData.h"

namespace ImageIO
{
    void loadImage(const char* fileName, ImageData* outputData);

    void saveImage(const char* fileName, ImageData* inputData, cimg_library::CImg<unsigned char>* out = nullptr);
};

