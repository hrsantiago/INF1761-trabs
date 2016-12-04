#ifndef RT5_H
#define RT5_H

#include <string>
#include <vector>
#include <map>
#include "image.h"
#include "vec3.h"

struct RGB
{
    float r;
    float g;
    float b;
};

struct Position2D
{
    float x;
    float y;
};

struct Scene
{
    RGB backgroundColor;
    RGB ambientLightColor;
    std::string texture;
};

struct Camera
{
    Vec3f eye;
    Vec3f ref;
    Vec3f up;
    float fov;
    float near;
    float far;
    int width;
    int height;
};

struct Material
{
    std::string name;
    RGB kd;
    RGB ks;
    int n; // specular
    float k; // reflection coef
    float refraction;
    float opacity;
    std::string texture;
};

struct Light
{
    Vec3f pos;
    RGB intensity;
};

struct Sphere
{
    bool intersect(const Vec3f& o, const Vec3f& d, float& t1, float& t2) const {
        Vec3f oc = o - pos;
        float a = d.dotProduct(d);
        float b = (d * 2).dotProduct(oc);
        float c = oc.dotProduct(oc) - r * r;

        float delta = b * b - 4 * a * c;
        if(delta < 0)
            return false;

        t1 = (-b - std::sqrt(delta)) / (2. * a);
        t2 = (-b + std::sqrt(delta)) / (2. * a);
        return true;
    }

    Vec3f normal(const Vec3f& p) const {
        return (p - pos).normalized();
    }

    std::string material;
    float r;
    Vec3f pos;
};

struct Box
{
    bool intersect(const Vec3f& o, const Vec3f& d, float& t1, float& t2) const {
        float tmin = (std::min<float>(bottomLeft.getX(), topRight.getX()) - o.getX()) / d.getX();
        float tmax = (std::max<float>(bottomLeft.getX(), topRight.getX()) - o.getX()) / d.getX();

        if(tmin > tmax)
            std::swap(tmin, tmax);

        float tymin = (std::min<float>(bottomLeft.getY(), topRight.getY()) - o.getY()) / d.getY();
        float tymax = (std::max<float>(bottomLeft.getY(), topRight.getY()) - o.getY()) / d.getY();

        if(tymin > tymax)
            std::swap(tymin, tymax);

        if((tmin > tymax) || (tymin > tmax))
            return false;

        if(tymin > tmin)
            tmin = tymin;

        if(tymax < tmax)
            tmax = tymax;

        float tzmin = (std::min<float>(bottomLeft.getZ(), topRight.getZ()) - o.getZ()) / d.getZ();
        float tzmax = (std::max<float>(bottomLeft.getZ(), topRight.getZ()) - o.getZ()) / d.getZ();

        if(tzmin > tzmax)
            std::swap(tzmin, tzmax);

        if((tmin > tzmax) || (tzmin > tmax))
            return false;

        if(tzmin > tmin)
            tmin = tzmin;

        if(tzmax < tmax)
            tmax = tzmax;

        t1 = tmin;
        t2 = tmax;
        return true;
    }

    Vec3f normal(const Vec3f& p) const {
        float EPS = 1E-03;
        if(std::abs(p.getX() - std::min(bottomLeft.getX(), topRight.getX())) < EPS)
            return Vec3f(-1,0,0);
        else if(std::abs(p.getX() - std::max(bottomLeft.getX(), topRight.getX())) < EPS)
            return Vec3f(1,0,0);
        else if(std::abs(p.getY() - std::min(bottomLeft.getY(), topRight.getY())) < EPS)
            return Vec3f(0,-1,0);
        else if(std::abs(p.getY() - std::max(bottomLeft.getY(), topRight.getY())) < EPS)
            return Vec3f(0,1,0);
        else if(std::abs(p.getZ() - std::min(bottomLeft.getZ(), topRight.getZ())) < EPS)
            return Vec3f(0,0,-1);
        else if(std::abs(p.getZ() - std::max(bottomLeft.getZ(), topRight.getZ())) < EPS)
            return Vec3f(0,0,1);
        return Vec3f();
    }

    std::string material;
    Vec3f bottomLeft;
    Vec3f topRight;
};

struct Triangle
{
    bool intersect(const Vec3f& o, const Vec3f& d, float& t) const {
        Vec3f n = normal();
        float proj = d.dotProduct(n);
        t = (v1 - o).dotProduct(n) / proj;

        Vec3f pi = o + d * t;
        float a1 = n.dotProduct(Vec3f::crossProduct(v3 - v2, pi - v2)) / 2.;
        float a2 = n.dotProduct(Vec3f::crossProduct(v1 - v3, pi - v3)) / 2.;
        float a3 = n.dotProduct(Vec3f::crossProduct(v2 - v1, pi - v1)) / 2.;
        float a = a1 + a2 + a3;

        float l1 = a1 / a;
        float l2 = a2 / a;
        float l3 = a3 / a;
        return l1 >= 0 && l2 >= 0 && l3 >= 0 && l1 <= 1 && l2 <= 1 && l3 <= 1;
    }
    Vec3f normal() const {
        return Vec3f::crossProduct(v2 - v1, v3 - v2).normalized();
    }

    std::string material;
    Vec3f v1;
    Vec3f v2;
    Vec3f v3;
    Position2D tv1;
    Position2D tv2;
    Position2D tv3;
};

struct ObjectIntersection
{
    Vec3f p;
    Vec3f n;
    Material material;
    bool valid = false;
};

class RT5
{
public:
    Image render();
    bool load(const std::string& filename);

private:
    ObjectIntersection intersection(const Vec3f& o, const Vec3f& d);
    Pixel trace(const Vec3f& o, const Vec3f& d, int depth);
    Pixel shade(const Vec3f& o, const Vec3f& d, const Vec3f& n, const Vec3f& p, const Material& material, int depth);

    std::string parseString(FILE *fp);
    RGB parseRGB(FILE *fp);
    Vec3f parsePosition(FILE *fp);
    Position2D parsePosition2D(FILE *fp);
    Scene parseScene(FILE *fp);
    Camera parseCamera(FILE *fp);
    Material parseMaterial(FILE *fp);
    Light parseLight(FILE *fp);
    Sphere parseSphere(FILE *fp);
    Box parseBox(FILE *fp);
    Triangle parseTriangle(FILE *fp);

    Scene m_scene;
    Camera m_camera;
    std::map<std::string, Material> m_materials;
    std::vector<Light> m_lights;
    std::vector<Sphere> m_spheres;
    std::vector<Box> m_boxes;
    std::vector<Triangle> m_triangles;
};

#endif
