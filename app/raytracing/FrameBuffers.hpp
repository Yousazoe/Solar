#pragma once
#include<render/Viewport.hpp>

namespace tutorial::graphics
{
	template<class T1, class T2>
	inline auto swap_frame_buffer(T1& a, T2& b)
	{
		auto tmp = std::move(a);
		a = std::move(b);
		b = std::move(tmp);
	}

	inline auto ___frame_buffer_HDR(uint32 dim, uint32 mip_levels = 1u) 
	{
		if (mip_levels == 0)
		{
			mip_levels = uint32(std::log2(dim)) + 1;
		}

		gfx::FrameBuffer<
			gfx::GfxObject<gfx::Texture2D, gfx::DSV>,
			gfx::GfxObject<gfx::Texture2D, gfx::SRV, gfx::RTV>
		> fb =
		{
			dim,
			dim,
			gfx::create_gfx_object(
				gfx::ResourceDesc<gfx::Texture2D> {
					dim,
					dim,
					1, 1,
					gfx::eFormat::R24G8_TYPELESS,
					{ 1, 0, },
					gfx::eUsage::DEFAULT,
					gfx::eBindFlag::SHADER_RESOURCE |
					gfx::eBindFlag::DEPTH_STENCIL, 0
				},
				gfx::IDSVDesc{ gfx::eFormat::D24_UNORM_S8_UINT }
			),
			gfx::create_gfx_object(
				gfx::ResourceDesc<gfx::Texture2D> {
					dim,
					dim,
					mip_levels, 1, gfx::eFormat::R32G32B32A32_FLOAT,
					{ 1, 0, },
					gfx::eUsage::DEFAULT,
					gfx::eBindFlag::SHADER_RESOURCE |
					gfx::eBindFlag::RENDER_TARGET,
					0
				},
				gfx::ISRVDesc{ gfx::eFormat::R32G32B32A32_FLOAT },
				gfx::IRTVDesc{ gfx::eFormat::R32G32B32A32_FLOAT }
			)
		};

		return std::move(fb);
	}

	inline auto ___frame_buffer_HDR2(uint32 dimx, uint32 dimy, uint32 mip_levels = 1u) 
	{
		if (mip_levels == 0)
		{
			mip_levels = uint32(std::log2(dimx)) + 1;
		}

		gfx::FrameBuffer<
			gfx::GfxObject<gfx::Texture2D, gfx::DSV>,
			gfx::GfxObject<gfx::Texture2D, gfx::SRV, gfx::RTV>
		> fb =
		{
			dimx,
			dimy,
			gfx::create_gfx_object(
				gfx::ResourceDesc<gfx::Texture2D> {
					dimx,
					dimy,
					1, 1,
					gfx::eFormat::R24G8_TYPELESS,
					{ 1, 0, },
					gfx::eUsage::DEFAULT,
					gfx::eBindFlag::SHADER_RESOURCE |
					gfx::eBindFlag::DEPTH_STENCIL, 0
				},
				gfx::IDSVDesc{ gfx::eFormat::D24_UNORM_S8_UINT }
			),
			gfx::create_gfx_object(
				gfx::ResourceDesc<gfx::Texture2D> {
					dimx,
					dimy,
					mip_levels, 1, gfx::eFormat::R32G32B32A32_FLOAT,
					{ 1, 0, },
					gfx::eUsage::DEFAULT,
					gfx::eBindFlag::SHADER_RESOURCE |
					gfx::eBindFlag::RENDER_TARGET,
					0
				},
				gfx::ISRVDesc{ gfx::eFormat::R32G32B32A32_FLOAT },
				gfx::IRTVDesc{ gfx::eFormat::R32G32B32A32_FLOAT }
			)
		};

		return std::move(fb);
	}

