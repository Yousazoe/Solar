#pragma once

namespace tutorial::gfx
{
	#define GfxRelease(p) if (p) { ((IUnknown*)p)->Release();  p = nullptr; }
}
