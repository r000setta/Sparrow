#pragma once

#include "geometry.h"
#include "hittable.h"

namespace sparrow {
	class XYRect :public Hittable {
	public:
		unsigned int mp;
		Float x0, x1, y0, y1, k;
	public:
		XYRect() {}
		XYRect(Float x0, Float x1, Float y0, Float y1, Float k, unsigned int mat)
			:x0(x0), x1(x1), y0(y0), y1(y1), k(k), mp(mat) {}
		virtual bool hit(const RRay& r, double tmin, double tmax, HitRecord& rec) const override;
	};

	class XZRect:public Hittable{
	public:
		unsigned int mp;
		Float x0, x1, z0, z1, k;
	public:
		XZRect() {}
		XZRect(Float x0, Float x1, Float z0, Float z1, Float k, unsigned int mat)
			:x0(x0), x1(x1), z0(z0), z1(z1), k(k), mp(mat) {}
		virtual bool hit(const RRay& r, double tmin, double tmax, HitRecord& rec) const override;

		virtual Float PDFvalue(const Point3f& o, const Vector3f& v) const override {
			HitRecord rec;
			if (!this->hit(RRay(o, v), 0.001, Infinity, rec)) return 0;

			auto area = (x1 - x0) * (z1 - z0);
			auto distanceSquared = rec.t * rec.t * v.LengthSquared();
			auto cosine = fabs(Dot(v, rec.normal) / v.Length());
			return distanceSquared / (cosine * area);
		}

		virtual Vector3f Random(const Point3f& o) const override {
			auto randomPoint = Point3f(RandomFloat(x0, x1), k, RandomFloat(z0, z1));
			return randomPoint - o;
		}
	};

	class YZRect :public Hittable {
	public:
		unsigned int mp;
		Float y0, y1, z0, z1, k;
	public:
		YZRect() {}
		YZRect(Float y0, Float y1, Float z0, Float z1, Float k, unsigned int mat)
			:y0(y0), y1(y1), z0(z0), z1(z1), k(k), mp(mat) {}
		virtual bool hit(const RRay& r, double tmin, double tmax, HitRecord& rec) const override;
	};
}