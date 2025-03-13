#include "ImageIO.h"

void ImageIO::loadImage(const char* fileName, ImageData* outputData)
{
    cimg_library::CImg<unsigned char> image(fileName);
    int imageSize = image.width() * image.height();

    outputData->width = image.width();
    outputData->height = image.height();

    outputData->R.reserve(imageSize);
    outputData->G.reserve(imageSize);
    outputData->B.reserve(imageSize);
    for (int i = 0; i < image.width(); i++)
    {
        for (int j = 0; j < image.height(); j++) {
            outputData->R.push_back(*image.data(i, j, 0));
            outputData->G.push_back(*image.data(i, j, 1));
            outputData->B.push_back(*image.data(i, j, 2));
            outputData->size++;
        }
    }
}

void ImageIO::saveImage(const char* fileName, ImageData* inputData, cimg_library::CImg<unsigned char>* out)
{
    cimg_library::CImg<unsigned char> *outputImage;
    if (out == nullptr)
        outputImage = new cimg_library::CImg<unsigned char>(inputData->width, inputData->height, 1, 3, 0);
    else
        outputImage = out;

    for (int i = 0; i < inputData->width; i++)
    {
        for (int j = 0; j < inputData->height; j++) {
            (*outputImage)(i, j, 0) = inputData->R[i * inputData->height + j];
            (*outputImage)(i, j, 1) = inputData->G[i * inputData->height + j];
            (*outputImage)(i, j, 2) = inputData->B[i * inputData->height + j];
        }
    }
    outputImage->save(fileName);
    //outputImage=?display();
}
