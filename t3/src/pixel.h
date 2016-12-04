#ifndef PIXEL_H
#define PIXEL_H

class Pixel
{
public:
    Pixel() {}
    Pixel(float v0, float v1, float v2) { m_v[0] = v0; m_v[1] = v1; m_v[2] = v2; }

    float v0() const { return m_v[0]; }
    float v1() const { return m_v[1]; }
    float v2() const { return m_v[2]; }

    void setV(float v0, float v1, float v2) { m_v[0] = v0; m_v[1] = v1; m_v[2] = v2; }

    void rgbToXyz();
    void xyzToLab();
    void labToXyz();
    void xyzToRgb();

    void operator+=(const Pixel& p) { m_v[0] += p.m_v[0]; m_v[1] += p.m_v[1]; m_v[2] += p.m_v[2]; }

    Pixel operator+(const Pixel& p) const { return Pixel(m_v[0] + p.m_v[0], m_v[1] + p.m_v[1], m_v[2] + p.m_v[2]); }
    Pixel operator*(float value) const { return Pixel(m_v[0] * value, m_v[1] * value, m_v[2] * value); }


private:
    float m_v[3];

};

#endif
