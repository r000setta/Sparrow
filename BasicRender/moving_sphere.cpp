#include "moving_sphere.h"

namespace sparrow {
	bool MovingSphere::hit(const RRay& r, double tmin, double tmax, HitRecord& rec) const
	{
		Vector3f oc = r.origin() - center(r.time());
		auto a = r.direction().LengthSquared();
		Float halfb = Dot(r.direction(), oc);
		auto c = oc.LengthSquared() - radius * radius;
		Float dis = halfb * halfb - a * c;
		if (dis > 0) {
			auto root = sqrt(dis);
			auto tmp = (-halfb - root) / a;
			if (tmp<tmax && tmp>tmin) {
				rec.t = tmp;
				rec.p = r.at(tmp);
				Vector3f outward = (rec.p - center(r.time())) / radius;
				rec.setFrontNormal(r, outward);
				rec.material = material;
				return true;
			}
			tmp = (-halfb + root) / a;
			if (tmp<tmax && tmp>tmin) {
				rec.t = tmp;
				rec.p = r.at(tmp);
				Vector3f outward = (rec.p - center(r.time())) / radius;
				rec.setFrontNormal(r, outward);
				rec.material = material;
				return true;
			}
		}
		return false;
	}
}

