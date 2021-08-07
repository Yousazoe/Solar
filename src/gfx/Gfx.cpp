#include<gfx/Gfx.hpp>
#include<gfx/middle/GfxTraits.hpp>

namespace tutorial::gfx
{
	GfxDevice* _device = nullptr;
	GfxAllocator* _allocator = nullptr;

	Sampler _point_sampler;
	Sampler _linear_wrap_sampler;
	Sampler _linear_clamp_sampler;

	auto setup_samplers() -> void
	{
		_point_sampler = gfx::create_sampler(
			gfx::SAMPLER_DESC{
				gfx::FILTER::FILTER_MIN_MAG_MIP_POINT,
				gfx::TEXTURE_ADDRESS_MODE::TEXTURE_ADDRESS_CLAMP,
				gfx::TEXTURE_ADDRESS_MODE::TEXTURE_ADDRESS_CLAMP,
				gfx::TEXTURE_ADDRESS_MODE::TEXTURE_ADDRESS_CLAMP,
				0,
				0,
				gfx::COMPARISON_FUNC::COMPARISON_NEVER,
				0
			}
		);

		_linear_wrap_sampler = gfx::create_sampler(
			gfx::SAMPLER_DESC{
				gfx::FILTER::FILTER_MIN_MAG_MIP_LINEAR,
				gfx::TEXTURE_ADDRESS_MODE::TEXTURE_ADDRESS_WRAP,
				gfx::TEXTURE_ADDRESS_MODE::TEXTURE_ADDRESS_WRAP,
				gfx::TEXTURE_ADDRESS_MODE::TEXTURE_ADDRESS_WRAP,
				0,
				0,
				gfx::COMPARISON_FUNC::COMPARISON_NEVER,
				0
			}
		);

		_linear_clamp_sampler = create_sampler(
			gfx::SAMPLER_DESC{
				gfx::FILTER::FILTER_MIN_MAG_MIP_LINEAR,
				gfx::TEXTURE_ADDRESS_MODE::TEXTURE_ADDRESS_CLAMP,
				gfx::TEXTURE_ADDRESS_MODE::TEXTURE_ADDRESS_CLAMP,
				gfx::TEXTURE_ADDRESS_MODE::TEXTURE_ADDRESS_CLAMP,
				0,
				0,
				gfx::COMPARISON_FUNC::COMPARISON_NEVER,
				0
			}
		);
	}

	auto setup() -> void
	{
		_device = new GfxDevice(false, false, 0);
		_device->setup();
		_allocator = new GfxAllocator(*_device);

		UniformHash::setup();
		setup_samplers();
	}

	auto shutdown() -> void
	{
		delete _allocator; _allocator = nullptr;
		_device->shutdown();
		delete _device; _device = nullptr;
	}


}