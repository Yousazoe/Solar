
#pragma once

#include<cmath>
#include<cstdio>
#include<cstring>
#include<cassert>
#include<string>
#include<sstream>

namespace tutorial::math
{

	const float eps = 1e-6f;
	const float PI = acosf(-1.f);

	const int M_MIN_INT = 0x80000000;
	const int M_MAX_INT = 0x7fffffff;
	const unsigned M_MIN_UNSIGNED = 0x00000000;
	const unsigned M_MAX_UNSIGNED = 0xffffffff;
	const int M_INF32 = 0x7fffffff;
	const float M_INFINITY = (float)HUGE_VAL;

	const float M_EPSILON = 0.000001f;
	const float M_LARGE_EPSILON = 0.00005f;

	template<class T> inline T Max(T a, T b) { return a > b ? a : b; }
	template<class T> inline T Min(T a, T b) { return a < b ? a : b; }

	inline int dcmp(float x) {
		if (fabs(x) < eps) return 0;
		return x < 0 ? -1 : 1;
	}

	///=======================================
	// @Vec3f
	///=======================================
	class Vec3f
	{

	public:
		// ---------------------
		// constructors
		// ---------------------
		Vec3f() :
			x(0.f), y(0.f), z(0.f)
		{
		}

		template<class T>
		Vec3f(const T *vec)
		{
			for (int i = 0; i < 3; ++i)
				v[i] = vec[i];
		}

		Vec3f(float x, float y, float z) :
			x(x), y(y), z(z)
		{
		}

		Vec3f(const Vec3f &v) :
			x(v.x), y(v.y), z(v.z)
		{
		}
		static Vec3f ZERO;

		// ---------------------
		// comparision
		// ---------------------
		inline bool operator == (const Vec3f &vec) const
		{
			for (unsigned i = 0; i < 3; ++i)
				if (dcmp(v[i] - vec.v[i]) != 0)
					return false;
			return true;
		}

		inline bool operator != (const Vec3f &v) const
		{
			return !(operator==(v));
		}

		// ---------------------
		// Arithmetic operations
		// ---------------------
		Vec3f operator-() const
		{
			return Vec3f(-x, -y, -z);
		}

		Vec3f operator+(const Vec3f &v) const
		{
			return Vec3f(x + v.x, y + v.y, z + v.z);
		}

		Vec3f &operator+=(const Vec3f &v)
		{
			return *this = *this + v;
		}

		Vec3f operator-(const Vec3f &v) const
		{
			return Vec3f(x - v.x, y - v.y, z - v.z);
		}

		Vec3f &operator-=(const Vec3f &v)
		{
			return *this = *this - v;
		}

		Vec3f operator*(const float f) const
		{
			return Vec3f(x * f, y * f, z * f);
		}

		Vec3f &operator*=(const float f)
		{
			return *this = *this * f;
		}

		Vec3f operator/(const float f) const
		{
			return Vec3f(x / f, y / f, z / f);
		}

		Vec3f &operator/=(const float f)
		{
			return *this = *this / f;
		}

		// ---------------------
		// special product
		// ---------------------	
		inline float abs() const
		{
			return sqrt(this->dot(*this));
		}

		inline float absSqr() const
		{
			return this->dot(*this);
		}

		//inline bool isIdentity() const { return dcmp(abs() - 1.0f) == 0; }

		Vec3f normalized() const
		{
			float len = abs();
			if (dcmp(len) == 0) len = eps;
			float invLen = 1.0f / len;
			return Vec3f(x * invLen, y * invLen, z * invLen);
		}

		float dot(const Vec3f &v) const
		{
			return x * v.x + y * v.y + z * v.z;
		}

		Vec3f cross(const Vec3f &v) const
		{
			return Vec3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
		}

		inline Vec3f reflect(const Vec3f &Normal) const
		{
			return *this - Normal * 2 * this->dot(Normal);
		}

		inline Vec3f project(const Vec3f &v) const
		{
			return v.normalized() * (*this).normalized().dot(v);
		}

		inline Vec3f lerp(const Vec3f &dst, float factor) const
		{
			return *this + (dst - *this) * factor;
		}

		std::string toString() const
		{
			std::stringstream outs;
			outs << v[0] << ' ' << v[1] << ' ' << v[2];
			return outs.str();
		}

