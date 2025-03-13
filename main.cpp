#include "CImg.h"
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <map>

#include <omp.h>
#include "PerformanceProfiler.h"
#include "ImageData.h"
#include "ImageIO.h"

#include "OtsuThreshold.h"

#define MEASURE_PERFORMANCE(func, name) {   \
                                            double performanceStartTime = omp_get_wtime();  \
                                            func                                            \
                                            double performanceEndTime = omp_get_wtime();    \
                                            std::cout << name <<" runtime: " << performanceEndTime - performanceStartTime << std::endl; \
                                        }


//struct ImageData {
//    std::vector<unsigned char> R;
//    std::vector<unsigned char> G;
//    std::vector<unsigned char> B;
//    SIZE_T size = 0;
//};

//void greyscaleImage(ImageData* inputData, ImageData* out);
//std::map<int, int> generateHistogram(ImageData* inputData, bool isGrayScale);
//double otsuThreshold(std::map<int, int> hist, int imageSize);
//double otsuThreshold_v2(std::map<int, int> hist, int imageSize);
//void applyThresholding(ImageData* inputData, ImageData* outputData, double thresholdValue, std::vector<int> low, std::vector<int> high);
//

int main()
{
    ImageData imageData{};
    ImageData outputData{};

    PerformanceProfiler profiler;
    OtsuThreshold otsuThreshold;

    ImageIO::loadImage("klcc.bmp", &imageData);

    //OtsuThreshold::greyscaleImage(&imageData, &outputData);
    //double greyscaleSpeed = profiler.profileFunction(greyscaleImage, &imageData, &outputData);
    //std::cout << "Greyscale function runs in " << greyscaleSpeed << " seconds\n";

    double greyscaleSpeed = 0;
    otsuThreshold.greyscaleImage(&imageData, &outputData);
    std::cout << "Greyscale function runs in " << greyscaleSpeed << " seconds\n";
    //otsuThreshold.greyscaleImage(&imageData, &outputData);
    //MEASURE_PERFORMANCE(greyscaleImage(&imageData, &outputData);, "Greyscale");

    std::map<int, int> hist;
    hist = otsuThreshold.generateHistogram(&outputData);
    //MEASURE_PERFORMANCE(hist = generateHistogram(&outputData, true); , "Generate Histogram");

    double targetThreshold = 0;
    targetThreshold = otsuThreshold.otsuThreshold(hist, outputData.size);
    //MEASURE_PERFORMANCE(targetThreshold = otsuThreshold(hist, imageData.size);, "Otsu's Thresholding");

    //MEASURE_PERFORMANCE(targetThreshold = otsuThreshold_v2(hist, imageSize); , "Otsu's Thresholding (Paralleized)");

    //threshold value found...
    std::cout << "\nTarget Threshold: " << targetThreshold << std::endl;

    //MEASURE_PERFORMANCE(applyThresholding(&outputData, &outputData, targetThreshold, { 0,0,0 }, { 255,255,255 });, "Apply Thresholding")
    otsuThreshold.applyThresholding(&outputData, &outputData, targetThreshold, { 0,0,0 }, { 255,255,255 });
    
    cimg_library::CImg<unsigned char> outImg = cimg_library::CImg<unsigned char>(outputData.width, outputData.height, 1, 3, 0);;
    ImageIO::saveImage("klccout.bmp", &outputData, &outImg);
    outImg.display();

    return 0;
}

