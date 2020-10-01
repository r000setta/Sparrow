#pragma once
#include "sparrow.h"
#include "geometry.h"
#include "transform.h"

namespace sparrow {
	struct Interaction {
		Interaction() :time(0) {}
		Interaction(const Point3f& p, const Normal3f& n,
			const Vector3f& pError, const Vector3f& wo, const Float time) :p(p), time(time), pError(pError), wo(wo), n(n) {}
		bool IsSurfaceInteration() const {
			return n != Normal3f();
		}
		/*
		All interactions must have a point and time associated with it
		*/
		Point3f p;
		Float time;
		Vector3f pError;	//gives a conservative bound on floating-point error
		Vector3f wo;	//the negative ray
		Normal3f n;		//for interactions on surfaces,n stores the surface normal at the point
		// TODO Medium
	};

	/*
	The geometry of a particular point on a surface 
	(often a position found by intersecting a ray against the surface)
	Having this abstraction lets most of the system work with points on surfaces 
	without needing to consider the particular type of geometric shape the points lie on
	*/
	class SurfaceInteraction :public Interaction {
	public:
		SurfaceInteraction() {}
		SurfaceInteraction(const Point3f& p, const Vector3f& pError,
			const Point2f& uv, const Vector3f& wo,
			const Vector3f& dpdu, const Vector3f& dpdv,
			const Normal3f& dndu, const Normal3f& dndv, Float time,
			const Shape* sh,
			int faceIndex = 0);
		Point2f uv;
		Vector3f dpdu, dpdv;
		Normal3f dndu, dndv;
		const Shape* shape = nullptr;	//the shape that the point lies on
		struct {
			Normal3f n;
			Vector3f dpdu, dpdv;
			Normal3f dndu, dndv;
		}shading;
		mutable Vector3f dpdx, dpdy;
		mutable Float dudx = 0, dvdx = 0, dudy = 0, dvdy = 0;
	};
}