		/*Vec3f& fromString(std::string &str)
		{
			v[0] = atof(vs[0].c_str());
			v[1] = atof(vs[1].c_str());
			v[2] = atof(vs[2].c_str());
			return *this;
		}*/

		union
		{
			struct { float x, y, z; };
			float v[3];
		};
	};
	typedef Vec3f EulerAngle;

	///=======================================
	// @Vec4f
	///=======================================
	class Vec4f
	{
	public:

		Vec4f() :
			x(0), y(0), z(0), w(0)
		{
		}

		explicit Vec4f(const float x, const float y, const float z, const float w) :
			x(x), y(y), z(z), w(w)
		{
		}

		explicit Vec4f(const Vec3f &v) :
			x(v.x), y(v.y), z(v.z), w(1.0f)
		{
		}

		template<class T>
		Vec4f(const T *vec)
		{
			for (int i = 0; i < 4; ++i)
				v[i] = vec[i];
		}

		Vec4f operator+(const Vec4f &v) const
		{
			return Vec4f(x + v.x, y + v.y, z + v.z, w + v.w);
		}

		Vec4f operator-() const
		{
			return Vec4f(-x, -y, -z, -w);
		}

		Vec4f operator*(float f) const
		{
			return Vec4f(x * f, y * f, z * f, w * f);
		}


		bool operator == (const Vec4f& vec) const
		{
			for (int i = 0; i < 4; ++i)
				if (dcmp(v[i] - vec.v[i]) != 0)
					return false;
			return true;
		}

		bool operator != (const Vec4f& vec) const { return !(operator==(vec)); }

		Vec4f& operator=(const Vec3f &vec)
		{
			x = vec.x;
			y = vec.y;
			z = vec.z;
			w = 0;
			return *this;
		}

		union
		{
			struct { float x, y, z, w; };
			float v[4];
		};
	};



	///=======================================
	// @Vec2f
	///=======================================
	class Vec2f
	{
	public:

		Vec2f() :
			x(0), y(0)
		{
		}

		explicit Vec2f(float x, float y) :
			x(x), y(y)
		{
		}

		static Vec2f ZERO;

		Vec2f& operator = (const Vec2f &v)
		{
			x = v.x;
			y = v.y;
			return *this;
		}

		Vec2f operator-() const
		{
			return Vec2f(-x, -y);
		}

		Vec2f operator*(float f) const
		{
			return Vec2f(x * f, y * f);
		}

		bool operator == (const Vec2f& vec) const
		{
			for (int i = 0; i < 2; ++i)
				if (dcmp(v[i] - vec.v[i]) != 0)
					return false;
			return true;
		}

		bool operator != (const Vec2f& vec) const { return !(operator==(vec)); }

		Vec2f operator-(const Vec2f& v) const
		{
			return Vec2f(x - v.x, y - v.y);
		}

		Vec2f operator+(const Vec2f& v) const
		{
			return Vec2f(x + v.x, y + v.y);
		}

		union
		{
			struct { float x, y; };
			float v[2];
		};
	};


	template<class T>
	struct DefaultVecTrait
	{
		static bool equal(const T& v1, const T& v2)
		{
			return v1 != v2;
		}
	};

	template<
		class T,
		class VecTrait = DefaultVecTrait<T>
	>
	class Vec2
	{
	public:
		Vec2() noexcept : 
			x(0), y(0)
		{
		}

		explicit Vec2(T x, T y) noexcept :
			x(x), y(y)
		{
		}

		static Vec2 ZERO;

		inline Vec2& operator = (const Vec2& v)
		{
			x = v.x;
			y = v.y;
			return *this;
		}

		inline Vec2 operator-() const
		{
			return Vec2(-x, -y);
		}

		/*Vec2 operator*(T f) const
		{
			return Vec2(x * f, y * f);
		}*/

		inline bool operator == (const Vec2& vec) const
		{
			for (int i = 0; i < 2; ++i)
				if (!VecTrait::equal(v[i] != vec.v[i]))
					return false;
			return true;
		}

		inline bool operator != (const Vec2& vec) const { return !(operator==(vec)); }

		inline Vec2 operator-(const Vec2& v) const
		{
			return Vec2(x - v.x, y - v.y);
		}

		inline Vec2 operator+(const Vec2& v) const
		{
			return Vec2(x + v.x, y + v.y);
		}

