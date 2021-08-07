#pragma once
#include<gfx/Gfx.hpp>

namespace tutorial::graphics
{
	struct Tonemap {
		bool enable = true;
	};

	inline auto _Tonemap() -> decltype(auto)
	{
		static Tonemap data;
		return (data);
	}

	class TonemapPass final
	{
	private:
		TonemapPass() = delete;

	public:
		static auto draw(const gfx::RTV& dst, const gfx::SRV& src, const gfx::Viewport& viewport) -> void;
	};
}
