#pragma once
#include "geometry.h"

#include <iostream>

namespace sparrow {
	void WriteColor(std::ostream& out, Color PixelColor) {
		out << static_cast<int>(255.999 * PixelColor.x) << ' '
			<< static_cast<int>(255.999 * PixelColor.y) << ' '
			<< static_cast<int>(255.999 * PixelColor.z) << '\n';
	}
}