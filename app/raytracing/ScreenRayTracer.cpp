#include"ScreenRayTracer.hpp"
#include<core/Random.hpp>

namespace tutorial::graphics
{
	static gfx::UniformHandle _ws_camera_pos;
	static gfx::UniformHandle _camera_to_world;
	static gfx::UniformHandle _project_matrix;
	static gfx::UniformHandle _resolution;
	static gfx::UniformHandle _pt_seed;
	
	struct SetupScreenRayTracer
	{ 
		SetupScreenRayTracer() noexcept
		{
			static bool _init = false;
			if (_init) return;
			_ws_camera_pos = gfx::UniformHash::get("_ws_camera_pos", gfx::UniformType::Var);
			_camera_to_world = gfx::UniformHash::get("_camera_to_world", gfx::UniformType::Var);
			_project_matrix = gfx::UniformHash::get("_project_matrix", gfx::UniformType::Var);
			_resolution = gfx::UniformHash::get("_resolution", gfx::UniformType::Var);
			_pt_seed = gfx::UniformHash::get("_pt_seed", gfx::UniformType::Var);
			_init = true;
		}
	};

	void ScreenRayTracer::draw(Viewport& viewport, ShaderVersion shader) 
	{
		static SetupScreenRayTracer __done;
		const auto& cam_trasform = viewport.camera.transform.world_matrix;
		math::Vec3f world_cam_pos{ cam_trasform[3][0], cam_trasform[3][1], cam_trasform[3][2] };
		std::array resolution = { viewport.rect.width, viewport.rect.height };
		static Random random;
		math::Vec2f rand_vec{ random.get(0.0f, 1.0f), random.get(0.0f, 1.0f) };
		gfx::set_uniform(_resolution, resolution);	
		gfx::set_uniform(_ws_camera_pos, world_cam_pos);
		gfx::set_uniform(_camera_to_world, cam_trasform);
		gfx::set_uniform(_project_matrix, viewport.camera.projection_matrix);
		gfx::set_uniform(_pt_seed, rand_vec);
		Filter::draw(viewport.target, viewport.rect, shader);    
	}
}
