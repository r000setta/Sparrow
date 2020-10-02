#pragma once
#include "geometry.h"

#include <iostream>

namespace sparrow {
	void WriteColor(std::ostream& out, Color PixelColor,int spp) {
		Float r = PixelColor.x;
		Float g = PixelColor.y;
		Float b = PixelColor.z;
		auto scale = 1.0 / spp;
		r *= scale;
		g *= scale;
		b *= scale;

		out << static_cast<int>(256 * Clamp(r, 0.0, 0.999)) << ' '
			<< static_cast<int>(256 * Clamp(g, 0.0, 0.999)) << ' '
			<< static_cast<int>(256 * Clamp(b, 0.0, 0.999)) << '\n';
	}
}