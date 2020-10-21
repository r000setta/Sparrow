#include "camera.h"
#include "geometry.h"

namespace sparrow {
	void sparrow::BCamera::update()
	{
		const Vector3f up(0, 1, 0);
		auto theta = Radians(fovy);
		Float half_height = static_cast<Float>(tan(theta * 0.5f)) * focusDist;
		Float half_width = aspect * half_height;

		auto h = tan(theta / 2);
		auto viewportHeight = 2.0 * h;
		auto viewportWidth = aspect * viewportHeight;

		auto w = Normalize(pos - target);
		auto u = Normalize(Cross(up, w));
		auto v = Cross(w, u);

		horizontal = viewportWidth * u;
		vertical = viewportHeight * v;
		lowerLeftCorner = pos - horizontal / 2 - vertical / 2 - w;
	}
}

