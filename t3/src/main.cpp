#include <cstdio>
#include <string>
#include <sstream>
#include "image.h"
#include "rt5.h"

bool parse(const std::string& input, const std::string& output)
{
    RT5 rt5;
    if(!rt5.load(input)) {
        printf("Failed to load %s\n", input.c_str());
        return false;
    }

    printf("Rendering scene: %s\n", input.c_str());
    Image image = rt5.render();
    image.saveBMP(output);
    return true;
}

int main(int argc, char** argv)
{
    std::vector<std::string> files = {
        "scenes/05_color_balls.rt5",
        "scenes/05_color_balls_1.rt5",
        "scenes/20_glass_balls.rt5",
        "scenes/horse_mesh.rt5",
        "scenes/metal_balls.rt5",
        "scenes/mirror_balls.rt5",
        "scenes/person.rt5",
        "scenes/plastic_ball.rt5",
        "scenes/pool.rt5",
        "scenes/room.rt5",
        "scenes/sphere_boxes.rt5",
        "scenes/sphere_boxes_ruan.rt5",
        "scenes/transparent_sphere_2_planes.rt5",
        "scenes/wall.rt5",
    };

    if(argc <= 1) {
        int index = 0;
        for(const std::string& file : files) {
            std::stringstream ss;
            ss << "output/" << (index++) << ".bmp";
            parse(file, ss.str());
        }
    }
    else {
        std::string filename = argv[1];
        parse(filename, "output/out.bmp");
    }

    return 0;
}
