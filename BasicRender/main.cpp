#include <iostream>
#include "color.h"
#include "ray.h"
#include "hittable_list.h"
#include "Sphere.h"

using namespace sparrow;

Color RayColor(const RRay& r,const Hittable& world) {
    HitRecord rec;
    if (world.hit(r, 0, Infinity, rec)) {
        return 0.5 * (rec.normal + Color(1, 1, 1));
    }
    auto unitDirection = Normalize(r.direction());
    auto t = 0.5 * (unitDirection.y + 1.0);
    return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

int main() {
    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // Camera

    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = Point3f(0, 0, 0);
    auto horizontal = Vector3f(viewport_width, 0, 0);
    auto vertical = Vector3f(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - Vector3f(0, 0, focal_length);

    HittableList world;
    world.add(make_shared<Sphere>(Point3f(0, 0, -1), 0.5));
    world.add(make_shared<Sphere>(Point3f(0, -100.5, -1), 100));

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        for (int i = 0; i < image_width; ++i) {
            auto u = double(i) / (image_width - 1);
            auto v = double(j) / (image_height - 1);
            RRay r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
            Color p = RayColor(r,world);
            WriteColor(std::cout, p);
        }
    }
    std::cerr << "\nDone!\n";
}