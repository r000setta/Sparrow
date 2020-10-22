#include "hittable.h"

namespace sparrow {
	bool Translate::hit(const RRay& r, double tmin, double tmax, HitRecord& rec) const
	{
		RRay moved(r.origin() - offset, r.direction(), r.time());
		if (!ptr->hit(moved, tmin, tmax, rec))
			return false;
		rec.p += offset;
		rec.setFrontNormal(moved, rec.normal);
		return true;
	}

	RotateY::RotateY(shared_ptr<Hittable> p, Float angle)
	{
		ptr = p;
		auto radians = Radians(angle);
		sinTheta = sin(radians);
		cosTheta = cos(radians);
		hasBox = false;
	}
	bool RotateY::hit(const RRay& r, double tmin, double tmax, HitRecord& rec) const
	{
		auto origin = r.origin();
		auto direction = r.direction();

		origin[0] = cosTheta * r.origin()[0] - sinTheta * r.origin()[2];
		origin[2] = sinTheta * r.origin()[0] + cosTheta * r.origin()[2];

		direction[0] = cosTheta * r.direction()[0] - sinTheta * r.direction()[2];
		direction[2] = sinTheta * r.direction()[0] + cosTheta * r.direction()[2];

		RRay rotatedR(origin, direction, r.time());

		if (!ptr->hit(rotatedR, tmin, tmax, rec))
			return false;
		auto p = rec.p;
		auto normal = rec.normal;

		p[0] = cosTheta * rec.p[0] + sinTheta * rec.p[2];
		p[2] = -sinTheta * rec.p[0] + cosTheta * rec.p[2];

		normal[0] = cosTheta * rec.normal[0] + sinTheta * rec.normal[2];
		normal[2] = -sinTheta * rec.normal[0] + cosTheta * rec.normal[2];

		rec.p = p;
		rec.setFrontNormal(rotatedR, normal);

		return true;
	}
}
