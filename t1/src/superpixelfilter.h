#ifndef SUPERPIXELFILTER_H
#define SUPERPIXELFILTER_H

#include "image.h"

struct SuperPixel
{
    float x = 0;
    float y = 0;
    float l = 0;
    float a = 0;
    float b = 0;
    int pixels = 0;
    float maxPositionDistance = 0;
    float maxColorDistance = 0;
};

class SuperPixelFilter
{
public:
    void process(Image& image, int numberSuperPixels, int maxIterations);

private:
    float getPositionDistance(int x, int y, SuperPixel& sp);
    float getColorDistance(Pixel& pixel, SuperPixel& sp);
    float getDistance(float ds, float dc, SuperPixel& sp);
    void updateSuperPixels();
    void updateDistances();
    void removeOrphaned();
    void repaintPixels();

    Image *m_image;

    int m_w;
    int m_h;
    int m_size;

    std::vector<SuperPixel> m_superPixels;
    std::vector<SuperPixel*> m_references;
    std::vector<float> m_distances;
};

#endif
