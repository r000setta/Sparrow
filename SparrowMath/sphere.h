#pragma once
#include "shape.h"

namespace sparrow {
	class Sphere :public Shape {
        Sphere(const Transform* ObjectToWorld, const Transform* WorldToObject,
            bool reverseOrientation, Float radius, Float zMin, Float zMax,
            Float phiMax)
            : Shape(ObjectToWorld, WorldToObject, reverseOrientation),
            radius(radius),
            zMin(Clamp(std::min(zMin, zMax), -radius, radius)),
            zMax(Clamp(std::max(zMin, zMax), -radius, radius)),
            thetaMin(std::acos(Clamp(std::min(zMin, zMax) / radius, -1, 1))),
            thetaMax(std::acos(Clamp(std::max(zMin, zMax) / radius, -1, 1))),
            phiMax(Radians(Clamp(phiMax, 0, 360))) {}
        Bounds3f ObjectBound() const;
        bool Intersect(const Ray& ray, Float* tHit, SurfaceInteraction* isect,
            bool testAlphaTexture) const;
        bool IntersectP(const Ray& ray, bool testAlphaTexture) const ;
        Float Area() const ;
	private:
		const Float radius;
		const Float zMin, zMax;
		const Float thetaMin, thetaMax, phiMax;
	};
}