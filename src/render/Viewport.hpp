#pragma once
#include<gfx/Gfx.hpp>
#include<render/FrameBuffer.hpp>
#include<render/RenderCamera.hpp>

namespace tutorial::graphics
{
	struct Viewport
	{
		Viewport(RenderCamera camera, BackBuffer& backbuffer) noexcept : camera{ camera }, target{ backbuffer.get_output_rtv<0>() }, rect{ 0, 0, backbuffer.width, backbuffer.height } { }
		Viewport(RenderCamera camera, FrameBuffer& fb) noexcept : camera{ camera }, target{ fb.get_output_rtv<0>() }, rect{ 0, 0, fb.width, fb.height } { }

		RenderCamera camera;
		const gfx::RTV& target;
		gfx::Viewport rect;
	};
}
