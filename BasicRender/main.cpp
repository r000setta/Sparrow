#include <iostream>
#include "color.h"
#include "ray.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include "lambertian.h"
#include "metal.h"
#include "dielectric.h"
#include "moving_sphere.h"

using namespace sparrow;

Color RayColor(const RRay& r,const Hittable& world,int depth){
    if (depth <= 0) {
        return Color(0, 0, 0);
    }
    HitRecord rec;
    if (world.hit(r, 0.001, Infinity, rec)) {
        RRay scattered;
        Color attenuation;
        if (rec.matPtr->scatter(r, rec, attenuation, scattered))
            return EleDot(attenuation,RayColor(scattered, world, depth - 1));
        return Color(0, 0, 0);
    }
    auto unitDirection = Normalize(r.direction());
    auto t = 0.5 * (unitDirection.y + 1.0);
    return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

int main() {
    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 1920;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int spp = 30;
    const int maxDepth = 30;
    
    Point3f lookfrom(3, 3, 2);
    Point3f lookat(0, 0, -1);
    Vector3f vup(0, 1, 0);
    auto distToFocus = (lookfrom - lookat).Length();
    auto aperture = 0.1;

    // Camera
    BCamera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, distToFocus, 0.0, 2.0);

    auto matLambertian=make_shared<Lambertian>(Color(0, 0.53, 0.73));
    //auto matCenter=make_shared<Dielectric>(1.5);
    //auto matLeft=make_shared<Metal>(Color(0.8, 0.8, 0.8),0.3);
    auto matDielectric=make_shared<Dielectric>(1.5);
    auto matMetal=make_shared<Metal>(Color(0.7, 0.3, 0.3),0.2);
    auto checker = make_shared<CheckerTexture>(Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));
    auto pertext = make_shared<NoiseTexture>(4);
    auto perlinMat = make_shared<Lambertian>(pertext);
    auto matGround = make_shared<Lambertian>(checker);

    HittableList world;
    Point3f cen(1.0, 0.0, -1.0);
    auto cen2 = cen + Vector3f(0, RandomFloat(0, .5), 0);
    //ground
    world.add(make_shared<Sphere>(Point3f(0.0, -100.5, -1.0), 100.0, perlinMat));
    //center
    world.add(make_shared<Sphere>(Point3f(0.0, 0.0, -1.0), 0.5, matMetal));
    //right
    world.add(make_shared<Sphere>(Point3f(1.0, 0.0, -1.0), 0.5, matLambertian));
    //left
    world.add(make_shared<Sphere>(Point3f(-1.0, 0.0, -1.0), 0.5, matDielectric));
    //world.add(make_shared<MovingSphere>(cen, cen2, 0.0, 1.0, 0.2, matLeft));

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        for (int i = 0; i < image_width; ++i) {
            Color p(0, 0, 0);
            for (int s = 0; s < spp; ++s) {
                auto u = (i + RandomFloat()) / (image_width - 1);
                auto v = (j + RandomFloat()) / (image_height - 1);
                RRay r = cam.getRay(u, v);
                p += RayColor(r, world, maxDepth);
            }
            WriteColor(std::cout, p, spp);
        }
    }
    std::cerr << "\nDone!\n";
}