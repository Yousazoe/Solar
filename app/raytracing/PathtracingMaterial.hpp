#pragma once
#include<math/Math3d.hpp>

namespace tutorial::graphics::ptgi
{
	struct Material
	{
		math::Vec3f baseColor{ 1.0f, 1.0f, 1.0f };
	};

	auto disney_material_gui(Material& material) -> bool;
}
