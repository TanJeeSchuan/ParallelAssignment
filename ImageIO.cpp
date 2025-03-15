//#include <sail-c++\sail-c++.h>
#define STB_IMAGE_IMPLEMENTATION 1
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION 1
#include "stb_image_write.h"
#include "ImageIO.h"
#include <iostream>

void ImageIO::loadImage(const char* fileName, ImageData* outputData)
{
    int x, y, n;
    unsigned char *data = stbi_load(fileName, &x, &y, &n, 0);
    int imageSize = x * y;
    int channelNum = n;

    std::vector<unsigned char> dataVector(data, data + imageSize);
    //auto* voidToVector = reinterpret_cast<std::vector<int>*>(data);

    //for (int i = 0; i < imageSize; i++) {
    //    std::cout << dataVector[i] << std::endl;
    //}

    outputData->R.reserve(imageSize);
    outputData->G.reserve(imageSize);
    outputData->B.reserve(imageSize);
    outputData->A.reserve(imageSize);
    outputData->width = x;
    outputData->height = y;
    outputData->size = imageSize;
    outputData->channels = channelNum;

    for (int i = 0; i < outputData->size; i++) {
        for (int j = 0; j < outputData->channels; j++) {
            switch(j)
            {
                case 0:
                    outputData->R.push_back(data[i * outputData->channels + j]);
                    break;
                case 1:
                    outputData->G.push_back(data[i * outputData->channels + j]);
                    break;
                case 2:
                    outputData->B.push_back(data[i * outputData->channels + j]);
                    break;
                case 3:
                    outputData->A.push_back(data[i * outputData->channels + j]);
                    break;
                default:
                    break;
            }
        }
    }
    return;
}
void ImageIO::saveImage(const char* fileName, ImageData* inputData)
{
    auto data = interweavePixels(inputData);
    stbi_write_png(fileName, inputData->width, inputData->height, inputData->channels, data->data(), inputData->width * inputData->channels);
    data.release();
}

std::unique_ptr<std::vector<unsigned char>> ImageIO::interweavePixels(const ImageData* inputData)
{
    std::unique_ptr<std::vector<unsigned char>> combined(new std::vector<unsigned char>());

    int n = inputData->channels;
    for (int i = 0; i < inputData->size; i++) {
        for (int j = 0; j < n; j++) {
            switch (j)
            {
            case 0:
                combined->push_back(inputData->R[i]);
                break;
            case 1:
                combined->push_back(inputData->G[i]);
                break;
            case 2:
                combined->push_back(inputData->B[i]);
                break;
            case 3:
                combined->push_back(inputData->A[i]);
                break;
            default:
                break;
            }
        }
    }

    return combined;
}




//
//void ImageIO::saveImage(const char* fileName, ImageData* inputData, cimg_library::CImg<unsigned char>* out)
//{
//    cimg_library::CImg<unsigned char> *outputImage;
//    if (out == nullptr)
//        outputImage = new cimg_library::CImg<unsigned char>(inputData->width, inputData->height, 1, 3, 0);
//    else
//        outputImage = out;
//
//    for (int i = 0; i < inputData->width; i++)
//    {
//        for (int j = 0; j < inputData->height; j++) {
//            (*outputImage)(i, j, 0) = inputData->R[i * inputData->height + j];
//            (*outputImage)(i, j, 1) = inputData->G[i * inputData->height + j];
//            (*outputImage)(i, j, 2) = inputData->B[i * inputData->height + j];
//        }
//    }
//    outputImage->save(fileName);
//    //outputImage=?display();
//}