		union
		{
			struct { T x, y; };
			T v[2];
		};
	};

	using Vec2i = Vec2<int>;
	using Point = Vec2i;

	///=======================================
	// @Quat
	///=======================================
	class Quat
	{
	public:
		// ---------------------
		// constructors
		// ---------------------
		Quat() :
			x(0), y(0), z(0), w(1.f)
		{
		}

		Quat(float x, float y, float z, float w)
		{
			float invlen = sqrt(x*x + y * y + z * z + w * w);
			if (dcmp(invlen) == 0) invlen = eps;
			invlen = 1.f / invlen;
			c[0] = x * invlen;
			c[1] = y * invlen;
			c[2] = z * invlen;
			c[3] = w * invlen;
		}

		Quat(const float *vec)
		{
			for (int i = 0; i < 4; ++i) c[i] = vec[i];
		}

		// Euler(x,y,z)
		Quat(float pitch, float yaw, float roll)
		{
			float ex = pitch * PI / 180.0f * 0.5f;
			float ey = yaw * PI / 180.0f * 0.5f;
			float ez = roll * PI / 180.0f * 0.5f;
			float sinX = sinf(ex);
			float cosX = cosf(ex);
			float sinY = sinf(ey);
			float cosY = cosf(ey);
			float sinZ = sinf(ez);
			float cosZ = cosf(ez);

			w = cosY * cosX * cosZ + sinY * sinX * sinZ;
			x = cosY * sinX * cosZ + sinY * cosX * sinZ;
			y = sinY * cosX * cosZ - cosY * sinX * sinZ;
			z = cosY * cosX * sinZ - sinY * sinX * cosZ;
		}

		Quat(const Vec3f &axis, float rad)
		{
			Vec3f _axis = axis.normalized();			// unit c gets unit quat
			float a = cosf(rad * 0.5f);
			float b = sinf(rad * 0.5f);
			x = b * _axis.x;
			y = b * _axis.y;
			z = b * _axis.z;
			w = a;
		}


		// ---------------------
		// comparision
		// ---------------------
		inline bool operator == (const Quat &q) const
		{
			for (unsigned i = 0; i < 4; ++i)
				if (dcmp(c[i] - q.c[i]) != 0)
					return false;
			return true;
		}

		inline bool operator != (const Quat &q) const
		{
			return !(operator==(q));
		}

		// ---------------------
		// arithmetic operations
		// ---------------------
		inline Quat operator * (const Quat &q) const {
			Quat res;
			res.w = w * q.w - x * q.x - y * q.y - z * q.z;
			res.x = w * q.x + q.w*x + y * q.z - z * q.y;
			res.y = w * q.y + q.w*y + q.x*z - x * q.z;
			res.z = w * q.z + q.w*z + x * q.y - y * q.x;
			return res;
		}

		inline Quat& operator *= (const Quat &q)
		{
			return *this = (*this) * q;
		}

		inline Quat operator + (const Quat &q) const
		{
			return Quat(c[0] + q.c[0], c[1] + q.c[1], c[2] + q.c[2], c[3] + q.c[3]);
		}

		inline Quat& operator += (const Quat &q)
		{
			return *this = (*this) + q;
		}

		inline Quat operator *(float s) const
		{
			return Quat(x * s, y* s, z * s, w * s);
		}


		// ---------------------
		// special
		// ---------------------

		inline Quat normalized() const
		{
			float len = sqrt(c[0] * c[0] + c[1] * c[1] + c[2] * c[2] + c[3] * c[3]);
			if (dcmp(len) == 0) len = eps;
			float invlen = 1.f / len;
			return Quat(c[0] * invlen, c[1] * invlen, c[2] * invlen, c[3] * invlen);
		}

		inline Quat conjugate() const
		{
			return Quat(-x, -y, -z, w);
		}

		inline Quat lerp(const Quat &t, float factor) const
		{
			Quat pend = t;
			float cosom = x * pend.x + y * pend.y + z * pend.z + w * pend.w;

			if (cosom < 0)
			{
				pend.x = -pend.x; pend.y = -pend.y; pend.z = -pend.z; pend.w = -pend.w;
			}

			Quat res(x + factor * (pend.x - x), y + factor * (pend.y - y), z + factor * (pend.z - z), w + factor * (pend.w - w));
			return res;
		}

