//#define cimg_use_png 1
//#include "CImg.h"
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

#define TEST_ITERATIONS 10

#define MEASURE_PERFORMANCE(func, name) {   \
                                            double performanceStartTime = omp_get_wtime();  \
                                            func                                            \
                                            double performanceEndTime = omp_get_wtime();    \
                                            std::cout << name <<" runtime: " << performanceEndTime - performanceStartTime << std::endl; \
                                        }


int main()
{
    ImageData imageData{};
    ImageData outputData{};
    //PerformanceProfiler profiler;

    ImageIO::loadImage("train.png", &imageData);
    
    OtsuThreshold::greyscaleImage(&imageData, &outputData);
    //auto result = profiler.profile(TEST_ITERATIONS, &OtsuThreshold::greyscaleImage, &imageData, &outputData);
    //std::cout << "Grayscale function:\n" << result.toString() << "\n\n";

    std::map<int, int> hist = {};
    //profiler.profileFunction(TEST_ITERATIONS, &OtsuThreshold::generateHistogram, &outputData, &hist);
    //std::cout << "Histogram function:\n" << d << "\n\n";
    OtsuThreshold::generateHistogram(&outputData, &hist);

    double targetThreshold = 0;
    targetThreshold = OtsuThreshold::otsuThreshold(hist, outputData.size);
    //result = profiler.profile(TEST_ITERATIONS, OtsuThreshold::otsuThresholdProfile, hist, outputData.size, &targetThreshold);
    //std::cout << "Otsu Threshold function:\n" << result.toString() << "\n\n";
    std::cout << "\nTarget Threshold: " << targetThreshold << std::endl;

    OtsuThreshold::applyThresholding(&outputData, &outputData, targetThreshold, { 0,0,0 }, { 255,255,255 });

    ImageIO::saveImage("trainout.png", &outputData);

    return 0;
}

//int main()
//{
//    ImageData imageData{};
//    ImageData outputData{};
//
//    PerformanceProfiler profiler;
//    //OtsuThreshold otsuThreshold;
//
//    ImageIO::loadImage("train.png", &imageData);
//
//    //OtsuThreshold::greyscaleImage(&imageData, &outputData);
//    double greyscaleSpeed = profiler.profileFunction(&OtsuThreshold::greyscaleImage, &imageData, &outputData);
//    std::cout << "Greyscale function runs in " << greyscaleSpeed << " seconds\n";
//
//    //double greyscaleSpeed = 0;
//    //otsuThreshold.greyscaleImage(&imageData, &outputData);
//    std::cout << "Greyscale function runs in " << greyscaleSpeed << " seconds\n";
//    //otsuThreshold.greyscaleImage(&imageData, &outputData);
//    //MEASURE_PERFORMANCE(greyscaleImage(&imageData, &outputData);, "Greyscale");
//
//    std::map<int, int> hist;
//    hist = OtsuThreshold::generateHistogram(&outputData);
//    //MEASURE_PERFORMANCE(hist = generateHistogram(&outputData, true); , "Generate Histogram");
//
//    double targetThreshold = 0;
//    targetThreshold = OtsuThreshold::otsuThreshold(hist, outputData.size);
//    //MEASURE_PERFORMANCE(targetThreshold = otsuThreshold(hist, imageData.size);, "Otsu's Thresholding");
//
//    //MEASURE_PERFORMANCE(targetThreshold = otsuThreshold_v2(hist, imageSize); , "Otsu's Thresholding (Paralleized)");
//
//    //threshold value found...
//    std::cout << "\nTarget Threshold: " << targetThreshold << std::endl;
//
//    //MEASURE_PERFORMANCE(applyThresholding(&outputData, &outputData, targetThreshold, { 0,0,0 }, { 255,255,255 });, "Apply Thresholding")
//    OtsuThreshold::applyThresholding(&outputData, &outputData, targetThreshold, { 0,0,0 }, { 255,255,255 });
//    
//    //cimg_library::CImg<unsigned char> outImg = cimg_library::CImg<unsigned char>(outputData.width, outputData.height, 1, 3, 0);;
//    ImageIO::saveImage("trainout.png", &outputData);
//    //outImg.display();
//
//    return 0;
//}