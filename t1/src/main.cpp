#include <cstdio>
#include <cmath>
#include <cassert>
#include <functional>
#include "image.h"
#include "superpixel.h"

void superPixel(Image& image, int numberSuperPixels)
{
    image.rgbToLab();

    int width = image.width();
    int height = image.height();
    int size = std::sqrt(width * height / numberSuperPixels);
    printf("Size: %d\n", size);

    std::vector<SuperPixel> superPixels(std::ceil((float)width / size) * std::ceil((float)height / size));
    printf("Super pixels: %d\n", superPixels.size());
    std::vector<SuperPixel*> references(width * height);
    std::vector<float> distances(width * height);

    auto updateSuperPixels = [&]() {
        for(SuperPixel& sp : superPixels) {
            sp.x = 0;
            sp.y = 0;
            sp.l = 0;
            sp.a = 0;
            sp.b = 0;
        }

        for(int y = 0; y < height; ++y) {
            for(int x = 0; x < width; ++x) {
                int pos = y * width + x;

                SuperPixel& sp = *references[pos];

                Pixel& pixel = image.pixel(x, y);
                sp.x += x;
                sp.y += y;
                sp.l += pixel.v0();
                sp.a += pixel.v1();
                sp.b += pixel.v2();
            }
        }

        for(SuperPixel& sp : superPixels) {
            sp.x /= sp.pixels;
            sp.y /= sp.pixels;
            sp.l /= sp.pixels;
            sp.a /= sp.pixels;
            sp.b /= sp.pixels;
        }
    };

    auto getDistance = [](int x, int y, Pixel& pixel, SuperPixel& sp, int size) {
        float ds = std::sqrt(std::pow(sp.x-x,2) + std::pow(sp.y-y,2));
        float dc = std::sqrt(std::pow(sp.l-pixel.v0(),2) + std::pow(sp.a-pixel.v1(),2) + std::pow(sp.b-pixel.v2(),2));
        float dt = std::sqrt(std::pow(dc/40.,2) + std::pow(ds/size,2));
        return dt;
    };

    auto updateDistances = [&]() {
        for(int y = 0; y < height; ++y) {
            for(int x = 0; x < width; ++x) {
                int spIndex = (y / size) * std::ceil((float)width / size) + (x / size);
                assert(spIndex < superPixels.size());
                SuperPixel& sp = superPixels[spIndex];

                Pixel& pixel = image.pixel(x, y);

                int pos = y * width + x;
                distances[pos] = getDistance(x, y, pixel, sp, size);
            }
        }
    };

    for(int y = 0; y < height; ++y) {
        for(int x = 0; x < width; ++x) {
            int spIndex = (y / size) * std::ceil((float)width / size) + (x / size);
            assert(spIndex < superPixels.size());
            SuperPixel& sp = superPixels[spIndex];
            ++sp.pixels;

            int pos = y * width + x;
            references[pos] = &sp;
        }
    }


    for(int it = 0; it < 20; ++it) {
        updateSuperPixels();
        updateDistances();

        bool changed = false;
        for(SuperPixel& sp : superPixels) {
            int x0 = std::max<int>(sp.x - size, 0);
            int x1 = std::min<int>(sp.x + size, width);
            int y0 = std::max<int>(sp.y - size, 0);
            int y1 = std::min<int>(sp.y + size, height);
            for(int y = y0; y < y1; ++y) {
                for(int x = x0; x < x1; ++x) {
                    int pos = y * width + x;
                    Pixel& pixel = image.pixel(x, y);
                    float d = getDistance(x, y, pixel, sp, size);
                    if(d < distances[pos]) {
                        SuperPixel& old = *references[pos];
                        --old.pixels;
                        references[pos] = &sp;
                        ++sp.pixels;
                        distances[pos] = d;
                        changed = true;
                    }
                }
            }
        }
        if(!changed) {
            printf("Algorithm converged in %d iterations.\n", it);
            break;
        }
    }

    for(int y = 0; y < height; ++y) {
        for(int x = 0; x < width; ++x) {
            Pixel& pixel = image.pixel(x, y);

            int pos = y * width + x;
            SuperPixel& sp = *references[pos];
            pixel = Pixel(sp.l, sp.a, sp.b);
        }
    }

    image.labToRgb();
}

int main(int argc, char** argv)
{
    Image image1;
    image1.loadBMP("images/AB_ufv_0675.bmp");
    superPixel(image1, 1024);
    image1.saveBMP("images/AB_ufv_0675_2.bmp");

    Image image2;
    image2.loadBMP("images/estrela.bmp");
    superPixel(image2, 1024);
    image2.saveBMP("images/estrela_2.bmp");
    return 0;
}
