#pragma once
#include<math/Math3d.hpp>


namespace tutorial::math
{
	float RayVSBoxDistance(const Ray &r, const BoundingBox &b)
	{
		Vec3f dirfrac;
		// r.dir is unit direction vector of ray
		dirfrac.x = 1.0f / r.dir.x;
		dirfrac.y = 1.0f / r.dir.y;
		dirfrac.z = 1.0f / r.dir.z;
		// lb is the corner of AABB with minimal coordinates - left bottom, rt is maximal corner
		// r.oris origin of ray
		const float t1 = (b._min.x - r.origin.x) * dirfrac.x;
		const float t2 = (b._max.x - r.origin.x) * dirfrac.x;
		const float t3 = (b._min.y - r.origin.y) * dirfrac.y;
		const float t4 = (b._max.y - r.origin.y) * dirfrac.y;
		const float t5 = (b._min.z - r.origin.z) * dirfrac.z;
		const float t6 = (b._max.z - r.origin.z) * dirfrac.z;

		const float tmin = (std::max)((std::max)((std::min)(t1, t2), (std::min)(t3, t4)), (std::min)(t5, t6));
		const float tmax = (std::min)((std::min)((std::max)(t1, t2), (std::max)(t3, t4)), (std::max)(t5, t6));

		float dist = 3.4028235e38;
		// if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
		if (tmax < 0)
		{
			dist = tmax;
			return dist;
		}

		// if tmin > tmax, ray doesn't intersect AABB
		if (tmin > tmax)
		{
			dist = tmax;
			return dist;
		}

		dist = (std::max)(0.f, tmin);
		return dist;
	}

	void RayVSTriangleDistance(
		const Vec3f& v1,
		const Vec3f& v2,
		const Vec3f& v3,
		const Ray& r, float &dist)
	{
		dist = 3.4028235e38;
		Vec3f e1 = v2 - v1;
		Vec3f e2 = v3 - v1;
		Vec3f q = r.dir.cross(e2);
		float a = e1.dot(q); //The determinant of the matrix (-direction e1 e2)
		if (a < 0.0001f)
			return; //avoid determinants close to zero since we will divide by this
		float f = 1.0f / a;
		Vec3f s = r.origin - v1;
		float bu = f * s.dot(q); //barycentric u coordinate
		if (bu < 0.0f)
			return;
		Vec3f rr = s.cross(e1);
		float bv = f * r.dir.dot(rr); //barycentric v coordinate
		if (bv < 0.0f || bu + bv > 1.0f)
			return;
		dist = f * e2.dot(rr);
	}
}