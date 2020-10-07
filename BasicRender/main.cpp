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
#include "diffuse_light.h"
#include "aarect.h"
#include "box.h"

using namespace sparrow;

Color RayColor(const RRay& r,const Color& background, const Hittable& world, int depth){
    HitRecord rec;
    if (depth <= 0) {
        return Color(0, 0, 0);
    }
    if (!world.hit(r, 0.001, Infinity, rec)) {
        return background;
    }

    RRay scattered;
    Color attenuation;
    Color emitted = rec.matPtr->emitted(rec.u, rec.v, rec.p);

    Float pdf;
    Color albedo;
    if (!rec.matPtr->scatter(r, rec, albedo, scattered,pdf))
        return emitted;

    shared_ptr<Hittable> lightShape = make_shared<XZRect>(213, 343, 227, 332, 554, shared_ptr<Material>());
    auto p0 = make_shared<HittablePDF>(lightShape, rec.p);
    auto p1 = make_shared<ConsinePDF>(rec.normal);
    MixturePDF p(p0, p1);

    scattered = RRay(rec.p, p.generate(), r.time());
    pdf = p.value(scattered.direction());

    return emitted
        + EleDot(albedo * rec.matPtr->scatteringPDF(r, rec, scattered)
            , RayColor(scattered, background, world, depth - 1)) / pdf;
}

HittableList SimpleLight() {
    auto ground = make_shared<Lambertian>(Color(0.2, 0.3, 0.1));
    auto red = make_shared<Lambertian>(Color(.65, .05, .05));
    HittableList objects;
    auto pertext = make_shared<NoiseTexture>(4);
    objects.add(make_shared<Sphere>(Point3f(0, -1000, 0), 1000, ground));
    objects.add(make_shared<Sphere>(Point3f(0, 2, 0), 2, red));

    auto diffLight = make_shared<DiffuseLight>(Color(4, 4, 4));
    objects.add(make_shared<XYRect>(3, 5, 1, 3, -2, diffLight));
    return objects;
}

HittableList CornellBox() {
    HittableList objects;
    auto red = make_shared<Lambertian>(Color(.65, .05, .05));
    auto white = make_shared<Lambertian>(Color(.73, .73, .73));
    auto green = make_shared<Lambertian>(Color(.12, .45, .15));
    auto light = make_shared<DiffuseLight>(Color(15, 15, 15));

    objects.add(make_shared<YZRect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<YZRect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<FlipFace>(make_shared<XZRect>(213, 343, 227, 332, 554, light)));
    objects.add(make_shared<XZRect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<XZRect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<XYRect>(0, 555, 0, 555, 555, white));

    objects.add(make_shared<Box>(Point3f(130, 0, 65), Point3f(295, 165, 230), white));
    objects.add(make_shared<Box>(Point3f(265, 0, 295), Point3f(430, 330, 460), white));

    return objects;
}

int main() {
    // Image
    Float aspectRatio = 16.0 / 9.0;
    Float imageWidth = 800;
    int spp = 5;
    int maxDepth = 3;
    Float vfov = 40.0;
    
    // Right up zom in
    //Point3f lookfrom(3, 3, 2);

    Point3f lookfrom(3, 3, 2);
    Point3f lookat(0, 0, 0);
    Vector3f vup(0, 1, 0);
    Float aperture = 0.0;

    //auto matLambertian=make_shared<Lambertian>(Color(0, 0.53, 0.73));
    //auto matCenter=make_shared<Dielectric>(1.5);
    //auto matLeft=make_shared<Metal>(Color(0.8, 0.8, 0.8),0.3);
    //auto matDielectric=make_shared<Dielectric>(1.5);
    //auto matMetal=make_shared<Metal>(Color(0.7, 0.3, 0.3),0.2);
    //auto checker = make_shared<CheckerTexture>(Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));
    //auto pertext = make_shared<NoiseTexture>(4);
    //auto perlinMat = make_shared<Lambertian>(pertext);
    //auto matChecker = make_shared<Lambertian>(checker);

    //auto earthTexture = make_shared<ImageTexture>("D:\\Sparrow\\Texture\\earthmap.jpg");
    //auto earthMat = make_shared<Lambertian>(earthTexture);

    Color background{ 0,0,0 };

    HittableList world;
    switch (0)
    {
    case 0:
        world = CornellBox();
        aspectRatio = 1.0;
        imageWidth = 800;
        spp = 400;
        background = Color(0, 0, 0);
        lookfrom = Point3f(278, 278, -800);
        lookat = Point3f(278, 278, 0);
        vfov = 40.0;
        break;
    case 1:
        world = SimpleLight();
        spp = 400;
        lookfrom = Point3f(3, 3, 2);
        lookat = Point3f(0, 0, 1);
        vfov = 20.0;
        maxDepth = 10;
        break;
    default:
        break;
    }

    auto distToFocus = (lookfrom - lookat).Length();
    int imageHeight = static_cast<int>(imageWidth / aspectRatio);

    BCamera cam(lookfrom, lookat, vup, vfov, aspectRatio, aperture, distToFocus, 0.0, 2.0);
    //ground
    //world.add(make_shared<Sphere>(Point3f(0.0, -100.5, -1.0), 100.0, matChecker));
    //center
    //world.add(make_shared<Sphere>(Point3f(0.0, 0.0, -1.0), 0.5, matMetal));
    //right
    //world.add(make_shared<Sphere>(Point3f(1.0, 0.0, -1.0), 0.5, earthMat));
    //left
    //world.add(make_shared<Sphere>(Point3f(-1.0, 0.0, -1.0), 0.5, matDielectric));
    //world.add(make_shared<MovingSphere>(cen, cen2, 0.0, 1.0, 0.2, matLeft));

    std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

    for (int j = imageHeight - 1; j >= 0; --j) {
        for (int i = 0; i < imageWidth; ++i) {
            Color p(0, 0, 0);
            for (int s = 0; s < spp; ++s) {
                auto u = (i + RandomFloat()) / (imageWidth - 1);
                auto v = (j + RandomFloat()) / (imageHeight - 1);
                RRay r = cam.getRay(u, v);
                p += RayColor(r, background, world, maxDepth);
            }
            WriteColor(std::cout, p, spp);
        }
    }
    std::cerr << "\nDone!\n";
}