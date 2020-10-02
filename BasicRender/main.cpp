#include <iostream>
#include "color.h"
#include "ray.h"

using namespace sparrow;

bool HitSphere(const Point3f& center, Float radius, const RRay& r) {
    Vector3f oc = r.origin() - center;
    auto a = Dot(r.direction(), r.direction());
    auto b = 2.0 * Dot(r.direction(), oc);
    auto c = Dot(oc, oc) - radius * radius;
    auto dis = b * b - 4 * a * c;
    return dis > 0;
}

Color RayColor(const RRay& r) {
    if (HitSphere(Point3f(0, 0, -1), 0.5, r)) {
        return Color(1, 0, 0);
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

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        for (int i = 0; i < image_width; ++i) {
            auto u = double(i) / (image_width - 1);
            auto v = double(j) / (image_height - 1);
            RRay r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
            Color p = RayColor(r);
            WriteColor(std::cout, p);
        }
    }
    std::cerr << "\nDone!\n";
}