#pragma once
#include "sparrow.h"
#include "geometry.h"
#include "transform.h"

namespace sparrow {
	struct Interaction {
		Interaction() :time(0) {}
		Interaction(const Point3f& p, const Normal3f& n,
			const Vector3f& pError, const Vector3f& wo, const Float time) :p(p), time(time), pError(pError), wo(wo), n(n) {}

		Point3f p;
		Float time;
		Vector3f pError;
		Vector3f wo;
		Normal3f n;
		// TODO Medium
	};

	class SurfaceInteraction :public Interaction {

	};
}