#pragma once
#include<gfx/entry/Define.hpp>

namespace tutorial::gfx
{
	struct BlendState
	{
		union
		{
			uint32 hash = uint32(-1);
			struct
			{
				uint32 blendEnable : 1;
				uint32 srcBlendFunc : 4;
				uint32 dstBlendFunc : 4;
			};
		};
	};

	struct DepthState
	{
		union
		{
			uint32 hash = uint32(-1);
			struct
			{
				uint32 depthTest : 1;
				uint32 depthWrite : 1;
				uint32 compareFunc : 4;
			};
		};
	};

	struct RasterizeState
	{
		union
		{
			uint32 hash = uint32(-1);
			struct
			{
				uint32 cullmode : 2;
				uint32 fillmode : 2;
				uint32 depthClip : 1;
			};
		};
	};

	template<class GraphicsApi>
	class IGfxStates
	{
	public:
		static auto hash_blend_mode(BlendMode mode);
		static auto hash_depth_test(bool depth_test, bool depth_write, DepthFunc func);
		static auto hash_rasterize_mode(uint32 cull_mode, uint32 fill_mode, bool depth_clip);
	};

	template<class GraphicsApi>
	inline auto IGfxStates<GraphicsApi>::hash_blend_mode(BlendMode mode)
	{
		BlendState newBlend;
		newBlend.hash = 0;
		switch (mode)
		{
		case BlendMode::Replace:
			newBlend.blendEnable = 0;
			break;
		case BlendMode::AlphaBlend:
			newBlend.blendEnable = 1; newBlend.srcBlendFunc = (uint32)BlendFunc::BS_BLEND_SRC_ALPHA;
			newBlend.dstBlendFunc = (uint32)BlendFunc::BS_BLEND_INV_SRC_ALPHA;
			break;
		case BlendMode::Add:
			newBlend.blendEnable = 1; newBlend.srcBlendFunc = (uint32)BlendFunc::BS_BLEND_ONE;
			newBlend.dstBlendFunc = (uint32)BlendFunc::BS_BLEND_ONE;
			break;
		case BlendMode::Mult:
			newBlend.blendEnable = 1; newBlend.srcBlendFunc = (uint32)BlendFunc::BS_BLEND_DEST_COLOR;
			newBlend.dstBlendFunc = (uint32)BlendFunc::BS_BLEND_ZERO;
			break;
		case BlendMode::BlendAdd:
			newBlend.blendEnable = 1; newBlend.srcBlendFunc = (uint32)BlendFunc::BS_BLEND_SRC_ALPHA;
			newBlend.dstBlendFunc = (uint32)BlendFunc::BS_BLEND_ONE;
			break;
		}

		return newBlend;
	}

	template<class GraphicsApi>
	inline auto IGfxStates<GraphicsApi>::hash_depth_test(bool depth_test, bool depth_write, DepthFunc func)
	{
		DepthState newDepth;
		newDepth.depthTest = depth_test;
		newDepth.depthWrite = depth_write;
		newDepth.compareFunc = (uint32)func;
		return newDepth;
	}

	template<class GraphicsApi>
	inline auto IGfxStates<GraphicsApi>::hash_rasterize_mode(uint32 cull_mode, uint32 fill_mode, bool depth_clip)
	{
		RasterizeState newstate;
		newstate.cullmode = cull_mode;
		newstate.fillmode = fill_mode;
		newstate.depthClip = depth_clip;
		return newstate;
	}

}