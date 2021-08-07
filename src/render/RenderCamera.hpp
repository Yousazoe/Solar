#pragma once
#include<math/Math3d.hpp>
#include<render/Camera.hpp>
#include<ecs/Transform.hpp>

namespace tutorial::graphics
{
	struct RenderCamera
	{
		explicit RenderCamera(Camera& camera, ecs::Transform& transform);

		Camera& camera;
		ecs::Transform& transform;
		math::M4f view_matrix;
		math::M4f projection_matrix;
		math::M4f view_proj_matrix;
		math::M4f jitter_projection_matrix;
		math::M4f jitter_view_proj_matrix;
	};
}
