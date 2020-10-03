#pragma once
#include "geometry.h"
namespace sparrow {

	class RRay {
	public:
		RRay() {}
		RRay(const Point3f& orig, const Vector3f& dir, Float time = 0.0) :
			orig(orig), dir(dir), tm(time) {}
		Point3f origin() const { return orig; }
		Vector3f direction() const { return dir; }
		Float time() const { return tm; }
		Point3f at(Float t) const {
			return orig + t * dir;
		}
	private:
		Point3f orig;
		Vector3f dir;
		Float tm;
	};
}
