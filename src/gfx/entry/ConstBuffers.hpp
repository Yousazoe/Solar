#pragma once
#include<string>
#include<memory>
#include<gfx/entry/GfxObject.hpp>
#include<gfx/entry/UniformHash.hpp>
#include<gfx/middle/GfxTraits.hpp>

namespace tutorial::gfx::detail
{
	class ConstBuffers final
	{
		ConstBuffers() = delete;
	public:
		static auto get(const std::string& name) -> UniformHandle;
		static auto get_or_create(const std::string& name, uint16 size) -> UniformHandle;
		static auto get(const UniformHandle& handle)-> const ConstBuffer*;
		static auto copy(const UniformHandle& handle, const void* data, uint16 offset, uint16 size) -> void;
		static auto upload(GfxDevice* device, const UniformHandle& handle) -> void;
	};
}
