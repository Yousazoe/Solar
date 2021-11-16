#pragma once
#include<render/Viewport.hpp>
#include<render/Shader.hpp>
#include"PathtracingScene.hpp"

namespace tutorial::graphics::ptgi
{
	class Pathtracer final
	{
	public:
		Pathtracer() noexcept;

	public:
		auto render(Viewport& vp, PTGIScene& scene) -> void;

	public:
		int bounce = 4;
		uint32 frame_index = 0u;
	private:
		Shader shader{ "pathtracer-refactor/PbrPathTracer" };
	};
}
