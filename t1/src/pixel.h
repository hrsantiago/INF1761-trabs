#ifndef PIXEL_H
#define PIXEL_H

class Pixel
{
public:
    Pixel() {}
    Pixel(float v0, float v1, float v2) { m_v[0] = v0; m_v[1] = v1; m_v[2] = v2; }

    float v0() { return m_v[0]; }
    float v1() { return m_v[1]; }
    float v2() { return m_v[2]; }

    void setV(float v0, float v1, float v2) { m_v[0] = v0; m_v[1] = v1; m_v[2] = v2; }

    void rgbToXyz();
    void xyzToLab();
    void labToXyz();
    void xyzToRgb();

private:
    float m_v[3];

};

#endif