	inline auto ___frame_buffer(uint32 dim)
	{
		gfx::FrameBuffer<
			gfx::GfxObject<gfx::Texture2D, gfx::DSV>,
			gfx::GfxObject<gfx::Texture2D, gfx::SRV, gfx::RTV>
		> fb =
		{
			dim,
			dim,
			gfx::create_gfx_object(
				gfx::ResourceDesc<gfx::Texture2D> {
					dim,
					dim,
					1, 1,
					gfx::eFormat::R24G8_TYPELESS,
					{ 1, 0, },
					gfx::eUsage::DEFAULT,
					gfx::eBindFlag::SHADER_RESOURCE |
					gfx::eBindFlag::DEPTH_STENCIL, 0
				},
				gfx::IDSVDesc{ gfx::eFormat::D24_UNORM_S8_UINT }
			),
			gfx::create_gfx_object(
				gfx::ResourceDesc<gfx::Texture2D> {
					dim,
					dim,
					1, 1, gfx::eFormat::R8G8B8A8_UNORM,
					{ 1, 0, },
					gfx::eUsage::DEFAULT,
					gfx::eBindFlag::SHADER_RESOURCE |
					gfx::eBindFlag::RENDER_TARGET,
					0
				},
				gfx::ISRVDesc{ gfx::eFormat::R8G8B8A8_UNORM },
				gfx::IRTVDesc{ gfx::eFormat::R8G8B8A8_UNORM }
			)
		};

		return std::move(fb);
	}

	inline auto ___frame_buffer(gfx::eFormat format, uint32 dim, uint32 mip_levels)
	{
		if (mip_levels == 0u) {
			mip_levels = uint32(std::log2(dim)) + 1u;
		}

		gfx::FrameBuffer<
			gfx::GfxObject<gfx::Texture2D, gfx::DSV>,
			gfx::GfxObject<gfx::Texture2D, gfx::SRV, gfx::RTV>
		> fb =
		{
			dim,
			dim,
			gfx::create_gfx_object(
				gfx::ResourceDesc<gfx::Texture2D> {
					dim,
					dim,
					1, 1,
					gfx::eFormat::R24G8_TYPELESS,
					{ 1, 0, },
					gfx::eUsage::DEFAULT,
					gfx::eBindFlag::SHADER_RESOURCE |
					gfx::eBindFlag::DEPTH_STENCIL, 0
				},
				gfx::IDSVDesc{ gfx::eFormat::D24_UNORM_S8_UINT }
			),
			gfx::create_gfx_object(
				gfx::ResourceDesc<gfx::Texture2D> {
					dim,
					dim,
					mip_levels, 1, format,
					{ 1, 0, },
					gfx::eUsage::DEFAULT,
					gfx::eBindFlag::SHADER_RESOURCE | gfx::eBindFlag::RENDER_TARGET,
					0
				},
				gfx::ISRVDesc{ format },
				gfx::IRTVDesc{ format }
			)
		};
		return std::move(fb);
	}

	inline auto material_buffer(uint32 mat_size, uint32 count) -> decltype(auto)
	{
		static gfx::GfxObject<gfx::StructureBuffer, gfx::SRV> mat_buf = 
			gfx::create_gfx_object(
				gfx::ResourceDesc<gfx::StructureBuffer> {  
					count * mat_size,
					gfx::eUsage::DEFAULT, 
					gfx::eBindFlag::SHADER_RESOURCE, 0, 
					gfx::RESOURCE_MISC_FLAG::RESOURCE_MISC_BUFFER_STRUCTURED, 
					mat_size
				},
				gfx::ISRVDesc{ gfx::eFormat::UNKNOWN }
			);
		return (mat_buf);
	}

	inline auto ___struct_buffer(uint32 struct_size, uint32 count) -> decltype(auto)
	{
		 gfx::GfxObject<gfx::StructureBuffer, gfx::SRV> mat_buf =
			gfx::create_gfx_object(
				gfx::ResourceDesc<gfx::StructureBuffer> {
				count* struct_size,
				gfx::eUsage::DEFAULT,
				gfx::eBindFlag::SHADER_RESOURCE, 0,
				gfx::RESOURCE_MISC_FLAG::RESOURCE_MISC_BUFFER_STRUCTURED,
				struct_size
		},
				gfx::ISRVDesc{ gfx::eFormat::UNKNOWN }
			);
		return std::move(mat_buf);
	}

