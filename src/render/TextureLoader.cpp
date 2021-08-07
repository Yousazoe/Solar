#include<render/TextureLoader.hpp>
#include<gfx/entry/TextureLoader.hpp>

namespace tutorial::graphics
{
	auto TextureLoader::load(const fs::Path& path) -> Texture
	{
		Texture texture;
		texture._texture = gfx::TextureLoader::load(path);
		return std::move(texture);
	}
}
