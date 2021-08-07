#include<render/Tonemap.hpp>
#include<render/Filter.hpp>

namespace tutorial::graphics
{
	auto TonemapPass::draw(const gfx::RTV& dst, const gfx::SRV& src, const gfx::Viewport& viewport) -> void
	{
		static auto _shader = graphics::Shader{ "tonemap/tonemap" };
		static auto _main_tex_handle = gfx::UniformHash::get("_main_tex", gfx::UniformType::Srv);
		gfx::FrameBuffer fb { 0, 0, gfx::empty_v<gfx::DSV>,  dst  };
		gfx::bind_framebuffer(fb);
		gfx::set_viewport(viewport);
		gfx::begin();
		gfx::set_texture(_main_tex_handle, src);
		Filter::draw(_shader.default());
		gfx::end();
		gfx::unbind_framebuffer(fb);
	}
}