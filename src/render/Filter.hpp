#pragma once
#include<render/Shader.hpp>
#include<render/Viewport.hpp>
#include<gfx/Gfx.hpp>

namespace tutorial::graphics
{
	class Filter final
	{
	public:
		static auto draw(ShaderVersion shader) -> void;
		static auto draw(const gfx::RTV& dst, const gfx::Viewport& rect, ShaderVersion shader) -> void;
		static auto draw(const Viewport& dst, ShaderVersion shader) -> void;
	};
}
