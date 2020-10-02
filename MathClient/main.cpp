#include "geometry.h"
using namespace std;
using namespace sparrow;

void VectorTest() {
	Vector3f n(1.0f, 0.0f, 0.0f);
	Vector3f u(1.0f, 2.0f, 3.0f);
	Color v(-2.0f, 1.0f, -3.0f);
	Color w(0.707f, 0.707f, 0.0f);

	auto a = u + v;
	auto b = u - v;
	auto l = u.Length();
	auto d = Normalize(u);
	auto s = Dot(u, v);
	auto e = Cross(u, v);

	cout << "u=" << u << endl;
	cout << "v=" << v << endl;
	cout << "w=" << w << endl;
	cout << "n=" << n << endl;

	cout << "a=" << a << endl;
	cout << "b=" << b << endl;
	cout << "l=" << l << endl;
	cout << "d=" << d << endl;
	cout << "s=" << s << endl;
	cout << "e=" << e << endl;
}

int main() {
	
}