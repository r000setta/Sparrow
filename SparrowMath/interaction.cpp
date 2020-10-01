#include "interaction.h"
#include "transform.h"
#include "primitive.h"
#include "shape.h"

namespace sparrow {
    SurfaceInteraction::SurfaceInteraction(
        const Point3f& p, const Vector3f& pError, const Point2f& uv,
        const Vector3f& wo, const Vector3f& dpdu, const Vector3f& dpdv,
        const Normal3f& dndu, const Normal3f& dndv, Float time, const Shape* shape,
        int faceIndex)
    {

    }
}

