#pragma once
#include "ray.h"

namespace sparrow {
	class BCamera {
	public:
		BCamera() {
			auto aspectRatio = 16.9 / 9.0;
			auto viewportHeight = 2.0;
			auto viewportWidth = aspectRatio * viewportHeight;
			auto focalLen = 1.0;
			origin = Point3f(0, 0, 0);
			horizontal = Vector3f(viewportWidth, 0, 0);
			vertical = Vector3f(0, viewportHeight, 0);
			lowerLeftCorner = origin - horizontal / 2 - vertical / 2 - Vector3f(0, 0, focalLen);
		}

		RRay getRay(Float u, Float v) const {
			return RRay(origin, lowerLeftCorner + u * horizontal + v * vertical - origin);
		}

	private:
		Point3f origin;
		Point3f lowerLeftCorner;
		Vector3f horizontal;
		Vector3f vertical;
	};
}