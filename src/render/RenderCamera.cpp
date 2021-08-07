#include<render/RenderCamera.hpp>

namespace tutorial::graphics
{
	RenderCamera::RenderCamera(Camera& camera, ecs::Transform& transform) :
		camera{ camera },
		transform{ transform }
	{
		auto& pos = transform.position;
		auto& rot = transform.rotation;
		view_matrix.initView(pos, rot.get_forward_vec(), rot.get_up_vec());
		projection_matrix.initProjection(camera.get_fov(), camera.get_aspect_ratio(), camera.get_znear(), camera.get_zfar());
		view_proj_matrix = view_matrix * projection_matrix;
		jitter_projection_matrix = projection_matrix;
		jitter_view_proj_matrix = view_proj_matrix;
	}

}