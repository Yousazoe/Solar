#pragma once
//#include<gfx/IGfxDevice.hpp>
//#include<gfx/IGfxAllocator.hpp>
//#include<gfx/IGfxStates.hpp>

#include<gfx/d3d11/GfxDeviceDx11.hpp>
#include<gfx/d3d11/GfxAllocatorDx11.hpp>
#include<gfx/d3d11/GfxStatesDx11.hpp>

namespace tutorial::gfx
{
//#ifdef D3D
	using GraphicsApi = D3D;
//#endif
	using GfxDevice = IGfxDevice<D3D>;
	using GfxAllocator = IGfxAllocator<D3D>;
	using GfxStates = IGfxStates<D3D>;
}
