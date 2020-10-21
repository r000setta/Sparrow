#pragma once
#include "ray.h"

namespace sparrow {
	class BCamera {
	public:
		BCamera(const Point3f& cameraPos,
			const Point3f& tar,
			Float vfov,Float aspectRatio,
			Float aperture,Float fDist,
			Float t0=0,Float t1=0) {

			pos = cameraPos;
			target = tar;
			fovy = vfov;
			aspect = aspectRatio;
			lensRadius = aperture*0.5f;
			focusDist = fDist;

			time0 = t0;
			time1 = t1;
		}

		RRay getRay(Float s, Float t) const {
			Vector3f rd = lensRadius * RandomInUnitDisk<Float>();
			Vector3f offset = axisX * rd.x + axisY * rd.y;
			return RRay(pos + offset, 
				lowerLeftCorner + s * horizontal + t * vertical - pos - offset,
				RandomFloat(time0, time1));
		}

		void setPosition(const Point3f& p) { pos = p; update(); }
		void setLenradius(const float& radius) { lensRadius = radius; update(); }
		void setTarget(const Point3f& _tar) { target = _tar; update(); }
		void setFovy(const float& fov) { fovy = fov; update(); }
		void setAspect(const float& asp) { aspect = asp; update(); }
		void setFoucusDist(const float& fdist) { focusDist = fdist; update(); }


	private:
		Point3f pos;
		Point3f target;
		Point3f lowerLeftCorner;
		Vector3f horizontal;
		Vector3f vertical;
		Float fovy, aspect;
		Float lensRadius, focusDist;
		Vector3f axisX, axisY, axisZ;
		Float time0, time1;

		void update();
	};
}