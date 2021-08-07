#pragma once
#include<type_traits>
#include<functional>
#include<../app/graphics-pro/mt/middle/GfxTraits.hpp>

namespace disk::gfx
{
	struct CommandBuffer final
	{
		using CommandData = void*;
		using CommandFunc = void(*)(gfx::GfxFrameContext&, void*);

		CommandFunc func;
		CommandData data;
		CommandBuffer* next;
	};
	static_assert(std::is_pod_v<CommandBuffer>, "CommandBuffer must be POD");
}
