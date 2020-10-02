#pragma once
#include "hittable.h"

namespace sparrow {
	class Sphere :public Hittable {
	public:
		Point3f center;
		Float radius;
		shared_ptr<Material> matPtr;
	public:
		Sphere() {}
		Sphere(Point3f cen, Float r, shared_ptr<Material> m)
			:center(cen), radius(r), matPtr(m) {}
		virtual bool hit(const RRay& r, double tmin, double tmax, HitRecord& rec) const override;
	};
}