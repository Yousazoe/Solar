#include"Pathtracer.hpp"
#include<../app/raytracing/ScreenRayTracer.hpp>
#include<render/Tonemap.hpp>
#include<core/Random.hpp>
#include"FrameBuffers.hpp"

namespace tutorial::graphics::ptgi
{
	static gfx::UniformHandle _materials = gfx::UniformHash::get("_materials", gfx::UniformType::Srv);
	static gfx::UniformHandle _balls = gfx::UniformHash::get("_balls", gfx::UniformType::Srv);
	static gfx::UniformHandle _quads = gfx::UniformHash::get("_quads", gfx::UniformType::Srv);
	static gfx::UniformHandle _lights = gfx::UniformHash::get("_lights", gfx::UniformType::Srv);
	static gfx::UniformHandle _frame_index = gfx::UniformHash::get("_frame_index", gfx::UniformType::Var);
	static gfx::UniformHandle _seed = gfx::UniformHash::get("_seed", gfx::UniformType::Var);
	static gfx::UniformHandle _nbounce = gfx::UniformHash::get("_nbounce", gfx::UniformType::Var);
	static gfx::UniformHandle _scene = gfx::UniformHash::get("_scene", gfx::UniformType::Var);
	static gfx::UniformHandle t_prev_result = gfx::UniformHash::get("t_prev_result", gfx::UniformType::Srv);


	struct GPUSceneInfo
	{
		uint32 nball;
		uint32 nquad;
		uint32 nlight;
		uint32 pad;
	};

	
	Pathtracer::Pathtracer() noexcept 
	{
	}

	auto Pathtracer::render(Viewport& vp, PTGIScene& scene) -> void
	{
		RenderCamera& camera = vp.camera;

		// use RGBA32 format to avoid light band
		static auto& fb_1024 = ___frame_buffer_HDR2(1024, 768);
		static auto& fb2_1024 = ___frame_buffer_HDR2(1024, 768);
		using FrameBuffer = std::decay_t<decltype(fb_1024)>;
		FrameBuffer* main_fb = &fb_1024;
		FrameBuffer* prev_main_fb = &fb2_1024;
		static Random random;

		Viewport pt_vp{ camera, *main_fb };
		GPUSceneInfo scene_info;
		scene_info.nball = scene._balls.size();
		scene_info.nlight = scene._lights.size();
		scene_info.nquad = scene._quads.size();
		math::Vec2f rand_vec{ random.get(0.0f, 1.0f), random.get(0.0f, 1.0f) };

		GFX_BEGIN;
		gfx::set_global_texture(_materials, scene._material_buffer);
		gfx::set_global_texture(_balls, scene._sphere_buffer);
		gfx::set_global_texture(_quads, scene._quad_buffer);
		gfx::set_global_texture(_lights, scene._light_buffer);
		
		gfx::set_uniform(_frame_index, frame_index);
		gfx::set_uniform(_seed, rand_vec);
		gfx::set_uniform(_nbounce, bounce);
		gfx::set_uniform(_scene, scene_info);
		gfx::set_texture(t_prev_result, prev_main_fb->get_output_rtv<0>());

		ShaderVersion shader_version{ shader };
		ScreenRayTracer::draw(pt_vp, shader_version);

		GFX_END;


		// tonemap
		TonemapPass::draw(vp.target, main_fb->get_output_rtv<0>(), vp.rect);

		swap_frame_buffer(*main_fb, *prev_main_fb);
		++frame_index;
	}
}