		Vec3f rotate(const Vec3f &vec) const
		{
			Vec3f result;
			auto &rotation = *this;
			float num12 = rotation.x + rotation.x;
			float num2 = rotation.y + rotation.y;
			float num = rotation.z + rotation.z;
			float num11 = rotation.w * num12;
			float num10 = rotation.w * num2;
			float num9 = rotation.w * num;
			float num8 = rotation.x * num12;
			float num7 = rotation.x * num2;
			float num6 = rotation.x * num;
			float num5 = rotation.y * num2;
			float num4 = rotation.y * num;
			float num3 = rotation.z * num;
			float num15 = ((vec.x * ((1.f - num5) - num3)) + (vec.y * (num7 - num9))) + (vec.z * (num6 + num10));
			float num14 = ((vec.x * (num7 + num9)) + (vec.y * ((1.f - num8) - num3))) + (vec.z * (num4 - num11));
			float num13 = ((vec.x * (num6 - num10)) + (vec.y * (num4 + num11))) + (vec.z * ((1.f - num8) - num5));
			result.x = num15;
			result.y = num14;
			result.z = num13;
			return result;
		}

		EulerAngle to_eular() const
		{
			//from https://www.geometrictools.com/Documentation/EulerAngles.pdf
			// but note that he uses column matrix
			// order in : y, x, z
			float r12 = 2 * (y*z - w * x);
			if (r12 < 1)
			{
				if (r12 > -1)
				{
					return EulerAngle(
						asinf(-r12),
						atan2f(2 * (x*z + w * y), 1 - 2 * (x*x + y * y)),
						atan2f(2 * (x*y + w * z), 1 - 2 * (x*x + z * z)));
				}
				else
				{
					return EulerAngle(
						PI / 2,
						-atan2f(-(2 * (x*y - w * z)), 1.0f - 2 * (y*y + z * z)),
						0);
				}
			}
			else
			{
				return EulerAngle(
					-PI / 2,
					atan2f(-(2 * (x*y - w * z)), 1.0f - 2 * (y*y + z * z)),
					0);
			}
		}

		Vec3f get_right_vec() const
		{
			return this->rotate(Vec3f(1, 0, 0));
		}

		Vec3f get_up_vec() const
		{
			return this->rotate(Vec3f(0, 1, 0));
		}

		Vec3f get_forward_vec() const
		{
			return this->rotate(Vec3f(0, 0, 1));
		}

		std::string toString()
		{
			std::stringstream outs;
			outs << '<';
			for (int i = 0; i < 4; ++i)
				outs << ' ' << c[i];
			outs << " >";

			return outs.str();
		}

		union
		{
			float c[4];
			struct { float x, y, z, w; };
		};
	};

	class M4f
	{
	public:
		// ------------
		// Constructors
		// ------------
		M4f()
		{
			initIdentity();
		}

		M4f(const float * f)
		{
			memcpy(x, f, sizeof(x));
		}

		M4f(const Quat &rotation)
		{
			auto &r = rotation;
			float x = r.x, y = r.y, z = r.z, w = r.w;
			m[0][0] = 1.0f - 2 * (y*y + z * z);  m[0][1] = 2 * (x*y + w * z);     m[0][2] = 2 * (x*z - w * y);       m[0][3] = 0;
			m[1][0] = 2 * (x*y - w * z);				 m[1][1] = 1 - 2 * (x*x + z * z); m[1][2] = 2 * (y*z + w * x);       m[1][3] = 0;
			m[2][0] = 2 * (x*z + w * y);				 m[2][1] = 2 * (y*z - w * x);     m[2][2] = 1 - 2 * (x*x + y * y);   m[2][3] = 0;
			m[3][0] = 0;											 m[3][1] = 0;                   m[3][2] = 0;                     m[3][3] = 1;
		}

		static M4f Zero()
		{
			M4f ret;
			memset(&ret, 0, sizeof(M4f));
			return ret;
		}

		// ------------
		// comparision
		// ------------
		inline bool operator == (const M4f &mat) const
		{
			for (int i = 0; i < 16; ++i)
				if (dcmp(x[i] - mat.x[i]) != 0)
				{
					return false;
				}
			return true;
		}

		inline bool operator != (const M4f &mat) const
		{
			return !(operator==(mat));
		}

