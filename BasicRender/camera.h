#pragma once
#include "ray.h"

namespace sparrow {
	class BCamera {
	public:
		BCamera(Point3f lookfrom,
			Point3f lookat,
			Vector3f vup,
			Float vfov,Float aspectRatio,
			Float aperture,Float focusDist,
			Float t0=0,Float t1=0) {
			auto theta = Radians(vfov);
			auto h = tan(theta / 2);
			auto viewportHeight = 2.0 * h;
			auto viewportWidth = aspectRatio * viewportHeight;

			w = Normalize(lookfrom - lookat);
			u = Normalize(Cross(vup, w));
			v = Cross(w, u);
			origin = lookfrom;
			horizontal = viewportWidth * u * focusDist;
			vertical = viewportHeight * v * focusDist;
			lowerLeftCorner = origin - horizontal / 2 - vertical / 2 - w * focusDist;
			lensRadius = aperture / 2;

			time0 = t0;
			time1 = t1;
		}

		RRay getRay(Float s, Float t) const {
			Vector3f rd = lensRadius * RandomInUnitDisk<Float>();
			Vector3f offset = u * rd.x + v * rd.y;
			return RRay(origin + offset, lowerLeftCorner + s * horizontal + t * vertical - origin - offset,
				RandomFloat(time0, time1));
		}

	private:
		Vector3f u, v, w;
		Float lensRadius;
		Point3f origin;
		Point3f lowerLeftCorner;
		Vector3f horizontal;
		Vector3f vertical;
		Float time0, time1;
	};
}