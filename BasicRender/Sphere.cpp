#include "Sphere.h"

namespace sparrow {
	bool Sphere::hit(const RRay& r, double tmin, double tmax, HitRecord& rec) const
	{
		Vector3f oc = r.origin() - center;
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
				Vector3f outward = (rec.p - center) / radius;
				rec.setFrontNormal(r, outward);
				return true;
			}
			tmp = (-halfb + root) / a;
			if (tmp<tmax && tmp>tmin) {
				rec.t = tmp;
				rec.p = r.at(tmp);
				Vector3f outward = (rec.p - center) / radius;
				rec.setFrontNormal(r, outward);
				return true;
			}
		}
		return false;
	}
}
