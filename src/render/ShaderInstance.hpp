#pragma once
#include<gfx/Gfx.hpp>
#include<vector>

namespace tutorial::graphics
{
	class ShaderInstance final
	{
	public:
		ShaderInstance() = default;
		ShaderInstance(gfx::Shader vs, gfx::Shader gs, gfx::Shader ps, gfx::ShaderBinding binding) noexcept :
			vs{ std::move(vs) }, gs{ std::move(gs) }, ps{ std::move(ps) }, binding{ std::move(binding) } { }
		ShaderInstance(gfx::Shader cs, gfx::ShaderBinding binding) noexcept :
			cs{ std::move(cs) }, binding{ std::move(binding) } { }
		ShaderInstance(const ShaderInstance&) = delete;
		void operator=(const ShaderInstance&) = delete;
		ShaderInstance(ShaderInstance&&) = default;
		void operator=(ShaderInstance&& rhs) noexcept {
			vs = std::move(rhs.vs);
			gs = std::move(rhs.gs);
			ps = std::move(rhs.ps);
			cs = std::move(rhs.cs);
			binding = std::move(rhs.binding);
		}

	public:
		gfx::Shader vs;
		gfx::Shader gs;
		gfx::Shader ps;
		gfx::Shader cs;
		gfx::ShaderBinding binding;
	};
}

namespace tutorial::gfx
{
	inline void bind_shader(const graphics::ShaderInstance& shader)
	{
		gfx::bind_shader(shader.vs, shader.gs, shader.ps, shader.binding);
	}
}
