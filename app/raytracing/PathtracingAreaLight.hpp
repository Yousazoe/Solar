#pragma once
#include<math/Math3d.hpp>

namespace tutorial::graphics::ptgi
{
	enum LightType : int
	{
		QuadLight = 0,
		SphereLight = 1,
		Count,
	};
	
	struct Light
	{
		math::Vec3f position;
		float radius = 1;
		math::Vec3f energy;
		float type; 
		math::Vec3f u;
		float area;
		math::Vec3f v;
	};

	inline Light MakeQuadLight(math::Vec3f position, math::Vec3f energy, math::Vec3f u, math::Vec3f v) noexcept
	{
		Light light;
		light.position = position;
		light.energy = energy;
		light.u = u;
		light.v = v;
		light.type = LightType::QuadLight;
		light.area = u.cross(v).abs();
		return light;
	}

}
