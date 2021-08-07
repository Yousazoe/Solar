#include<math/Math3d.hpp>
#include<core/Random.hpp>
#include<random>

namespace tutorial::math
{
	//Vector3f Vector3f::rotate(const Quaternion &q) const
	//{
	//	Quaternion v((*this)[0], (*this)[1], (*this)[2], 0);
	//	v = q*v*q.conjugate();		// positive rotation direction
	//	return Vector3f(v.x(), v.y(), v.z());
	//}

	Vec2f Vec2f::ZERO = Vec2f(0, 0);
	Vec3f Vec3f::ZERO = Vec3f(0, 0, 0);

	void M4f::decompose(Vec3f &pos, Quat &rot, Vec3f &scale) const
	{
		pos.x = m[3][0];
		pos.y = m[3][1];
		pos.z = m[3][2];

		scale.x = sqrtf(m[0][0] * m[0][0] + m[0][1] * m[0][1] + m[0][2] * m[0][2]);
		scale.y = sqrtf(m[1][0] * m[1][0] + m[1][1] * m[1][1] + m[1][2] * m[1][2]);
		scale.z = sqrtf(m[2][0] * m[2][0] + m[2][1] * m[2][1] + m[2][2] * m[2][2]);

		Vec3f invScale(1.0f / scale.x, 1.0f / scale.y, 1.0f / scale.z);

		// S^-1 * SRT = RT
		M4f res = *this;
		res.m[0][0] *= invScale.x;
		res.m[0][1] *= invScale.x;
		res.m[0][2] *= invScale.x;
		res.m[1][0] *= invScale.y;
		res.m[1][1] *= invScale.y;
		res.m[1][2] *= invScale.y;
		res.m[2][0] *= invScale.z;
		res.m[2][1] *= invScale.z;
		res.m[2][2] *= invScale.z;
		res.m[3][3] = 1;
		rot = res.toQuat();
	}

}

namespace tutorial::math
{

	Vec3f CreateRandomConeVector(const Vec3f &direction, float angle)
	{
		static Random random;
		Vec3f axis = Vec3f((float)random.get(-1.f, 1.f), (float)random.get(-1.f, 1.f), (float)random.get(-1.f, 1.f)).normalized();
		Quat quat(axis, Deg2Rad((random.get(0.f, 1.f) - 0.5f) * angle));
		return quat.rotate(direction);
	}
}