#pragma once
#include "hittable.h"

namespace sparrow {
	class Sphere :public Hittable {
	public:
		Point3f center;
		Float radius;
		unsigned int material;
	public:
		Sphere() {}
		Sphere(Point3f cen, Float r, unsigned int mat)
			:center(cen), radius(r), material(mat) {}
		virtual bool hit(const RRay& r, double tmin, double tmax, HitRecord& rec) const override;
	};

	inline void GetSphereUV(const Vector3f p, Float& u, Float& v) {
		auto phi = atan2(p.z, p.x);
		auto theta = asin(p.y);
		u = 1 - (phi + Pi) / (2 * Pi);
		v = (theta + Pi / 2) / Pi;
	}
}