#include<render/Texture.hpp>

namespace tutorial::graphics
{
	Texture::Texture(gfx::Texture2D texture, gfx::SRV srv) noexcept
	{
		_texture = decltype(_texture) { std::move(texture), std::move(srv) };
	}

	Texture::Texture(Texture&& rhs) noexcept :
		_texture{ std::move(rhs._texture) }
	{
	}

	void Texture::operator=(Texture&& rhs) noexcept
	{
		_texture = std::move(rhs._texture);
	}

	auto Texture::cube_texture(gfx::eFormat format, uint32 dim, uint32 mip_levels) -> Texture
	{
		if (mip_levels == 0u) {
			mip_levels = uint32(std::log2(dim)) + 1u;
		}

		Texture cubemap;
		cubemap._texture = gfx::create_gfx_object(
			gfx::ResourceDesc<gfx::Texture2D> {
				dim,
				dim,
				mip_levels, 6u, format,
				{ 1, 0, },
				gfx::eUsage::DEFAULT,
				gfx::eBindFlag::SHADER_RESOURCE,
				0, 
				gfx::RESOURCE_MISC_FLAG::RESOURCE_MISC_TEXTURECUBE
			},
			gfx::ISRVDesc{ format }
		);


		return std::move(cubemap);
	}
}