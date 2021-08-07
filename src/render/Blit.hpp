#pragma once
#include<gfx/Gfx.hpp>
#include<render/Viewport.hpp>

namespace tutorial::graphics
{
	class Blit final
	{
	private:
		Blit() = delete;

	public:
		static auto draw(const gfx::RTV& dst, const gfx::SRV& src, const gfx::Viewport& rect) -> void;
		static auto blit(const Viewport& dst, const gfx::SRV& src) -> void;

		static auto copy_depth(const gfx::RTV& dst, const gfx::SRV& src, const gfx::Viewport& rect) -> void;
		static auto blit_depth(const Viewport& dst, const gfx::SRV& src) -> void;
	};

}
