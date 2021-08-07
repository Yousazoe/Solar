#pragma once
#include<gfx/Gfx.hpp>

namespace tutorial::graphics
{
	using BackBuffer = gfx::FrameBuffer<const gfx::DSV&, const gfx::RTV&>;
	using FrameBuffer = gfx::FrameBuffer<
		gfx::GfxObject<gfx::Texture2D, gfx::DSV>,
		gfx::GfxObject<gfx::Texture2D, gfx::SRV, gfx::RTV>
	>;
	using GBuffer = gfx::FrameBuffer<
		gfx::GfxObject<gfx::Texture2D, gfx::DSV>,
		gfx::GfxObject<gfx::Texture2D, gfx::SRV, gfx::RTV>, // RGBA8
		gfx::GfxObject<gfx::Texture2D, gfx::SRV, gfx::RTV>, // RGBA8
		gfx::GfxObject<gfx::Texture2D, gfx::SRV, gfx::RTV>, // R32F
		gfx::GfxObject<gfx::Texture2D, gfx::SRV, gfx::RTV>  // RGBA8
	>;
	using RenderTexture = gfx::GfxObject<gfx::Texture2D, gfx::SRV, gfx::RTV>;
	using RenderRWTexture = gfx::GfxObject<gfx::Texture2D, gfx::SRV, gfx::RTV, gfx::UAV>;
}
