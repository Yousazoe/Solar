#pragma once
#include<gfx/entry/Create.hpp>
#include<gfx/entry/Command.hpp>
#include<gfx/entry/Submit.hpp>


namespace tutorial::gfx
{
	auto setup() -> void;
	auto shutdown() -> void;
	
	//void set_swapchain(ISwapChain<D3D>& swapchain);
	//const ISwapChain<D3D>* get_swapchain();

	
	auto next_frame() -> void;
	auto begin() -> bool;
	auto end() -> void;
	auto present(const Swapchain& swapchain) -> void;
	auto submit(/*CommandQueue*/) -> void;

#define GFX_BEGIN if (gfx::begin()) {
#define GFX_END gfx::end(); }
}
