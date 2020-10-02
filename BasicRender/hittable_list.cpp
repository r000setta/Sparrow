#include "hittable_list.h"

namespace sparrow {
	bool HittableList::hit(const RRay& r, double tmin, double tmax, HitRecord& rec) const
	{
		HitRecord tmpRec;
		bool hitAny = false;
		auto closet = tmax;
		for (const auto& obj : objects) {
			if (obj->hit(r, tmin, closet, tmpRec)) {
				hitAny = true;
				closet = tmpRec.t;
				rec = tmpRec;
			}
		}
		return hitAny;
	}
}

