#include <cstdio>
#include "image.h"
#include "superpixelfilter.h"

int main(int argc, char** argv)
{
    SuperPixelFilter spFilter;

    Image image1;
    image1.loadBMP("images/AB_ufv_0675.bmp");
    spFilter.process(image1, 1024, 10);
    image1.saveBMP("images/AB_ufv_0675_2.bmp");

    Image image2;
    image2.loadBMP("images/estrela.bmp");
    spFilter.process(image2, 1024, 10);
    image2.saveBMP("images/estrela_2.bmp");
    return 0;
}
