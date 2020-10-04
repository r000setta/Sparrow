#include "box.h"

namespace sparrow {
	Box::Box(const Point3f& p0, const Point3f& p1, shared_ptr<Material> mat)
	{
		boxMin = p0;
		boxMax = p1;
		sides.add(make_shared<XYRect>(p0.x, p1.x, p0.y, p1.y, p1.z, mat));
		sides.add(make_shared<XYRect>(p0.x, p1.x, p0.y, p1.y, p0.z, mat));
		sides.add(make_shared<XZRect>(p0.x, p1.x, p0.z, p1.z, p1.y, mat));
		sides.add(make_shared<XZRect>(p0.x, p1.x, p0.z, p1.z, p0.y, mat));
		sides.add(make_shared<YZRect>(p0.y, p1.y, p0.z, p1.z, p1.x, mat));
		sides.add(make_shared<YZRect>(p0.y, p1.y, p0.z, p1.z, p0.x, mat));
	}
	bool Box::hit(const RRay& r, double tmin, double tmax, HitRecord& rec) const
	{
		return sides.hit(r, tmin, tmax, rec);
	}
}