	inline auto ___gbuffer(uint32 dim) 
	{
		GBuffer gbuffer
		{
			dim, dim,
			gfx::create_gfx_object(
			gfx::ResourceDesc<gfx::Texture2D> {
				dim,dim,
				1, 1,
				gfx::eFormat::R24G8_TYPELESS,
				{ 1, 0, },
				gfx::eUsage::DEFAULT,
				gfx::eBindFlag::SHADER_RESOURCE |
				gfx::eBindFlag::DEPTH_STENCIL, 0
			},
			gfx::IDSVDesc{ gfx::eFormat::D24_UNORM_S8_UINT }
			),

		 gfx::create_gfx_object(
				gfx::ResourceDesc<gfx::Texture2D> {
					dim,dim,
					1, 1, gfx::eFormat::R8G8B8A8_UNORM,
					{ 1, 0, },
					gfx::eUsage::DEFAULT,
					gfx::eBindFlag::SHADER_RESOURCE |
					gfx::eBindFlag::RENDER_TARGET,
					0
				},
				gfx::ISRVDesc{ gfx::eFormat::R8G8B8A8_UNORM },
				gfx::IRTVDesc{ gfx::eFormat::R8G8B8A8_UNORM }
			)
			,

			gfx::create_gfx_object(
				gfx::ResourceDesc<gfx::Texture2D> {
					dim,dim,
					1, 1, gfx::eFormat::R16G16B16A16_FLOAT,
					{ 1, 0, },
					gfx::eUsage::DEFAULT,
					gfx::eBindFlag::SHADER_RESOURCE |
					gfx::eBindFlag::RENDER_TARGET,
					0
				},
				gfx::ISRVDesc{ gfx::eFormat::R16G16B16A16_FLOAT },
				gfx::IRTVDesc{ gfx::eFormat::R16G16B16A16_FLOAT }
			)
			,

			gfx::create_gfx_object(
				gfx::ResourceDesc<gfx::Texture2D> {
					dim,dim,
					1, 1, gfx::eFormat::R32_TYPELESS,
					{ 1, 0, },
					gfx::eUsage::DEFAULT,
					gfx::eBindFlag::SHADER_RESOURCE |
					gfx::eBindFlag::RENDER_TARGET,
					0
				},
				gfx::ISRVDesc{ gfx::eFormat::R32_FLOAT },
				gfx::IRTVDesc{ gfx::eFormat::R32_FLOAT }
			)
			,

			gfx::create_gfx_object(
				gfx::ResourceDesc<gfx::Texture2D> {
					dim,dim,
					1, 1, gfx::eFormat::R8G8B8A8_UNORM,
					{ 1, 0, },
					gfx::eUsage::DEFAULT,
					gfx::eBindFlag::SHADER_RESOURCE | gfx::eBindFlag::RENDER_TARGET,
					0
				},
				gfx::ISRVDesc{ gfx::eFormat::R8G8B8A8_UNORM },
				gfx::IRTVDesc{ gfx::eFormat::R8G8B8A8_UNORM }
			)
		};

		return std::move(gbuffer);
	}