		// ---------------------
		// arithmetic operations
		// ---------------------
		inline M4f operator*(const float &c) const
		{
			M4f res;
			for (int i = 0; i < 16; ++i) res.x[i] = x[i] * c;
			return res;
		}

		inline M4f& operator *= (const float &c)
		{
			for (int i = 0; i < 16; ++i) x[i] = x[i] * c;
			return *this;
		}

		inline M4f operator+(const M4f& mat) const
		{
			M4f res;
			for (int i = 0; i < 16; ++i) res.x[i] = x[i] + mat.x[i];
			return res;
		}

		inline M4f& operator+=(const M4f &mat)
		{
			for (int i = 0; i < 16; ++i) x[i] = x[i] + mat.x[i];
			return *this;
		}

		inline M4f operator*(const M4f& mat) const
		{
			/*	unsigned i, j, k;
			M4f res;
			for (i = 0; i < 4; ++i)  {
			for (j = 0; j < 4; ++j)  {
			res.m[i][j] = 0;
			for (k = 0; k < 4; ++k)
			res.m[i][j] += m[i][k] * mat.m[k][j];
			}
			}*/
			unsigned i, j, k;
			M4f res;
			for (i = 0; i < 4; ++i) {
				for (j = 0; j < 4; ++j) {
					res.m[i][j] = 0;
					for (k = 0; k < 4; ++k)
						res.m[i][j] += m[i][k] * mat.m[k][j];
				}
			}
			return res;
		}

		inline M4f& operator *= (const M4f& mat)
		{
			return (*this) = (*this) * mat;
		}

		// ---------------------
		// special
		// ---------------------
		inline M4f& initIdentity()
		{
			memset(m, 0, sizeof(m));
			m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.f;
			return *this;
		}

		inline M4f& set_translation(float x, float y, float z)
		{
			this->initIdentity();
			m[3][0] = x; m[3][1] = y; m[3][2] = z;
			return *this;
		}

		inline M4f& set_translation(const Vec3f &pos)
		{
			return set_translation(pos.x, pos.y, pos.z);
		}

		inline M4f& initScale(float x, float y, float z)
		{
			m[0][0] = x; m[0][1] = 0; m[0][2] = 0; m[0][3] = 0;
			m[1][0] = 0; m[1][1] = y;	m[1][2] = 0; m[1][3] = 0;
			m[2][0] = 0; m[2][1] = 0;	m[2][2] = z; m[2][3] = 0;
			m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
			return *this;
		}

		inline M4f& initScale(const Vec3f &scale)
		{
			return initScale(scale.x, scale.y, scale.z);
		}

		inline M4f& scaled(const Vec3f &scale) const
		{
			M4f res = *this;
			res.m[0][0] *= scale.x;
			res.m[1][0] *= scale.x;
			res.m[2][0] *= scale.x;
			res.m[3][0] *= scale.x;
			res.m[0][1] *= scale.y;
			res.m[1][1] *= scale.y;
			res.m[2][1] *= scale.y;
			res.m[3][1] *= scale.y;
			res.m[0][2] *= scale.z;
			res.m[1][2] *= scale.z;
			res.m[2][2] *= scale.z;
			res.m[3][2] *= scale.z;
			res.m[3][3] = 1;

			return res;
		}

		inline M4f& initView(const Vec3f &pos, const Vec3f & forward, const Vec3f & up)
		{
			Vec3f right = up.cross(forward).normalized();

			m[0][0] = right.x; m[0][1] = up.x;  m[0][2] = forward.x; m[0][3] = 0;
			m[1][0] = right.y; m[1][1] = up.y;	m[1][2] = forward.y; m[1][3] = 0;
			m[2][0] = right.z; m[2][1] = up.z;	m[2][2] = forward.z; m[2][3] = 0;
			m[3][0] = -pos.dot(right); m[3][1] = -pos.dot(up); m[3][2] = -pos.dot(forward);   m[3][3] = 1;

			return *this;
		}

