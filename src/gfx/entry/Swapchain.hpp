#pragma once
#include<gfx/entry/GfxObject.hpp>

namespace tutorial::gfx
{
	struct Swapchain
	{
		Swapchain() = default;
		Swapchain(const Swapchain&) = delete;
		Swapchain& operator=(const Swapchain&) = delete;
		Swapchain(
			uint32 width, uint32 height, void* hwnd,
			GPUHandle rtv, GPUHandle dsv, GPUHandle swapchain, GPUHandle depthbuffer
		) noexcept;
		Swapchain(Swapchain&& rhs) noexcept;
		Swapchain& operator=(Swapchain&& rhs) noexcept;
		auto backbuffer() const;
	
		uint32 width = 0u;
		uint32 height = 0u;
		void* hwnd = nullptr;
		RTV rtv;
		DSV dsv;
		GPUHandle swapchain = nullptr;
		GPUHandle depthbuffer = nullptr;
	};


	inline Swapchain::Swapchain(
		uint32 width, uint32 height, void* hwnd,
		GPUHandle rtv, GPUHandle dsv, GPUHandle swapchain, GPUHandle depthbuffer
	) noexcept :
		width(width),
		height(std::move(height)),
		hwnd(hwnd),
		swapchain(swapchain),
		depthbuffer(depthbuffer)
	{
		this->rtv.vptr = rtv;
		this->dsv.vptr = dsv;
	}

	inline Swapchain::Swapchain(Swapchain&& rhs) noexcept :
		width(std::move(rhs.width)),
		height(std::move(rhs.height)),
		hwnd((rhs.hwnd)),
		rtv(std::move(rhs.rtv)),
		dsv(std::move(rhs.dsv)),
		swapchain((rhs.swapchain)),
		depthbuffer((rhs.depthbuffer))
	{
		rhs.hwnd = nullptr;
		rhs.depthbuffer = nullptr;
		rhs.depthbuffer = nullptr;
	}

	inline Swapchain& Swapchain::operator=(Swapchain&& rhs) noexcept
	{
		width = rhs.width;
		height = rhs.height;
		hwnd = rhs.hwnd;
		rtv = std::move(rhs.rtv);
		dsv = std::move(rhs.dsv);
		swapchain = rhs.swapchain;
		depthbuffer = rhs.depthbuffer;
		rhs.hwnd = nullptr;
		rhs.depthbuffer = nullptr;
		rhs.depthbuffer = nullptr;
		return *this;
	}

	inline auto Swapchain::backbuffer() const
	{
		return FrameBuffer{ width, height, dsv, rtv };
	}
}
