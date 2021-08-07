#pragma once
#include<math/Math3d.hpp>
#include<render/Filter.hpp>
#include<render/Viewport.hpp>

namespace tutorial::graphics
{
	class ScreenRayTracer 
	{
	private:
		ScreenRayTracer() = delete;

	public:
		static void draw(Viewport& viewport, ShaderVersion shader);
	};

	
}
