#ifndef IMAGE_H
#define IMAGE_H

#include "pixel.h"
#include <string>
#include <vector>

class Image
{
public:
    Image() {}

    void loadBMP(const std::string& filename);
    void saveBMP(const std::string& filename);

    void rgbToLab();
    void labToRgb();

private:
    int m_width;
    int m_height;
    std::vector<Pixel> m_pixels;
};

#endif
