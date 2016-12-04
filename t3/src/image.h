#ifndef IMAGE_H
#define IMAGE_H

#include "pixel.h"
#include <string>
#include <vector>

class Image
{
public:
    Image() {}
    Image(int width, int height);

    void loadBMP(const std::string& filename);
    void saveBMP(const std::string& filename);

    void fill(const Pixel& p);
    void normalize();

    Pixel& pixel(int x, int y) { return m_pixels[y * m_width + x]; }

    int width() { return m_width; }
    int height() { return m_height; }

    void rgbToLab();
    void labToRgb();

private:
    int m_width;
    int m_height;
    std::vector<Pixel> m_pixels;
};

#endif