//void greyscaleImage(ImageData* inputData, ImageData* out)
//{
//    out->size = inputData->size;
//    out->width = inputData->width;
//    out->height = inputData->height;
//
//    unsigned char luma = 0;
//    for (int i = 0; i < inputData->size; i++)
//    {
//        luma = 0.299 * inputData->R[i] + 0.587 * inputData->G[i] + 0.114 * inputData->B[i];
//
//        //assign luma to output
//        out->R.push_back(luma);
//        out->G.push_back(luma);
//        out->B.push_back(luma);
//        out->size += 1;
//    }
//}
//
////parallize
//std::map<int, int> generateHistogram(ImageData* inputData, bool isGrayScale = true)
//{
//    std::map<int, int> histogram;
//
//    for (int i = 0; i < 256; i++) {
//        histogram.insert({ i,0 });
//    }
//
//    //if grayscale, rgb values of a pixel are identical
//    //chose r as sample
//    for (int i = 0; i < inputData->size; i++) {
//        int value = inputData->R[i];
//        if (histogram.count(value) > 0)    //contains key
//        {
//            histogram[value]++;
//        }
//        else {
//            histogram.insert({ value, 1 });
//        }
//    }
//
//    return histogram;
//}
//
//double otsuThreshold(std::map<int, int> hist, int imageSize) {
//    //pre-calculations
//    double totalMean = hist[0] * 0;
//
//    std::map<int, double> valueProbability;
//    valueProbability.insert({ 0, (double)hist[0] / (double)imageSize });
//    for (int i = 1; i < 256; i++) {
//        valueProbability.insert({ i, (double)hist[i] / (double)imageSize });
//        totalMean += valueProbability[i] * i;
//    }
//
//    //--------------------------------------------------
//    int threshold = 0;
//    double w0 = 0, w1 = 0;
//    double mu0 = 0, mu1 = 0, mu = 0;
//
//    double interclassVariance = 0;
//    double highestVariance = 0;
//    double highestThreshold = 0;
//
////#pragma omp parallel
//    for (int i = 0; i < 256; i++)
//    {
//        threshold = i;
//        if (threshold == 0)
//            continue;
//
//        w0 += valueProbability[threshold];
//        w1 = 1 - w0;
//
//        mu += threshold * valueProbability[threshold];
//        mu0 = mu / w0;
//        mu1 = (totalMean - mu) / w1;
//
//        interclassVariance = w0 * w1 * pow(mu0 - mu1, 2);
//
//        if (interclassVariance > highestVariance)
//        {
//            highestVariance = interclassVariance;
//            highestThreshold = threshold;
//        }
//    }
//
//    return highestThreshold;
//}
//
//double otsuThreshold_v2(std::map<int, int> hist, int imageSize) {
//    //pre-calculations
//    double totalMean = hist[0] * 0;
//
//    std::map<int, double> valueProbability;
//    valueProbability.insert({ 0, (double)hist[0] / (double)imageSize });
//    for (int i = 1; i < 256; i++) {
//        valueProbability.insert({ i, (double)hist[i] / (double)imageSize });
//        totalMean += valueProbability[i] * i;
//    }
//
//    //--------------------------------------------------
//    int threshold = 0;
//    double w0 = 0, w1 = 0;
//    double mu0 = 0, mu1 = 0, mu = 0;
//
//    double interclassVariance = 0;
//    double highestVariance = 0;
//    double highestThreshold = 0;
//    for (auto d : hist) {
//        threshold = d.first;
//
//        w0 += valueProbability[threshold];
//        w1 = 1 - w0;
//
//        mu += threshold * valueProbability[threshold];
//        mu0 = mu / w0;
//        mu1 = (totalMean - mu) / w1;
//
//        interclassVariance = w0 * w1 * pow(mu0 - mu1, 2);
//
//        if (interclassVariance > highestVariance)
//        {
//            highestVariance = interclassVariance;
//            highestThreshold = threshold;
//        }
//    }
//
//    return highestThreshold;
//}
//
//
////assumes image is greyscale
//void applyThresholding(ImageData* inputData, ImageData* outputData, double thresholdValue, std::vector<int> low, std::vector<int> high)
//
//{
//    for (int i = 0; i < inputData->size; i++)
//    {
//        //check pixel brightness, assume grayscale, use R channel as sample
//        
//        //if higher than threshold
//        if (inputData->R[i] > thresholdValue)
//        {
//            outputData->R[i] = (high[0]);
//            outputData->G[i] = (high[1]);
//            outputData->B[i] = (high[2]);
//        } else if (inputData->R[i] <= thresholdValue)
//        {
//            outputData->R[i] = (low[0]);
//            outputData->G[i] = (low[1]);
//            outputData->B[i] = (low[2]);
//        }
//    }
//}