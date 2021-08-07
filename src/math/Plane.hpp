#pragma once
#include<math/Math3d.hpp>

namespace tutorial::math
{
	struct Plane final
	{
	public:
		Plane() : d(0.0f) { }

		Plane(const Plane& plane) :
			normal(plane.normal),
			abs_normal(plane.abs_normal),
			d(plane.d)
		{
		}

		// by 3 verts
		Plane(const Vec3f& v0, const Vec3f& v1, const Vec3f& v2)
		{
			init(v0, v1, v2);
		}

		// point on plane and normal
		Plane(const Vec3f& normal, const Vec3f& point)
		{
			init(normal, point);
		}

		Plane(const Vec4f& normald) :
			normal(normald.x, normald.y, normald.z),
			d(normald.w)
		{
			abs_normal = Vec3f(abs(normald.x), abs(normald.y), abs(normald.z));
		}

		Plane& operator =(const Plane& rhs)
		{
			normal = rhs.normal;
			abs_normal = rhs.abs_normal;
			d = rhs.d;
			return *this;
		}

		// by 3 verts
		void init(const Vec3f& v0, const Vec3f& v1, const Vec3f& v2)
		{
			Vec3f dist1 = v1 - v0;
			Vec3f dist2 = v2 - v0;

			init(dist1.cross(dist2), v0);
		}

		void init(const Vec3f& n, const Vec3f& point)
		{
			// ax + by + cz = -d
			normal = n.normalized();
			abs_normal = Vec3f(abs(normal.x), abs(normal.y), abs(normal.z));
			d = -normal.dot(point);
		}

		// 点投影到面
		Vec3f project(const Vec3f& point) const 
		{ 
			return point - normal * (normal.dot(point) + d);
		}

		// 点到面的距离
		float distance(const Vec3f& point) const 
		{ 
			return normal.dot(point) + d;
		}

		Vec3f normal;
		Vec3f abs_normal;
		float d;
	};

}
