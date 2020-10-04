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
}
