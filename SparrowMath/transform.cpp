#include "transform.h"

namespace sparrow {
	Matrix4x4 Transpose(const Matrix4x4& m)
	{
		return Matrix4x4(m.m[0][0], m.m[1][0], m.m[2][0], m.m[3][0], m.m[0][1],
			m.m[1][1], m.m[2][1], m.m[3][1], m.m[0][2], m.m[1][2],
			m.m[2][2], m.m[3][2], m.m[0][3], m.m[1][3], m.m[2][3],
			m.m[3][3]);
	}
	Matrix4x4 Inverse(const Matrix4x4& m)
	{
        int indxc[4], indxr[4];
        int ipiv[4] = { 0, 0, 0, 0 };
        Float minv[4][4];
        memcpy(minv, m.m, 4 * 4 * sizeof(Float));
        for (int i = 0; i < 4; i++) {
            int irow = 0, icol = 0;
            Float big = 0.f;
            // Choose pivot
            for (int j = 0; j < 4; j++) {
                if (ipiv[j] != 1) {
                    for (int k = 0; k < 4; k++) {
                        if (ipiv[k] == 0) {
                            if (std::abs(minv[j][k]) >= big) {
                                big = Float(std::abs(minv[j][k]));
                                irow = j;
                                icol = k;
                            }
                        }
                        else if (ipiv[k] > 1)
                            exit(1);
                            //Error("Singular matrix in MatrixInvert");
                    }
                }
            }
            ++ipiv[icol];
            // Swap rows _irow_ and _icol_ for pivot
            if (irow != icol) {
                for (int k = 0; k < 4; ++k) std::swap(minv[irow][k], minv[icol][k]);
            }
            indxr[i] = irow;
            indxc[i] = icol;
            if (minv[icol][icol] == 0.f) exit(1); //Error("Singular matrix in MatrixInvert");

            // Set $m[icol][icol]$ to one by scaling row _icol_ appropriately
            Float pivinv = 1. / minv[icol][icol];
            minv[icol][icol] = 1.;
            for (int j = 0; j < 4; j++) minv[icol][j] *= pivinv;

            // Subtract this row from others to zero out their columns
            for (int j = 0; j < 4; j++) {
                if (j != icol) {
                    Float save = minv[j][icol];
                    minv[j][icol] = 0;
                    for (int k = 0; k < 4; k++) minv[j][k] -= minv[icol][k] * save;
                }
            }
        }
        // Swap columns to reflect permutation
        for (int j = 3; j >= 0; j--) {
            if (indxr[j] != indxc[j]) {
                for (int k = 0; k < 4; k++)
                    std::swap(minv[k][indxr[j]], minv[k][indxc[j]]);
            }
        }
        return Matrix4x4(minv);
	}
	void Matrix4x4::Print(FILE* f) const
	{
		fprintf(f, "[ ");
		for (auto i = 0; i < 4; ++i) {
			fprintf(f, " [ ");
			for (auto j = 0; j < 4; ++j) {
				fprintf(f, "%f", m[i][j]);
				if (j != 3) fprintf(f, ", ");
			}
			fprintf(f, "]\n");
		}
		fprintf(f, " ] ");
	}

	Bounds3f Transform::operator()(const Bounds3f& b) const {
		const Transform& M = *this;
		Bounds3f ret(M(Point3f(b.pMin.x, b.pMin.y, b.pMin.z)));
		ret = Union(ret, M(Point3f(b.pMax.x, b.pMin.y, b.pMin.z)));
		ret = Union(ret, M(Point3f(b.pMin.x, b.pMax.y, b.pMin.z)));
		ret = Union(ret, M(Point3f(b.pMin.x, b.pMin.y, b.pMax.z)));
		ret = Union(ret, M(Point3f(b.pMin.x, b.pMax.y, b.pMax.z)));
		ret = Union(ret, M(Point3f(b.pMax.x, b.pMax.y, b.pMin.z)));
		ret = Union(ret, M(Point3f(b.pMax.x, b.pMin.y, b.pMax.z)));
		ret = Union(ret, M(Point3f(b.pMax.x, b.pMax.y, b.pMax.z)));
		return ret;
	}

	Transform Transform::operator*(const Transform& t2) const {
		return Transform(Matrix4x4::Mul(m, t2.m), Matrix4x4::Mul(t2.mInv, mInv));
	}

	bool Transform::SwapsHandedness() const {
		Float det = m.m[0][0] * (m.m[1][1] * m.m[2][2] - m.m[1][2] * m.m[2][1]) -
			m.m[0][1] * (m.m[1][0] * m.m[2][2] - m.m[1][2] * m.m[2][0]) +
			m.m[0][2] * (m.m[1][0] * m.m[2][1] - m.m[1][1] * m.m[2][0]);
		return det < 0;
	}
    inline Ray Transform::operator()(const Ray& r, Vector3f* oError, Vector3f* dError) const
    {
        Point3f o = (*this)(r.o, oError);
        Vector3f d = (*this)(r.d, dError);
        Float tMax = r.tMax;
        Float lengthSquared = d.LengthSquared();
        if (lengthSquared > 0) {
            Float dt = Dot(Abs(d), *oError) / lengthSquared;
            o += d * dt;
            //        tMax -= dt;
        }
        return Ray(o, d, tMax, r.time);
    }
    inline Ray Transform::operator()(const Ray& r, const Vector3f& oErrorIn, const Vector3f& dErrorIn, Vector3f* oErrorOut, Vector3f* dErrorOut) const
    {
        Point3f o = (*this)(r.o, oErrorIn, oErrorOut);
        Vector3f d = (*this)(r.d, dErrorIn, dErrorOut);
        Float tMax = r.tMax;
        Float lengthSquared = d.LengthSquared();
        if (lengthSquared > 0) {
            Float dt = Dot(Abs(d), *oErrorOut) / lengthSquared;
            o += d * dt;
            //        tMax -= dt;
        }
        return Ray(o, d, tMax, r.time);
    }
}
