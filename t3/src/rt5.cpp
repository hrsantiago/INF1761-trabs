#include "rt5.h"
#include <cstdio>

bool RT5::load(const std::string& filename)
{
    FILE *fp = fopen(filename.c_str(), "r");
    if(!fp)
        return false;

    fscanf(fp, "RT 5");

    while(!feof(fp)) {
        std::string key = parseString(fp);
        if(key == "SCENE")
            m_scene = parseScene(fp);
        else if(key == "CAMERA")
            m_camera = parseCamera(fp);
        else if(key == "MATERIAL") {
            Material material = parseMaterial(fp);
            m_materials[material.name] = material;
        }
        else if(key == "LIGHT")
            m_lights.push_back(parseLight(fp));
        else if(key == "SPHERE")
            m_spheres.push_back(parseSphere(fp));
        else if(key == "BOX")
            m_boxes.push_back(parseBox(fp));
        else if(key == "TRIANGLE")
            m_triangles.push_back(parseTriangle(fp));
    }

    fclose(fp);
    return true;
}

std::string RT5::parseString(FILE *fp)
{
    char str[80];
    fscanf(fp, "%s", str);
    return std::string(str);
}

RGB RT5::parseRGB(FILE *fp)
{
    RGB rgb;
    fscanf(fp, "%f", &rgb.r);
    fscanf(fp, "%f", &rgb.g);
    fscanf(fp, "%f", &rgb.b);
    return rgb;
}

Position RT5::parsePosition(FILE *fp)
{
    Position pos;
    fscanf(fp, "%f", &pos.x);
    fscanf(fp, "%f", &pos.y);
    fscanf(fp, "%f", &pos.z);
    return pos;
}

Position2D RT5::parsePosition2D(FILE *fp)
{
    Position2D pos;
    fscanf(fp, "%f", &pos.x);
    fscanf(fp, "%f", &pos.y);
    return pos;
}

Scene RT5::parseScene(FILE *fp)
{
    Scene scene;
    scene.backgroundColor = parseRGB(fp);
    scene.ambientLightColor = parseRGB(fp);
    scene.texture = parseString(fp);
    if(scene.texture == "null")
        scene.texture = "";
    return scene;
}

Camera RT5::parseCamera(FILE *fp)
{
    Camera camera;
    camera.eye = parsePosition(fp);
    camera.ref = parsePosition(fp);
    camera.up = parsePosition(fp);
    fscanf(fp, "%f", &camera.fov);
    fscanf(fp, "%f", &camera.near);
    fscanf(fp, "%f", &camera.far);
    fscanf(fp, "%d", &camera.width);
    fscanf(fp, "%d", &camera.height);
    return camera;
}

Material RT5::parseMaterial(FILE *fp)
{
    Material material;
    material.name = parseString(fp);
    material.kd = parseRGB(fp);
    material.ks = parseRGB(fp);
    fscanf(fp, "%d", &material.n);
    fscanf(fp, "%f", &material.k);
    fscanf(fp, "%f", &material.refraction);
    fscanf(fp, "%f", &material.opacity);
    material.texture = parseString(fp);
    return material;
}

Light RT5::parseLight(FILE *fp)
{
    Light light;
    light.pos = parsePosition(fp);
    light.intensity = parseRGB(fp);
    return light;
}

Sphere RT5::parseSphere(FILE *fp)
{
    Sphere sphere;
    sphere.material = parseString(fp);
    fscanf(fp, "%f", &sphere.r);
    sphere.pos = parsePosition(fp);
    return sphere;
}

Box RT5::parseBox(FILE *fp)
{
    Box box;
    box.material = parseString(fp);
    box.bottomLeft = parsePosition(fp);
    box.topRight = parsePosition(fp);
    return box;
}

Triangle RT5::parseTriangle(FILE *fp)
{
    Triangle triangle;
    triangle.material = parseString(fp);
    triangle.v1 = parsePosition(fp);
    triangle.v2 = parsePosition(fp);
    triangle.v3 = parsePosition(fp);
    triangle.tv1 = parsePosition2D(fp);
    triangle.tv2 = parsePosition2D(fp);
    triangle.tv3 = parsePosition2D(fp);
    return triangle;
}
