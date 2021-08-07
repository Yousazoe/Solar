#pragma once
#include<render/Texture.hpp>

namespace tutorial::graphics
{
	class TextureLoader final
	{
		TextureLoader() = delete;
	public:
		static auto load(const fs::Path& path) -> Texture;
	};
}