	inline auto ___depthbuffer(uint32 dim)
	{
		FrameBuffer buffer
		{
			dim, dim,
			gfx::create_gfx_object(
				gfx::ResourceDesc<gfx::Texture2D> {
					dim,dim,
					1, 1,
					gfx::eFormat::R24G8_TYPELESS,
					{ 1, 0, },
					gfx::eUsage::DEFAULT,
					gfx::eBindFlag::SHADER_RESOURCE | gfx::eBindFlag::DEPTH_STENCIL, 
					0
				},
				gfx::IDSVDesc{ gfx::eFormat::D24_UNORM_S8_UINT }
			),

			gfx::create_gfx_object(
				gfx::ResourceDesc<gfx::Texture2D> {
					dim,dim,
					1, 1, gfx::eFormat::R32_TYPELESS,
					{ 1, 0, },
					gfx::eUsage::DEFAULT,
					gfx::eBindFlag::SHADER_RESOURCE | gfx::eBindFlag::RENDER_TARGET,
					0
				},
				gfx::ISRVDesc{ gfx::eFormat::R32_FLOAT },
				gfx::IRTVDesc{ gfx::eFormat::R32_FLOAT }
			)
		};

		return std::move(buffer);
	}

	inline auto ___minmax_depthbuffer(uint32 dim)
	{
		FrameBuffer buffer
		{
			dim, dim,
			gfx::create_gfx_object(
				gfx::ResourceDesc<gfx::Texture2D> {
					dim,dim,
					1, 1,
					gfx::eFormat::R24G8_TYPELESS,
					{ 1, 0, },
					gfx::eUsage::DEFAULT,
					gfx::eBindFlag::SHADER_RESOURCE | gfx::eBindFlag::DEPTH_STENCIL,
					0
				},
				gfx::IDSVDesc{ gfx::eFormat::D24_UNORM_S8_UINT }
			),

			gfx::create_gfx_object(
				gfx::ResourceDesc<gfx::Texture2D> {
					dim,dim,
					1, 1, gfx::eFormat::R32G32_TYPELESS,
					{ 1, 0, },
					gfx::eUsage::DEFAULT,
					gfx::eBindFlag::SHADER_RESOURCE | gfx::eBindFlag::RENDER_TARGET,
					0
				},
				gfx::ISRVDesc{ gfx::eFormat::R32G32_FLOAT },
				gfx::IRTVDesc{ gfx::eFormat::R32G32_FLOAT }
			)
		};

		return std::move(buffer);
	}

	inline auto ___rw_texture2D(gfx::eFormat format, uint32 dim, uint32 mip_levels)
	{
		if (mip_levels == 0u) {
			mip_levels = uint32(std::log2(dim)) + 1u;
		}

		return gfx::create_gfx_object(
			gfx::ResourceDesc<gfx::Texture2D> {
				dim, dim,
				mip_levels, 1u, format,
				{ 1, 0, },
				gfx::eUsage::DEFAULT,
				gfx::eBindFlag::SHADER_RESOURCE | gfx::eBindFlag::UNORDERED_ACCESS | gfx::eBindFlag::RENDER_TARGET,
				0, 0
			},
			gfx::ISRVDesc{ format },
			gfx::IRTVDesc{ format },
			gfx::IUAVDesc{ format }
		);
	}

	inline auto ___rw_texture3D(gfx::eFormat format, uint32 dim, uint32 mip_levels)
	{
		return gfx::create_gfx_object(
			gfx::ResourceDesc<gfx::Texture3D> {
				dim, dim, dim,
				mip_levels, format,
				gfx::eUsage::DEFAULT,
				gfx::eBindFlag::SHADER_RESOURCE | gfx::eBindFlag::UNORDERED_ACCESS | gfx::eBindFlag::RENDER_TARGET,
				0, 0
			},
			gfx::ISRVDesc{ format },
			gfx::IUAVDesc{ format }
		);
	}

	static auto ___staging_texture(gfx::eFormat format, uint32 dim, uint32 mip_levels)
	{
		auto tmp_buffer = gfx::create_gfx_object(
			gfx::ResourceDesc<gfx::Texture2D> {
				dim, dim,
				mip_levels, 1, format,
				{ 1, 0, },
				gfx::eUsage::STAGING,
				0,
				gfx::eCpuAccessFlag::CPU_ACCESS_READ,
				0
			}
		);
		return std::move(tmp_buffer);
	}
}
