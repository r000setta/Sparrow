#pragma once
#include "sparrow.h"
#include "shape.h"
#include "transform.h"

namespace sparrow {
	class Primitive {
		virtual ~Primitive() {}
		virtual Bounds3f WorldBound() const = 0;
		virtual bool Intersect(const Ray& r, SurfaceInteraction*) const = 0;
		virtual bool IntersectP(const Ray& r) const = 0;
		
	};
}