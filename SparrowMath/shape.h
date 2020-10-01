#pragma once
#include "sparrow.h"
#include "geometry.h"
#include "transform.h"

namespace sparrow {
	class Shape {
	public:
		const Transform* ObjectToWorld, * WorldToObject;
		const bool reverseOrientation;
		const bool transformSwapsHandedness;
		
		Shape(const Transform* ObjectToWorld, const Transform* WorldToObject, bool reverseOrientation)
			:ObjectToWorld(ObjectToWorld),
			WorldToObject(WorldToObject),
			reverseOrientation(reverseOrientation),
			transformSwapsHandedness(ObjectToWorld->SwapsHandedness()){
		}

		virtual ~Shape() {}
		virtual Bounds3f ObjectBound() const = 0;
		virtual Bounds3f WorldBound() const;
		virtual bool Intersect(const Ray& ray, Float* tHit, 
			SurfaceInteraction* isect, bool testAlphaTexture = true) const = 0;
		virtual bool IntersectP(const Ray& ray, bool testAlphaTexture = true) const {
			return Intersect(ray, nullptr, nullptr, testAlphaTexture);
		}
		virtual Float Area() const = 0;
	};
}