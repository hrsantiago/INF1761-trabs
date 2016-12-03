#include "image.h"
#include "imageloader.h"

void Image::loadBMP(const std::string& filename)
{
    float *rgb;
    ImageLoader l;
    l.readBMP(rgb, m_width, m_height, filename.c_str());

    m_pixels.resize(m_width * m_height);

    for(int i = 0; i < m_height; i++) {
        for(int j = 0; j < m_width; j++) {
            int pos = i * m_width + j;
            m_pixels[pos] = Pixel(rgb[3*pos+0], rgb[3*pos+1], rgb[3*pos+2]);
        }
    }
    delete rgb;
}

void Image::saveBMP(const std::string& filename)
{
    float *rgb = new float[3 * m_width * m_height];
    for(int i = 0; i < m_height; i++) {
        for(int j = 0; j < m_width; j++) {
            int pos = i * m_width + j;
            Pixel& pixel = m_pixels[pos];
            rgb[3*pos+0] = pixel.v0();
            rgb[3*pos+1] = pixel.v1();
            rgb[3*pos+2] = pixel.v2();
        }
    }

    ImageLoader l;
    l.writeBMP(rgb, m_width, m_height, filename.c_str());
    delete rgb;
}

void Image::rgbToLab()
{
    for(Pixel& pixel : m_pixels) {
        pixel.rgbToXyz();
        pixel.xyzToLab();
    }
}

void Image::labToRgb()
{
    for(Pixel& pixel : m_pixels) {
        pixel.labToXyz();
        pixel.xyzToRgb();
    }
}