		inline M4f& initProjection(float fovy, float ratio, float zNear, float zFar)
		{
			/*xScale     0          0               0
			0        yScale       0               0
			0          0       zf / (zf - zn)     1
			0          0 - zn*zf / (zf - zn)     0*/

			float cotHalfFOV = 1.0f / tanf(fovy*0.5f);
			float Q = zFar / (zFar - zNear);
			m[0][0] = cotHalfFOV / ratio; m[0][1] = 0;								  m[0][2] = 0;			  m[0][3] = 0;
			m[1][0] = 0;									m[1][1] = cotHalfFOV;					m[1][2] = 0;				m[1][3] = 0;
			m[2][0] = 0;									m[2][1] = 0;									m[2][2] = Q;				m[2][3] = 1;
			m[3][0] = 0;									m[3][1] = 0;									m[3][2] = -zNear * Q; m[3][3] = 0;

			return *this;
		}

		inline M4f& initOrtho(float l, float r, float b, float t, float zn, float zf)
		{
			float d = zf - zn;


			m[0][0] = 2 / (r - l);				 m[0][1] = 0;									m[0][2] = 0;							 m[0][3] = 0;
			m[1][0] = 0;								   m[1][1] = 2 / (t - b);       m[1][2] = 0;							 m[1][3] = 0;
			m[2][0] = 0;									 m[2][1] = 0;									m[2][2] = 1 / (zf - zn);   m[2][3] = 0;
			m[3][0] = (l + r) / (l - r);   m[3][1] = (t + b) / (b - t); m[3][2] = zn / (zn - zf);  m[3][3] = 1;
			return *this;
		}

		void decompose(Vec3f &pos, Quat &rot, Vec3f &scale) const;

		// ----------------------
		// access
		// ----------------------
		inline float* operator[](unsigned int i) { return m[i]; }
		inline const float* operator[](unsigned int i) const { return m[i]; }


		inline Quat toQuat() const
		{
			float mw = m[0][0] + m[1][1] + m[2][2];
			float mx = m[0][0] - m[1][1] - m[2][2];
			float my = m[1][1] - m[0][0] - m[2][2];
			float mz = m[2][2] - m[0][0] - m[1][1];

			float x, y, z, w, s, d;
			//  max one in mx, my, mz, mw
			if (mx < my || mx < mz || mx < mw)
			{
				if (my < mx || my < mz || my < mw)
				{
					if (mz < mx || mz < my || mz < mw)
					{
						s = sqrtf(1.0f + mw)*0.5f;
						d = 0.25f / s;
						w = s;
						x = (m[1][2] - m[2][1])*d;
						y = (m[2][0] - m[0][2])*d;
						z = (m[0][1] - m[1][0])*d;
					}
					else
					{
						s = sqrtf(1.0f + mz)*0.5f;
						d = 0.25f / s;
						z = s;
						w = (m[0][1] - m[1][0])*d;
						x = (m[2][0] + m[0][2])*d;
						y = (m[1][2] + m[2][1])*d;
					}
				}
				else
				{
					s = sqrt(1.0f + my)*0.5f;
					d = 0.25f / s;
					y = s;
					w = (m[2][0] - m[0][2])*d;
					x = (m[0][1] + m[1][0])*d;
					z = (m[1][2] + m[2][1])*d;
				}
			}
			else
			{
				s = sqrt(1.0f + mx) * 0.5f;
				d = 0.25f / s;
				x = s;
				w = (m[1][2] - m[2][1])*d;
				y = (m[0][1] + m[1][0])*d;
				z = (m[2][0] + m[0][2])*d;
			}
			return Quat(x, y, z, w);
		}

		static M4f fastMult(const M4f &m1, const M4f &m2)
		{
			M4f res;
			res.x[0] = m1.x[0] * m2.x[0] + m1.x[1] * m2.x[4] + m1.x[2] * m2.x[8];
			res.x[1] = m1.x[0] * m2.x[1] + m1.x[1] * m2.x[5] + m1.x[2] * m2.x[9];
			res.x[2] = m1.x[0] * m2.x[2] + m1.x[1] * m2.x[6] + m1.x[2] * m2.x[10];
			res.x[3] = 0;

			res.x[4] = m1.x[4] * m2.x[0] + m1.x[5] * m2.x[4] + m1.x[6] * m2.x[8];
			res.x[5] = m1.x[4] * m2.x[1] + m1.x[5] * m2.x[5] + m1.x[6] * m2.x[9];
			res.x[6] = m1.x[4] * m2.x[2] + m1.x[5] * m2.x[6] + m1.x[6] * m2.x[10];
			res.x[7] = 0;

			res.x[8] = m1.x[8] * m2.x[0] + m1.x[9] * m2.x[4] + m1.x[10] * m2.x[8];
			res.x[9] = m1.x[8] * m2.x[1] + m1.x[9] * m2.x[5] + m1.x[10] * m2.x[9];
			res.x[10] = m1.x[8] * m2.x[2] + m1.x[9] * m2.x[6] + m1.x[10] * m2.x[10];
			res.x[11] = 0;

			res.x[12] = m1.x[12] * m2.x[0] + m1.x[13] * m2.x[4] + m1.x[14] * m2.x[8] + m2.x[12];
			res.x[13] = m1.x[12] * m2.x[1] + m1.x[13] * m2.x[5] + m1.x[14] * m2.x[9] + m2.x[13];
			res.x[14] = m1.x[12] * m2.x[2] + m1.x[13] * m2.x[6] + m1.x[14] * m2.x[10] + m2.x[14];
			res.x[15] = 1;
			return res;
		}

