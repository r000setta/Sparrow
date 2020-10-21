#include "aarect.h"

namespace sparrow {
	bool XYRect::hit(const RRay& r, double tmin, double tmax, HitRecord& rec) const
	{
		auto t = (k - r.origin().z) / r.direction().z;
		if (t<tmin || t>tmax) return false;
		auto x = r.origin().x + t * r.direction().x;
		auto y = r.origin().y + t * r.direction().y;
		if (x<x0 || x>x1 || y<y0 || y>y1) return false;
		rec.u = (x - x0)/(x1 - x0);
		rec.v = (y - y0)/(y1 - y0);
		rec.t = t;
		auto outwardNormal = Vector3f(0, 0, 1);
		rec.setFrontNormal(r, outwardNormal);
		rec.material = mp;
		rec.p = r.at(t);
		return true;
	}
	bool XZRect::hit(const RRay& r, double tmin, double tmax, HitRecord& rec) const
	{
		auto t = (k - r.origin().y) / r.direction().y;
		if (t<tmin || t>tmax) return false;
		auto x = r.origin().x + t * r.direction().x;
		auto z = r.origin().z + t * r.direction().z;
		if (x<x0 || x>x1 || z<z0 || z>z1) return false;
		rec.u = (x - x0) / (x1 - x0);
		rec.v = (z - z0) / (z1 - z0);
		rec.t = t;
		auto outwardNormal = Vector3f(0, 1, 0);
		rec.setFrontNormal(r, outwardNormal);
		rec.material = mp;
		rec.p = r.at(t);
		return true;
	}
	bool YZRect::hit(const RRay& r, double tmin, double tmax, HitRecord& rec) const
	{
		auto t = (k - r.origin().x) / r.direction().x;
		if (t<tmin || t>tmax) return false;
		auto y = r.origin().y + t * r.direction().y;
		auto z = r.origin().z + t * r.direction().z;
		if (y<y0 || y>y1 || z<z0 || z>z1) return false;
		rec.u = (y - y0) / (y1 - y0);
		rec.v = (z - z0) / (z1 - z0);
		rec.t = t;
		auto outwardNormal = Vector3f(1, 0, 0);
		rec.setFrontNormal(r, outwardNormal);
		rec.material = mp;
		rec.p = r.at(t);
		return true;
	}
}

