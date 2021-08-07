#pragma once
#include<gfx/Gfx.hpp>
#include<fs/Path.hpp>

namespace tutorial::graphics
{
	class Texture final 
	{
	public:
		Texture() = default;
		Texture(gfx::Texture2D texture, gfx::SRV srv) noexcept;
		~Texture() = default;
		Texture(const Texture&) = delete;
		void operator=(const Texture&) = delete;
		Texture(Texture&& rhs) noexcept;
		void operator=(Texture&& rhs) noexcept;

	public:
		static auto cube_texture(gfx::eFormat format, uint32 dim, uint32 mip_levels) -> Texture;
		auto& get() const { return _texture; }

	private:
		gfx::GfxObject<gfx::Texture2D, gfx::SRV> _texture;

		friend class TextureLoader;
	};

	
	
}