		inline M4f transpose() const
		{
			M4f res;
			for (unsigned i = 0; i < 4; ++i)
				for (unsigned j = 0; j < 4; ++j)
					res.m[i][j] = m[j][i];
			return res;
		}

		M4f inverse() const
		{
			float v0 = m[0][2] * m[1][3] - m[1][2] * m[0][3];
			float v1 = m[0][2] * m[2][3] - m[2][2] * m[0][3];
			float v2 = m[0][2] * m[3][3] - m[3][2] * m[0][3];
			float v3 = m[1][2] * m[2][3] - m[2][2] * m[1][3];
			float v4 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
			float v5 = m[2][2] * m[3][3] - m[3][2] * m[2][3];

			float i00 = (v5 * m[1][1] - v4 * m[2][1] + v3 * m[3][1]);
			float i10 = -(v5 * m[0][1] - v2 * m[2][1] + v1 * m[3][1]);
			float i20 = (v4 * m[0][1] - v2 * m[1][1] + v0 * m[3][1]);
			float i30 = -(v3 * m[0][1] - v1 * m[1][1] + v0 * m[2][1]);

			float invDet = 1.0f / (i00 * m[0][0] + i10 * m[1][0] + i20 * m[2][0] + i30 * m[3][0]);

			i00 *= invDet;
			i10 *= invDet;
			i20 *= invDet;
			i30 *= invDet;

			float i01 = -(v5 * m[1][0] - v4 * m[2][0] + v3 * m[3][0]) * invDet;
			float i11 = (v5 * m[0][0] - v2 * m[2][0] + v1 * m[3][0]) * invDet;
			float i21 = -(v4 * m[0][0] - v2 * m[1][0] + v0 * m[3][0]) * invDet;
			float i31 = (v3 * m[0][0] - v1 * m[1][0] + v0 * m[2][0]) * invDet;

			v0 = m[0][1] * m[1][3] - m[1][1] * m[0][3];
			v1 = m[0][1] * m[2][3] - m[2][1] * m[0][3];
			v2 = m[0][1] * m[3][3] - m[3][1] * m[0][3];
			v3 = m[1][1] * m[2][3] - m[2][1] * m[1][3];
			v4 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
			v5 = m[2][1] * m[3][3] - m[3][1] * m[2][3];

			float i02 = (v5 * m[1][0] - v4 * m[2][0] + v3 * m[3][0]) * invDet;
			float i12 = -(v5 * m[0][0] - v2 * m[2][0] + v1 * m[3][0]) * invDet;
			float i22 = (v4 * m[0][0] - v2 * m[1][0] + v0 * m[3][0]) * invDet;
			float i32 = -(v3 * m[0][0] - v1 * m[1][0] + v0 * m[2][0]) * invDet;

			v0 = m[1][2] * m[0][1] - m[0][2] * m[1][1];
			v1 = m[2][2] * m[0][1] - m[0][2] * m[2][1];
			v2 = m[3][2] * m[0][1] - m[0][2] * m[3][1];
			v3 = m[2][2] * m[1][1] - m[1][2] * m[2][1];
			v4 = m[3][2] * m[1][1] - m[1][2] * m[3][1];
			v5 = m[3][2] * m[2][1] - m[2][2] * m[3][1];

			float i03 = -(v5 * m[1][0] - v4 * m[2][0] + v3 * m[3][0]) * invDet;
			float i13 = (v5 * m[0][0] - v2 * m[2][0] + v1 * m[3][0]) * invDet;
			float i23 = -(v4 * m[0][0] - v2 * m[1][0] + v0 * m[3][0]) * invDet;
			float i33 = (v3 * m[0][0] - v1 * m[1][0] + v0 * m[2][0]) * invDet;

			M4f r;
			r[0][0] = i00;
			r[0][1] = i10;
			r[0][2] = i20;
			r[0][3] = i30;

			r[1][0] = i01;
			r[1][1] = i11;
			r[1][2] = i21;
			r[1][3] = i31;

			r[2][0] = i02;
			r[2][1] = i12;
			r[2][2] = i22;
			r[2][3] = i32;

			r[3][0] = i03;
			r[3][1] = i13;
			r[3][2] = i23;
			r[3][3] = i33;

			return r;
		}

