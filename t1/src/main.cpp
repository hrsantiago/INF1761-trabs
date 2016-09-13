#include <cstdio>
#include "image.h"

int main(int argc, char** argv)
{
    Image image;

    image.loadBMP("images/AB_ufv_0675.bmp");
    image.rgbToLab();
    image.labToRgb();
    image.saveBMP("images/test.bmp");

    return 0;
}
