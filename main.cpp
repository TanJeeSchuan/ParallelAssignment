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


void threshold(const char* filename, const char* outputname);
void threshold_profile(const char* filename, const char* outputname);
void threshold_profile_v2(const char* filename, const char* outputname);

int main()
{
    threshold_profile("train.png", "output-train.png");

    threshold_profile_v2("train.png", "output-train.png");

    return 0;
}

void threshold(const char* filename, const char* outputname) {
    ImageData imageData{};
    ImageData outputData{};

    ImageIO::loadImage(filename, &imageData);

    OtsuThreshold::greyscaleImage(&imageData, &outputData);

    std::map<int, int> hist = {};
    OtsuThreshold::generateHistogram(&outputData, &hist);

    double targetThreshold = 0;
    targetThreshold = OtsuThreshold::otsuThreshold(hist, outputData.size);
    //std::cout << "\nTarget Threshold: " << targetThreshold << std::endl;

    OtsuThreshold::applyThresholding(&outputData, &outputData, targetThreshold, { 0,0,0 }, { 255,255,255 });

    ImageIO::saveImage(outputname, &outputData);
}

void threshold_profile(const char* filename, const char* outputname) {
    double iOTime = 0, otsuTime = 0, totalTime = 0;
    double start, end, total;
    ImageData imageData{};
    ImageData outputData{};

    start = omp_get_wtime();
        ImageIO::loadImage(filename, &imageData);
    end = omp_get_wtime();
    total = end - start;
    iOTime += total;
    totalTime += total;
    printf("Image loaded in \t\t%lf seconds\n", total);


    start = omp_get_wtime();
        OtsuThreshold::greyscaleImage(&imageData, &outputData);
    end = omp_get_wtime();
    total = end - start;
    otsuTime += total;
    totalTime += total;
    printf("Converted to greyscale in \t%lf seconds\n", total);


    start = omp_get_wtime();
        std::map<int, int> hist = {};
        OtsuThreshold::generateHistogram(&outputData, &hist);
    end = omp_get_wtime();
    total = end - start;
    otsuTime += total;
    totalTime += total;
    printf("Histogram generated in \t\t%lf seconds\n", total);


    start = omp_get_wtime();
        double targetThreshold = 0;
        targetThreshold = OtsuThreshold::otsuThreshold(hist, outputData.size);
    end = omp_get_wtime();
    total = end - start;
    otsuTime += total;
    totalTime += total;
    std::cout << "\nTarget Threshold: " << targetThreshold << std::endl;
    printf("Threshold calculated in \t%lf seconds\n", total);


    start = omp_get_wtime();
        OtsuThreshold::applyThresholding(&outputData, &outputData, targetThreshold, { 0,0,0 }, { 255,255,255 });
    end = omp_get_wtime();
    total = end - start;
    otsuTime += total;
    totalTime += total;
    printf("Threshold applied in \t\t%lf seconds\n", total);


    start = omp_get_wtime();
        ImageIO::saveImage(outputname, &outputData);
    end = omp_get_wtime();
    total = end - start;
    iOTime += total;
    totalTime += total;
    printf("Image saved in \t\t\t%lf seconds\n", total);

    printf("\nIO took total of \t\t%lf seconds\nOtsu Thresholding took total of %lf seconds\nTotal Time took \t\t%lf seconds\n", iOTime, otsuTime, totalTime);

    //printf("%lf,%lf,%lf,%lf,%lf,%lf\n")
}

void threshold_profile_v2(const char* filename, const char* outputname) {
    double iOTime = 0, otsuTime = 0, totalTime = 0;
    double start, end, total;
    ImageData imageData{};
    ImageData outputData{};

    start = omp_get_wtime();
    ImageIO::loadImage(filename, &imageData);
    end = omp_get_wtime();
    total = end - start;
    iOTime += total;
    totalTime += total;
    printf("\n\nImage loaded in \t\t%lf seconds\n", total);


    start = omp_get_wtime();
    OtsuThreshold_v2::greyscaleImage(&imageData, &outputData);
    end = omp_get_wtime();
    total = end - start;
    otsuTime += total;
    totalTime += total;
    printf("Converted to greyscale in \t%lf seconds\n", total);


    start = omp_get_wtime();
    std::map<int, int> hist = {};
    OtsuThreshold_v2::generateHistogram(&outputData, &hist);
    end = omp_get_wtime();
    total = end - start;
    otsuTime += total;
    totalTime += total;
    printf("Histogram generated in \t\t%lf seconds\n", total);


    start = omp_get_wtime();
    double targetThreshold = 0;
    targetThreshold = OtsuThreshold_v2::otsuThreshold(hist, outputData.size);
    end = omp_get_wtime();
    total = end - start;
    otsuTime += total;
    totalTime += total;
    std::cout << "\nTarget Threshold: " << targetThreshold << std::endl;
    printf("Threshold calculated in \t%lf seconds\n", total);


    start = omp_get_wtime();
    OtsuThreshold_v2::applyThresholding(&outputData, &outputData, targetThreshold, { 0,0,0 }, { 255,255,255 });
    end = omp_get_wtime();
    total = end - start;
    otsuTime += total;
    totalTime += total;
    printf("Threshold applied in \t\t%lf seconds\n", total);


    start = omp_get_wtime();
    ImageIO::saveImage(outputname, &outputData);
    end = omp_get_wtime();
    total = end - start;
    iOTime += total;
    totalTime += total;
    printf("Image saved in \t\t\t%lf seconds\n", total);

    printf("\nIO took total of \t\t%lf seconds\nOtsu Thresholding took total of %lf seconds\nTotal Time took \t\t%lf seconds\n", iOTime, otsuTime, totalTime);

    //printf("%lf,%lf,%lf,%lf,%lf,%lf\n")
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