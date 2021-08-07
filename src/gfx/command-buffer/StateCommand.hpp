#pragma once
#include<core/Types.hpp>
#include<../app/graphics-pro/mt/middle/IGfxStates.hpp>
#include<../app/graphics-pro/mt/StateEncode.hpp>

namespace disk::gfx
{
	struct SetStateCommand
	{
		void set_blend_mode(gfx::BlendMode mode);
		void set_depth_test(bool depth_test, bool depth_write, gfx::DepthFunc func);
		void set_rasterize_mode(gfx::PolyCullMode cull_mode, gfx::PolyFillMode fill_mode, bool depth_clip = true);
		void set_shader(uint32 shader);
		auto get_key() const { return state.hash; }

		detail::StateHash state;
	};

	inline void SetStateCommand::set_blend_mode(gfx::BlendMode mode)
	{
		state.blend = (uint32)mode;
	}

	inline void SetStateCommand::set_depth_test(bool depth_test, bool depth_write, gfx::DepthFunc func)
	{
		state.depth_test = depth_test;
		state.depth_write = depth_write;
		state.depth_compare_func = (uint32)func;
	}

	inline void SetStateCommand::set_rasterize_mode(gfx::PolyCullMode cull_mode, gfx::PolyFillMode fill_mode, bool depth_clip)
	{
		state.cull_mode = (uint32)cull_mode;
		state.fill_mode = (uint32)fill_mode;
		state.depth_clip = depth_clip;
	}

	inline void SetStateCommand::set_shader(uint32 shader)
	{
		state.shader_id = shader;
	}


}
