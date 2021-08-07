#include<render/Filter.hpp>

namespace tutorial::graphics
{
	auto Filter::draw(ShaderVersion shader) -> void
	{
		gfx::bind_shader(shader.fetch());
		gfx::set_primitive_type(gfx::PrimType::PRIM_TRISTRIP);
		gfx::set_blend_mode(gfx::BlendMode::Replace);
		gfx::set_depth_test(false, false, gfx::DepthFunc::DSS_DEPTHFUNC_ALWAYS);
		gfx::set_rasterize_mode(gfx::PolyCullMode::PCM_NONE, gfx::PolyFillMode::PFM_SOLID, true);
		gfx::bind_vertex_buffer(nullptr, nullptr);
		gfx::bind_index_buffer(nullptr);
		gfx::draw(4, 0);
	}

	auto Filter::draw(const gfx::RTV& dst, const gfx::Viewport& rect, ShaderVersion shader) -> void
	{
		gfx::bind_framebuffer(gfx::empty_v<gfx::DSV>, { dst });
		gfx::set_viewport(rect);
		draw(shader);
		gfx::unbind_framebuffer(gfx::empty_v<gfx::DSV>, { dst });
	}

	auto Filter::draw(const Viewport& dst, ShaderVersion shader) -> void
	{
		draw(dst.target, dst.rect, shader);
	}
}