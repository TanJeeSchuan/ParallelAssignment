#pragma once
#include <vector>

struct ImageData {
    int width = 0, height = 0;

    std::vector<unsigned char> R;
    std::vector<unsigned char> G;
    std::vector<unsigned char> B;
    std::vector<unsigned char> A;

    int channels = 0;
    int size = 0;
};

