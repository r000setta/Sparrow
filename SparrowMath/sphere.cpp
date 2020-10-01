#include "sphere.h"

namespace sparrow {
	Bounds3f Sphere::ObjectBound() const
	{
		return Bounds3f(Point3f(-radius, -radius, zMin), Point3f(radius, radius, zMax));
	}

	bool Sphere::Intersect(const Ray& ray, Float* tHit, SurfaceInteraction* isect, bool testAlphaTexture) const
	{
		return false;
	}

	bool Sphere::IntersectP(const Ray& ray, bool testAlphaTexture) const
	{
		return false;
	}

	Float Sphere::Area() const
	{
		return phiMax * radius * (zMax - zMin);
	}

}
