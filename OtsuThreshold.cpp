#include "OtsuThreshold.h"

void OtsuThreshold::greyscaleImage(ImageData* inputData, ImageData* out)
{
    out->size = inputData->size;
    out->width = inputData->width;
    out->height = inputData->height;
    out->channels = inputData->channels;

    unsigned char luma = 0;
    for (int i = 0; i < inputData->size; i++)
    {
        luma = 0.299 * inputData->R[i] + 0.587 * inputData->G[i] + 0.114 * inputData->B[i];

        //assign luma to output
        out->R.push_back(luma);
        out->G.push_back(luma);
        out->B.push_back(luma);

        if (out->A.size() > 0)
            out->A.push_back(inputData->A[i]);
    }
}

//parallize
void OtsuThreshold::generateHistogram(ImageData* inputData, std::map<int, int>* histogram)
{
    for (int i = 0; i < 256; i++) {
        histogram->insert({ i,0 });
    }

    //if grayscale, rgb values of a pixel are identical
    //chose r as sample
    for (int i = 0; i < inputData->size; i++) {
        double value = inputData->R[i];
        if (histogram->count(value) > 0)    //contains key
        {
            (*histogram)[value]++;
        }
        else {
            histogram->insert({ value, 1 });
        }
    }

    return;
}

void OtsuThreshold::otsuThresholdProfile(std::map<int, int> hist, int imageSize, double* thresholdValue)
{
    *thresholdValue = otsuThreshold(hist, imageSize);
}

double OtsuThreshold::otsuThreshold(std::map<int, int> hist, int imageSize) {
    //pre-calculations
    double totalMean = hist[0] * 0;

    std::map<int, double> valueProbability;
    valueProbability.insert({ 0, (double)hist[0] / (double)imageSize });
    for (int i = 1; i < 256; i++) {
        valueProbability.insert({ i, (double)hist[i] / (double)imageSize });
        totalMean += valueProbability[i] * i;
    }

    //--------------------------------------------------
    int threshold = 0;
    double w0 = 0, w1 = 0;
    double mu0 = 0, mu1 = 0, mu = 0;

    double interclassVariance = 0;
    double highestVariance = 0;
    double highestThreshold = 0;

    //#pragma omp parallel
    for (int i = 0; i < 256; i++)
    {
        threshold = i;
        if (threshold == 0)
            continue;

        w0 += valueProbability[threshold];
        w1 = 1 - w0;

        mu += threshold * valueProbability[threshold];
        mu0 = mu / w0;
        mu1 = (totalMean - mu) / w1;

        interclassVariance = w0 * w1 * pow(mu0 - mu1, 2);

        if (interclassVariance > highestVariance)
        {
            highestVariance = interclassVariance;
            highestThreshold = threshold;
        }
    }

    return highestThreshold;
}

//assumes image is greyscale
void OtsuThreshold::applyThresholding(ImageData* inputData, ImageData* outputData, double thresholdValue, std::vector<int> low, std::vector<int> high)
{
    for (int i = 0; i < inputData->size; i++)
    {
        //check pixel brightness, assume grayscale, use R channel as sample

        //if higher than threshold
        if (inputData->R[i] > thresholdValue)
        {
            outputData->R[i] = (high[0]);
            outputData->G[i] = (high[1]);
            outputData->B[i] = (high[2]);
        }
        else if (inputData->R[i] <= thresholdValue)
        {
            outputData->R[i] = (low[0]);
            outputData->G[i] = (low[1]);
            outputData->B[i] = (low[2]);
        }
    }
}

void OtsuThreshold_v2::greyscaleImage(ImageData* inputData, ImageData* out)
{
    out->size = inputData->size;
    out->width = inputData->width;
    out->height = inputData->height;
    out->channels = inputData->channels;

    unsigned char luma = 0;
    for (int i = 0; i < inputData->size; i++)
    {
        luma = 0.299 * inputData->R[i] + 0.587 * inputData->G[i] + 0.114 * inputData->B[i];

        //assign luma to output
        out->R.push_back(luma);
        out->G.push_back(luma);
        out->B.push_back(luma);

        if (out->A.size() > 0)
            out->A.push_back(inputData->A[i]);
    }
}

//parallize
void OtsuThreshold_v2::generateHistogram(ImageData* inputData, std::map<int, int>* histogram)
{
    for (int i = 0; i < 256; i++) {
        histogram->insert({ i,0 });
    }

    //if grayscale, rgb values of a pixel are identical
    //chose r as sample

#pragma omp parallel 
    {
        // Each thread gets its own local histogram
        std::map<double, int> local_hist;

        for (int i = 0; i < inputData->size; i++) {
            double value = inputData->R[i];
            local_hist[value]++;
        }

        // Merge local histograms into global one
        #pragma omp critical
        {
            for (const auto& key : local_hist) {
                (*histogram)[key.first] += key.second;
            }
        }
    }

    return;
}

double OtsuThreshold_v2::otsuThreshold(std::map<int, int> hist, int imageSize) {
    //pre-calculations
    double totalMean = hist[0] * 0;

    std::map<int, double> valueProbability;
    valueProbability.insert({ 0, (double)hist[0] / (double)imageSize });
    for (int i = 1; i < 256; i++) {
        valueProbability.insert({ i, (double)hist[i] / (double)imageSize });
        totalMean += valueProbability[i] * i;
    }

    //--------------------------------------------------
    int threshold = 0;
    double w0 = 0, w1 = 0;
    double mu0 = 0, mu1 = 0, mu = 0;

    double interclassVariance = 0;
    double highestVariance = 0;
    double highestThreshold = 0;

    //#pragma omp parallel
    for (int i = 0; i < 256; i++)
    {
        threshold = i;
        if (threshold == 0)
            continue;

        w0 += valueProbability[threshold];
        w1 = 1 - w0;

        mu += threshold * valueProbability[threshold];
        mu0 = mu / w0;
        mu1 = (totalMean - mu) / w1;

        interclassVariance = w0 * w1 * pow(mu0 - mu1, 2);

        if (interclassVariance > highestVariance)
        {
            highestVariance = interclassVariance;
            highestThreshold = threshold;
        }
    }

    return highestThreshold;
}

//assumes image is greyscale
void OtsuThreshold_v2::applyThresholding(ImageData* inputData, ImageData* outputData, double thresholdValue, std::vector<int> low, std::vector<int> high)
{
    for (int i = 0; i < inputData->size; i++)
    {
        //check pixel brightness, assume grayscale, use R channel as sample

        //if higher than threshold
        if (inputData->R[i] > thresholdValue)
        {
            outputData->R[i] = (high[0]);
            outputData->G[i] = (high[1]);
            outputData->B[i] = (high[2]);
        }
        else if (inputData->R[i] <= thresholdValue)
        {
            outputData->R[i] = (low[0]);
            outputData->G[i] = (low[1]);
            outputData->B[i] = (low[2]);
        }
    }
}
