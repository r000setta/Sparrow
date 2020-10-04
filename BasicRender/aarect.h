#pragma once

#include "geometry.h"
#include "hittable.h"

namespace sparrow {
	class XYRect :public Hittable {
	public:
		shared_ptr<Material> mp;
		Float x0, x1, y0, y1, k;
	public:
		XYRect() {}
		XYRect(Float x0, Float x1, Float y0, Float y1, Float k, shared_ptr<Material> mat)
			:x0(x0), x1(x1), y0(y0), y1(y1), k(k), mp(mat) {}
		virtual bool hit(const RRay& r, double tmin, double tmax, HitRecord& rec) const override;
	};

	class XZRect:public Hittable{
	public:
		shared_ptr<Material> mp;
		Float x0, x1, z0, z1, k;
	public:
		XZRect() {}
		XZRect(Float x0, Float x1, Float z0, Float z1, Float k, shared_ptr<Material> mat)
			:x0(x0), x1(x1), z0(z0), z1(z1), k(k), mp(mat) {}
		virtual bool hit(const RRay& r, double tmin, double tmax, HitRecord& rec) const override;
	};

	class YZRect :public Hittable {
	public:
		shared_ptr<Material> mp;
		Float y0, y1, z0, z1, k;
	public:
		YZRect() {}
		YZRect(Float y0, Float y1, Float z0, Float z1, Float k, shared_ptr<Material> mat)
			:y0(y0), y1(y1), z0(z0), z1(z1), k(k), mp(mat) {}
		virtual bool hit(const RRay& r, double tmin, double tmax, HitRecord& rec) const override;
	};
}