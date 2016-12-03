#include "pixel.h"
#include <cmath>

void Pixel::rgbToXyz()
{
    for(int i = 0; i < 3; ++i) {
        if(m_v[i] > 0.04045)
            m_v[i] = std::pow((m_v[i] + 0.055) / 1.055, 2.4);
        else
            m_v[i] = m_v[i] / 12.92;

        m_v[i] *= 100.0;
    }

    double x = m_v[0] * 0.4124 + m_v[1] * 0.3576 + m_v[2] * 0.1805;
    double y = m_v[0] * 0.2126 + m_v[1] * 0.7152 + m_v[2] * 0.0722;
    double z = m_v[0] * 0.0193 + m_v[1] * 0.1192 + m_v[2] * 0.9505;

    m_v[0] = x;
    m_v[1] = y;
    m_v[2] = z;
}

void Pixel::xyzToLab()
{
    m_v[0] /= 95.047; //ref_X =  95.047   Observer= 2°, Illuminant= D65
    m_v[1] /= 100.000; //ref_Y = 100.000
    m_v[2] /= 108.883; //ref_Z = 108.883

    for(int i = 0; i < 3; ++i) {
        if(m_v[i] > 0.008856)
            m_v[i] = std::pow(m_v[i], (1. / 3.));
        else
            m_v[i] = (7.787 * m_v[i]) + (16.0 / 116.0);
    }

    double l = (116 * m_v[1]) - 16;
    double a = 500 * (m_v[0] - m_v[1]);
    double b = 200 * (m_v[1] - m_v[2]);

    m_v[0] = l;
    m_v[1] = a;
    m_v[2] = b;
}

void Pixel::labToXyz()
{
    float xyz[3];
    xyz[1] = (m_v[0] + 16) / 116;
    xyz[0] = m_v[1] / 500 + xyz[1];
    xyz[2] = xyz[1] - m_v[2] / 200;

    for(int i = 0; i < 3; ++i) {
        if (std::pow(xyz[i], 3) > 0.008856)
            xyz[i] = std::pow(xyz[i], 3);
        else
            xyz[i] = (xyz[i] - 16.0 / 116.0) / 7.787;
    }

    m_v[0] = 95.047 * xyz[0]; //ref_X =  95.047     Observer= 2°, Illuminant= D65
    m_v[1] = 100.000 * xyz[1]; //ref_Y = 100.000
    m_v[2] = 108.883 * xyz[2]; //ref_Z = 108.883
}

void Pixel::xyzToRgb()
{
    m_v[0] /= 100.;
    m_v[1] /= 100.;
    m_v[2] /= 100.;

    float rgb[3];
    rgb[0] = m_v[0] * 3.2406 + m_v[1] * -1.5372 + m_v[2] * -0.4986;
    rgb[1] = m_v[0] * -0.9689 + m_v[1] * 1.8758 + m_v[2] * 0.0415;
    rgb[2] = m_v[0] * 0.0557 + m_v[1] * -0.2040 + m_v[2] * 1.0570;

    for(int i = 0; i < 3; ++i) {
        if(rgb[i] > 0.0031308)
            rgb[i] = 1.055 * std::pow(rgb[i], (1 / 2.4)) - 0.055;
        else
            rgb[i] = 12.92 * rgb[i];
    }

    m_v[0] = rgb[0];
    m_v[1] = rgb[1];
    m_v[2] = rgb[2];
}
