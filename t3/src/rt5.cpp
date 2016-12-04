#include "rt5.h"
#include <cstdio>
#include <limits>

float PI = 3.14159265;
float deg_to_rad = PI / 180;

Image RT5::render()
{
    float w = m_camera.width;
    float h = m_camera.height;
    Image image(w, h);

    float a = 2 * m_camera.near * std::tan(m_camera.fov * deg_to_rad / 2.);
    float b = (a * w) / h;

    Vec3f ze = (m_camera.eye - m_camera.ref).normalized();
    Vec3f xe = Vec3f::crossProduct(m_camera.up, ze).normalized();
    Vec3f ye = Vec3f::crossProduct(ze, xe);

    for(int y = 0; y < h; ++y) {
        for(int x = 0; x < w; ++x) {
            Vec3f o = m_camera.eye;
            Vec3f d = xe * b * (x / w - 0.5) + ye * a *(y / h - 0.5) - ze * m_camera.near;
            image.pixel(x, y) = trace(o, d, 1);
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

ObjectIntersection RT5::intersection(const Vec3f& o, const Vec3f& d)
{
    float t = std::numeric_limits<float>::infinity();
    ObjectIntersection i;

    for(const Sphere& sphere : m_spheres) {
        float t1, t2;
        if(sphere.intersect(o, d, t1, t2)) {
            float t0 = std::min(t1, t2); // must ensure they are above 0 or above near? donno
            if(t0 > 0 && t0 < t) {
                t = t0;
                i.p = o + d * t0;
                i.n = sphere.normal(i.p);
                i.material = m_materials[sphere.material];
                i.valid = true;
            }
        }
    }

    for(const Box& box : m_boxes) {
        float t1, t2;
        if(box.intersect(o, d, t1, t2)) {
            float t0 = std::min(t1, t2); // must ensure they are above 0 or above near? donno
            if(t0 > 0 && t0 < t) {
                t = t0;
                i.p = o + d * t0;
                i.n = box.normal(i.p);
                i.material = m_materials[box.material];
                i.valid = true;
            }
        }
    }

    for(const Triangle& triangle : m_triangles) {
        float t0;
        if(triangle.intersect(o, d, t0)) {
            if(t0 > 0 && t0 < t) {
                t = t0;
                i.p = o + d * t0;
                i.n = triangle.normal();
                i.material = m_materials[triangle.material];
                i.valid = true;
            }
        }
    }
    return i;
}

Pixel RT5::trace(const Vec3f& o, const Vec3f& d, int depth)
{
    ObjectIntersection i = intersection(o, d);
    if(i.valid)
        return shade(o, d, i.n, i.p, i.material, depth);
    else
        return Pixel(m_scene.backgroundColor.r, m_scene.backgroundColor.g, m_scene.backgroundColor.b);
}

Pixel RT5::shade(const Vec3f& o, const Vec3f& d, const Vec3f& n, const Vec3f& p, const Material& material, int depth)
{
    Vec3f v = (o - p).normalized();
    Vec3f Ia = Vec3f(m_scene.ambientLightColor.r, m_scene.ambientLightColor.g, m_scene.ambientLightColor.b);
    Vec3f Ip = Ia;

    for(const Light& light : m_lights) {
        ObjectIntersection i = intersection(p, light.pos - p);
        if(!i.valid) {
            Vec3f l = (light.pos - p).normalized();
            Vec3f r = n * (2 * l.dotProduct(n)) - l;
            Vec3f Is = Vec3f(material.ks.r, material.ks.g, material.ks.b) * std::pow(r.dotProduct(v), material.n);
            Vec3f Id = Vec3f(material.kd.r, material.kd.g, material.kd.b) * std::max<float>(l.dotProduct(n), 0);
            Ip += Is + Id;
        }
    }

    Pixel src = Pixel(Ip.getX(), Ip.getY(), Ip.getZ());
    if(depth > 10)
        return src;

    if(material.k > 0) {
        Vec3f r = n * (2 * v.dotProduct(n)) - v;
        Pixel rColor = trace(p, r, depth + 1);
        src += rColor * material.k;
    }

    if(material.opacity < 1) {
        Vec3f vt = (n * v.dotProduct(n)) - v;
        float sinThetaI = std::sqrt(vt.dotProduct(vt));
        float sinTheta = sinThetaI / material.refraction;
        float cosTheta = std::sqrt(1 - sinTheta * sinTheta);
        Vec3f r = vt.normalized() * sinTheta - n * cosTheta;
        Pixel tColor = trace(p, r, depth + 1);
        //src += tColor * (1 - material.opacity);
        //if(src.v0() >= 1 || src.v1() >= 1 || src.v2() >= 1)
            //printf("vish");
    }

    return src;
    //Pixel dest = image.pixel(x, y);
    //return (src * material.opacity) + (dest * (1 - material.opacity));
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
