#pragma once
#include<math/Math3d.hpp>

namespace tutorial::graphics::ptgi
{
	struct SphereMesh
	{
		SphereMesh(math::Vec3f position, float radius, int material) {
			this->position = position;
			this->radius = radius;
			this->material = material;
		}

		math::Vec3f position;
		float radius;
		int material;
	};

	struct QuadMesh
	{
		QuadMesh(math::Vec3f position, math::Vec3f u, math::Vec3f v, int material) {
			this->position = position;
			this->u = u;
			this->v = v;
			this->material = material;
		}

		math::Vec3f position;
		math::Vec3f u;
		math::Vec3f v;
		int material;
	};

	}
