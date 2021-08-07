#pragma once
#include<math/Math3d.hpp>

namespace tutorial::math
{
	struct BoundingBox
	{
		bool operator == (const BoundingBox& bound) const;
		void clear();
		void transform(const M4f& m);
		Vec3f center() const;
		Vec3f size() const;
		bool is_inside(const Vec3f& point) const;

		Vec3f min;
		Vec3f max;
	};

	inline bool BoundingBox::operator == (
		const BoundingBox &bound
	) const
	{
		return min == bound.min && max == bound.max;
	}

	inline void BoundingBox::clear()
	{
		min = Vec3f(0, 0, 0);
		max = Vec3f(0, 0, 0);
	}

	inline void BoundingBox::transform(const M4f &m)
	{
		// Efficientive transforming an AABB, from Graphics Gems

		float minA[3] = { min.x, min.y, min.z }, minB[3];
		float maxA[3] = { max.x, max.y, max.z }, maxB[3];

		for (auto i = 0; i < 3; ++i)
		{
			minB[i] = m[3][i];
			maxB[i] = m[3][i];

			for (auto j = 0; j < 3; ++j)
			{
				float x = minA[j] * m[j][i];
				float y = maxA[j] * m[j][i];
				if (x > y) std::swap(x, y);
				minB[i] += x;
				maxB[i] += y;
			}
		}

		min = Vec3f(minB[0], minB[1], minB[2]);
		max = Vec3f(maxB[0], maxB[1], maxB[2]);
	}

	inline Vec3f BoundingBox::center() const 
	{ 
		return (min + max) * 0.5f;
	}

	inline Vec3f BoundingBox::size() const
	{
		return max - min; 
	}

	inline bool BoundingBox::is_inside(const Vec3f& point) const
	{
		if (point.x < min.x ||
			point.x > max.x ||
			point.y < min.y ||
			point.y > max.y ||
			point.z < min.z ||
			point.z > max.z
			)
			return false;
		else
			return true;
	}

}
