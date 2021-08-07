#include<render/Blit.hpp>
#include<render/Filter.hpp>

namespace tutorial::graphics
{
	auto Blit::draw(const gfx::RTV& dst, const gfx::SRV& src, const gfx::Viewport& rect) -> void
	{
		static auto _shader = graphics::Shader{ "blit/Blit" };
		static auto _main_tex_handle = gfx::UniformHash::get("_main_tex", gfx::UniformType::Srv);
		gfx::FrameBuffer fb{ 0, 0, gfx::empty_v<gfx::DSV>,  dst  };
		gfx::bind_framebuffer(fb);
		gfx::set_viewport(rect);
		GFX_BEGIN;
		gfx::set_texture(_main_tex_handle, src);
		Filter::draw(_shader.default());
		GFX_END;
		gfx::unbind_framebuffer(fb);
	}

	auto Blit::blit(const Viewport& dst, const gfx::SRV& src) -> void
	{
		draw(dst.target, src, dst.rect);
	}

	auto Blit::copy_depth(const gfx::RTV& dst, const gfx::SRV& src, const gfx::Viewport& rect) -> void
	{
		static auto _shader = graphics::Shader{ "blit/CopyDepth" };
		static auto _main_tex_handle = gfx::UniformHash::get("_main_tex", gfx::UniformType::Srv);
		gfx::bind_framebuffer(gfx::empty_v<gfx::DSV>, { dst });
		gfx::set_viewport(rect);
		GFX_BEGIN;
		gfx::set_texture(_main_tex_handle, src);
		Filter::draw(_shader.default());
		GFX_END;
	}

	auto Blit::blit_depth(const Viewport& dst, const gfx::SRV& src) -> void
	{
		copy_depth(dst.target, src, dst.rect);
	}
}