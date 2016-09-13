#include "superpixelfilter.h"
#include <cmath>
#include <queue>
#include <limits>

void SuperPixelFilter::process(Image& image, int numberSuperPixels, int maxIterations)
{
    m_image = &image;

    image.rgbToLab();

    m_w = image.width();
    m_h = image.height();
    m_size = std::sqrt(m_w * m_h / numberSuperPixels);
    printf("Size: %d\n", m_size);

    m_superPixels.clear();
    m_references.clear();
    m_distances.clear();

    m_superPixels.resize(std::ceil((float)m_w / m_size) * std::ceil((float)m_h / m_size));
    printf("Super pixels: %d\n", m_superPixels.size());
    m_references.resize(m_w * m_h);
    m_distances.resize(m_w * m_h);

    for(int y = 0; y < m_h; ++y) {
        for(int x = 0; x < m_w; ++x) {
            int spIndex = (y / m_size) * std::ceil((float)m_w / m_size) + (x / m_size);
            SuperPixel& sp = m_superPixels[spIndex];
            ++sp.pixels;

            int pos = y * m_w + x;
            m_references[pos] = &sp;
        }
    }

    for(int i = 0; i < maxIterations; ++i) {
        updateSuperPixels();
        updateDistances();

        for(SuperPixel& sp : m_superPixels) {
            int x0 = std::max<int>(sp.x - m_size, 0);
            int x1 = std::min<int>(sp.x + m_size, m_w);
            int y0 = std::max<int>(sp.y - m_size, 0);
            int y1 = std::min<int>(sp.y + m_size, m_h);

            for(int y = y0; y < y1; ++y) {
                for(int x = x0; x < x1; ++x) {
                    int pos = y * m_w + x;
                    Pixel& pixel = image.pixel(x, y);
                    float ds = getPositionDistance(x, y, sp);
                    float dc = getColorDistance(pixel, sp);
                    float d = getDistance(ds, dc, sp);
                    if(d < m_distances[pos]) {
                        SuperPixel& old = *m_references[pos];
                        --old.pixels;
                        m_references[pos] = &sp;
                        ++sp.pixels;
                        m_distances[pos] = d;
                    }
                }
            }
        }
    }

    removeOrphaned();
    repaintPixels();
    image.labToRgb();
}

float SuperPixelFilter::getPositionDistance(int x, int y, SuperPixel& sp)
{
    return std::sqrt(std::pow(sp.x-x,2) + std::pow(sp.y-y,2));
}

float SuperPixelFilter::getColorDistance(Pixel& pixel, SuperPixel& sp)
{
    return std::sqrt(std::pow(sp.l-pixel.v0(),2) + std::pow(sp.a-pixel.v1(),2) + std::pow(sp.b-pixel.v2(),2));
}

float SuperPixelFilter::getDistance(float ds, float dc, SuperPixel& sp)
{
    return std::sqrt(std::pow(dc/sp.maxColorDistance,2) + std::pow(ds/sp.maxPositionDistance,2));
    //return std::sqrt(std::pow(dc/20.,2) + std::pow(ds/m_size,2));
    //return std::sqrt(std::pow(dc/20.,2) + std::pow(ds/sp.maxPositionDistance,2));
}

void SuperPixelFilter::updateSuperPixels()
{
    for(SuperPixel& sp : m_superPixels) {
        sp.x = 0;
        sp.y = 0;
        sp.l = 0;
        sp.a = 0;
        sp.b = 0;
        sp.maxPositionDistance = 0;
        sp.maxColorDistance = 0;
    }

    for(int y = 0; y < m_h; ++y) {
        for(int x = 0; x < m_w; ++x) {
            int pos = y * m_w + x;
            Pixel& pixel = m_image->pixel(x, y);
            SuperPixel& sp = *m_references[pos];
            sp.x += x;
            sp.y += y;
            sp.l += pixel.v0();
            sp.a += pixel.v1();
            sp.b += pixel.v2();
        }
    }

    for(SuperPixel& sp : m_superPixels) {
        sp.x /= sp.pixels;
        sp.y /= sp.pixels;
        sp.l /= sp.pixels;
        sp.a /= sp.pixels;
        sp.b /= sp.pixels;
    }
}

void SuperPixelFilter::updateDistances()
{
    for(int y = 0; y < m_h; ++y) {
        for(int x = 0; x < m_w; ++x) {
            int spIndex = (y / m_size) * std::ceil((float)m_w / m_size) + (x / m_size);
            SuperPixel& sp = m_superPixels[spIndex];
            Pixel& pixel = m_image->pixel(x, y);

            float ds = getPositionDistance(x, y, sp);
            float dc = getColorDistance(pixel, sp);
            float d = getDistance(ds, dc, sp);
            sp.maxPositionDistance = std::max(sp.maxPositionDistance, ds);
            sp.maxColorDistance = std::max(sp.maxColorDistance, dc);

            int pos = y * m_w + x;
            m_distances[pos] = d;
        }
    }
}

void SuperPixelFilter::removeOrphaned()
{
    std::vector<bool> connectedPixels(m_references.size());

    for(SuperPixel& sp : m_superPixels) {
        if(sp.pixels == 0)
            continue;
        std::queue<std::pair<int,int>> searchPixels;
        searchPixels.push(std::pair<int,int>(std::round(sp.x), std::round(sp.y)));

        while(!searchPixels.empty()) {
            std::pair<int,int> pixel = searchPixels.front();
            searchPixels.pop();
            int x = pixel.first;
            int y = pixel.second;
            int pos = y * m_w + x;
            connectedPixels[pos] = true;
            SuperPixel *parent = m_references[pos];

            for(int yl = y-1; yl <= y+1; ++yl) {
                for(int xl = x-1; xl <= x+1; ++xl) {
                    if(std::abs(xl-x) == std::abs(yl-y))
                        continue;

                    if(xl >= 0 && xl < m_w && yl >= 0 && yl < m_h && !(xl == x && yl == y)) {
                        int posl = yl * m_w + xl;
                        SuperPixel *siblingParent = m_references[posl];
                        if(parent == siblingParent && !connectedPixels[posl]) {
                            connectedPixels[posl] = true;
                            searchPixels.push(std::pair<int,int>(xl,yl));
                        }
                    }
                }
            }
        }
    }

    for(int y = 0; y < m_h; ++y) {
        for(int x = 0; x < m_w; ++x) {
            int pos = y * m_w + x;
            if(!connectedPixels[pos]) {
                SuperPixel *ref = m_references[pos];
                float dMin = std::numeric_limits<float>::infinity();
                for(SuperPixel& sp : m_superPixels) {
                    if(sp.pixels > 0 && &sp != ref) {
                        float d = getPositionDistance(x, y, sp);
                        if(d < dMin) {
                            --ref->pixels;
                            ref = &sp;
                            ++sp.pixels;
                            m_references[pos] = &sp;
                            dMin = d;
                        }
                    }
                }
            }
        }
    }
}

void SuperPixelFilter::repaintPixels()
{
    for(int y = 0; y < m_h; ++y) {
        for(int x = 0; x < m_w; ++x) {
            int pos = y * m_w + x;
            SuperPixel& sp = *m_references[pos];

            Pixel& pixel = m_image->pixel(x, y);
            pixel.setV(sp.l, sp.a, sp.b);
        }
    }
}
