#pragma once
#include<gfx/entry/GfxObject.hpp>
#include<fs/Path.hpp>

namespace tutorial::gfx
{
	class TextureLoader final
	{
		TextureLoader() = delete;
	public:
		static auto load(const fs::Path& path) -> GfxObject<Texture2D, SRV>;
	};
}
