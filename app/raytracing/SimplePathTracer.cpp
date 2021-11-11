// Desc :simple scene path tracer
// lambert diffuse ball only

#include"../Demo.hpp"
#include"FrameBuffers.hpp"
#include"ScreenRayTracer.hpp"
#include<render/Tonemap.hpp>
#include<core/Random.hpp>
using namespace tutorial;
using namespace graphics;

namespace uniforms
{
	static gfx::UniformHandle _frame_index;
	static gfx::UniformHandle _seed;
	static gfx::UniformHandle t_prev_result;
	
	static auto setup() {
		_frame_index = gfx::UniformHash::get("_frame_index", gfx::UniformType::Var);
		_seed = gfx::UniformHash::get("_seed", gfx::UniformType::Var);
		t_prev_result = gfx::UniformHash::get("t_prev_result", gfx::UniformType::Srv);
	}
}


void ___PathTracerSimple(App& app,  bool init)
{
	static Shader shader{ "simple-pt/PathTracerSimple" };
	static auto main_fb = ___frame_buffer_HDR2(1024, 768);
	static auto prev_main_fb = ___frame_buffer_HDR2(1024, 768);
	static Random random;
	static uint32 frame_index = 0u;

	if (init)
	{
		uniforms::setup();
	}

	RenderCamera camera{ app.camera, app.camera_trans };
	GFX_BEGIN;
	gfx::set_uniform(uniforms::_frame_index, frame_index);
	math::Vec2f rand_vec{ random.get(0.0f, 1.0f), random.get(0.0f, 1.0f) };
	gfx::set_uniform(uniforms::_seed, rand_vec);
	gfx::set_texture(uniforms::t_prev_result, prev_main_fb.get_output_rtv<0>());
	Viewport vp1{ camera, main_fb };
	ScreenRayTracer::draw(vp1, shader.default());
	GFX_END;

	// tonemap
	Viewport vp{ camera, app.swapchain.backbuffer() };
	TonemapPass::draw(vp.target, main_fb.get_output_rtv<0>(), vp.rect);

	// swap
	swap_frame_buffer(main_fb, prev_main_fb);
	++frame_index;
}