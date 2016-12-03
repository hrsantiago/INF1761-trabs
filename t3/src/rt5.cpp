#include "rt5.h"
#include <cstdio>

float PI = 3.14159265;
float deg_to_rad = PI / 180;

Image RT5::render()
{
    float w = m_camera.width;
    float h = m_camera.height;
    Image image(w, h);

    renderBackground(image); // Maybe if not intersects any object, draw this pixel? Seems better

    for(int y = 0; y < h; ++y) {
        for(int x = 0; x < w; ++x) {
            float a = 2 * m_camera.near * std::tan(m_camera.fov * deg_to_rad / 2.);
            float b = (a * w) / h;

            Vec3f ze = (m_camera.eye - m_camera.ref).normalized();
            Vec3f xe = Vec3f::crossProduct(m_camera.up, ze).normalized();
            Vec3f ye = Vec3f::crossProduct(ze, xe);

            Vec3f o = m_camera.eye;
            Vec3f d = xe * b * (x / w - 0.5) + ye * a *(y / h - 0.5) - ze * m_camera.near;

            float t = m_camera.far;
            Vec3f p;
            Vec3f surfaceNormal;
            Material material;

            for(const Sphere& sphere : m_spheres) {
                float t1, t2;
                if(sphere.intersect(o, d, t1, t2)) {
                    float t0 = std::min(t1, t2); // must ensure they are above 0 or above near? donno
                    if(t0 < t) {
                        t = t0;
                        p = o + d * t0;
                        surfaceNormal = (p - sphere.pos).normalized();
                        material = m_materials[sphere.material];
                    }
                }
            }

            if(t < m_camera.far) {
                Vec3f v = (m_camera.eye - p).normalized();
                Vec3f Ia = Vec3f(m_scene.ambientLightColor.r, m_scene.ambientLightColor.g, m_scene.ambientLightColor.b);
                Vec3f Ip = Ia;

                for(const Light& light : m_lights) {
                    Vec3f l = (light.pos - p).normalized();
                    Vec3f r = surfaceNormal * (2 * l.dotProduct(surfaceNormal)) - l;
                    Vec3f Is = Vec3f(material.ks.r, material.ks.g, material.ks.b) * std::pow(r.dotProduct(v), material.n);
                    Vec3f Id = Vec3f(material.kd.r, material.kd.g, material.kd.b) * std::max<float>(l.dotProduct(surfaceNormal), 0);
                    Ip += Is + Id;
                }

                image.pixel(x, y) = Pixel(Ip.getX(), Ip.getY(), Ip.getZ());
            }
        }
    }


    return image;
}

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

void RT5::renderBackground(Image& image)
{
    Pixel backgroundPixel(m_scene.backgroundColor.r, m_scene.backgroundColor.g, m_scene.backgroundColor.b);
    image.fill(backgroundPixel);

    // TODO render bg texture
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

Vec3f RT5::parsePosition(FILE *fp)
{
    float x, y, z;
    fscanf(fp, "%f", &x);
    fscanf(fp, "%f", &y);
    fscanf(fp, "%f", &z);
    return Vec3f(x, y, z);
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
    if(material.texture == "null")
        material.texture = "";
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
