#pragma once
#include "geometry.h"
namespace sparrow {

	class RRay {
	public:
		RRay() {}
		RRay(const Point3f& orig, const Vector3f& dir) :
			orig(orig), dir(dir) {}
		Point3f origin() const { return orig; }
		Vector3f direction() const { return dir; }
		Point3f at(Float t) const {
			return orig + t * dir;
		}
	private:
		Point3f orig;
		Vector3f dir;
	};
}