		std::string toString() const
		{
			std::stringstream outs;
			for (int i = 0; i < 4; ++i)
			{
				outs << "( ";
				for (int j = 0; j < 4; ++j)
					outs << m[i][j] << "  ";
				outs << " ) ";
			}
			return outs.str();
		}

		union
		{
			float m[4][4];
			float x[16];
		};
	};

	inline Vec3f transform_vertex(const Vec3f& v, const M4f &m)
	{
		float a[3] = { v.x, v.y, v.z };
		float b[3];

		for (int i = 0; i < 3; ++i)
		{
			b[i] = m[3][i];
			for (int j = 0; j < 3; ++j)
				b[i] += a[j] * m[j][i];
		}

		return Vec3f(b);
	}

	inline Vec4f transform_vec4(const Vec4f& v, const M4f &m)
	{
		float a[4] = { v.x, v.y, v.z, v.w };
		float b[4];

		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
				b[i] += a[j] * m[j][i];
		}

		return Vec4f(b);
	}

	inline Vec3f transform_vec3(const Vec3f& v, const M4f &m)
	{
		float a[3] = { v.x, v.y, v.z };
		float b[3];

		for (int i = 0; i < 3; ++i)
		{
			b[i] = 0;
			for (int j = 0; j < 3; ++j)
				b[i] += a[j] * m[j][i];
		}

		return Vec3f(b);
	}

	inline M4f initPlaneReflectionMat(const Vec3f& n)
	{
		M4f res;
		float xy2 = 2 * n.x * n.y;
		float xz2 = 2 * n.x * n.z;
		float yz2 = 2 * n.y * n.z;
		float xx2 = 2 * n.x * n.x;
		float yy2 = 2 * n.y * n.y;
		float zz2 = 2 * n.z * n.z;
		res.x[0] = 1 - xx2;
		res.x[1] = -xy2;
		res.x[2] = -xz2;
		res.x[3] = 0;

		res.x[4] = -xy2;
		res.x[5] = 1 - yy2;
		res.x[6] = -yz2;
		res.x[7] = 0;

		res.x[8] = -xz2;
		res.x[9] = -yz2;
		res.x[10] = 1 - zz2;
		res.x[11] = 0;

		res.x[12] = 0;
		res.x[13] = 0;
		res.x[14] = 0;
		res.x[15] = 1;
		return res;
	}



	template<class T>
	T clamp(const T& v, const T& a, const T& b)
	{
		if (v > b) return b;
		else if (v < a) return a;
		return v;
	}

	template <typename T> T signum(T a)
	{
		return a > 0 ? (T)1 : (a < 0 ? (T)-1 : 0);
	}

	inline int RandomRangeI(int a, int b)
	{
		return a + rand() % (b - a + 1);
	}

}

namespace tutorial::math
{
	inline Quat FromToRotation(const Vec3f &From, const Vec3f &To)
	{
		Vec3f from = From.normalized();
		Vec3f to = To.normalized();
		Vec3f up = from.cross(to);
		float radian = acosf(from.dot(to));
		return Quat(up, radian);
	}

	Vec3f CreateRandomConeVector(const Vec3f &direction, float angle);

	inline float Deg2Rad(float ang) { return ang * PI / 180.0f; }
	inline float Rad2Deg(float rad) { return rad * 180.0f / PI; }
	inline Vec3f Rad2Deg(const Vec3f& rad) { return Vec3f{ Rad2Deg(rad.x), Rad2Deg(rad.y), Rad2Deg(rad.z) }; }
}
