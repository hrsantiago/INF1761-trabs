#include <cstdio>
#include <string>
#include "image.h"
#include "rt5.h"

int main(int argc, char** argv)
{
    if(argc <= 1) {
        printf("Use scene filename as argument.\n");
        return -1;
    }

    std::string filename = argv[1];
    RT5 rt5;
    if(!rt5.load(filename)) {
        printf("Failed to load %s\n", filename.c_str());
        return -1;
    }

    printf("Rendering scene: %s\n", filename.c_str());
    Image image = rt5.render();
    image.saveBMP("out.bmp");

    return 0;
}
