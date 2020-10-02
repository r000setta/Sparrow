#pragma once
#include "hittable.h"

namespace sparrow {
	class Sphere :public Hittable {
	public:
		Point3f center;
		Float radius;
	public:
		Sphere() {}
		Sphere(Point3f cen, Float r) :center(cen), radius(r) {}
		virtual bool hit(const RRay& r, double tmin, double tmax, HitRecord& rec) const override;
	};
}