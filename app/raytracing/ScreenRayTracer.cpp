#include"ScreenRayTracer.hpp"
#include<core/Random.hpp>

namespace tutorial::graphics
{
	struct SetupScreenRayTracer
	{ 
		SetupScreenRayTracer() noexcept
		{
			static bool _init = false;
			if (_init) return;
				_init = true;
		}
	};

	void ScreenRayTracer::draw(Viewport& viewport, ShaderVersion shader) 
	{
		static SetupScreenRayTracer __done;
		Filter::draw(viewport.target, viewport.rect, shader);    
	}
}
