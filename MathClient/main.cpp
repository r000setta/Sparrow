#include "geometry.h"
#include <iomanip>
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

void MCPi() {
	int inside = 0;
	int insideStr = 0;
	int sqrtN = 10000;
	for (int i = 0; i < sqrtN; ++i) {
		for (int j = 0; j < sqrtN; ++j) {
			auto x = RandomFloat(-1, 1);
			auto y = RandomFloat(-1, 1);
			if (x * x + y * y < 1)
				inside++;
			x = 2 * ((i + RandomFloat()) / sqrtN) - 1;
			y = 2 * ((j + RandomFloat()) / sqrtN) - 1;
			if (x * x + y * y < 1)
				insideStr++;
		}
	}
	auto N = static_cast<Float>(sqrtN) * sqrtN;
	cout << fixed << setprecision(12);
	cout << 4 * Float(inside) / (N) << endl;
	cout << 4 * Float(insideStr) / (N) << endl;
}

inline Float pdf(const Vector3f& p) {
	return 1 / (4 * Pi);
}

int main() {
	int N = 1000000;
	auto sum = 0.0;
	for (int i = 0; i < N; ++i) {
		Vector3f d = RandomUnitVector<Float>();
		auto cosine = d.z * d.z;
		sum += cosine / pdf(d);
	}
	cout << fixed << setprecision(12);
	cout << sum / N << endl;
}