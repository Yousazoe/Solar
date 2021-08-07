#pragma once
#include<memory/StdProxyAllocator.hpp>
#include<core/Types.hpp>
#include<vector>
#include<gfx/entry/UniformHash.hpp>

namespace tutorial::gfx
{
	class UniformCache final
	{
	public:
		UniformCache(IAllocator& frame_allocator) noexcept :
			_frame_allocator{ frame_allocator }, 
			commands{ _frame_allocator } {}

	public:
		struct Command {
			UniformHandle uniform_handle;
			const void* data;
			uint16 size;
		};
		auto clear() -> void { commands.clear(); }
		auto copy(const UniformHandle& uniform_handle, const void* data, uint16 size) -> void;
		auto ref(const UniformHandle& uniform_handle, const void* data, uint16 size) -> void;

	private:
		IAllocator& _frame_allocator;

	public:
		std::vector<Command, StdProxyAllocator<Command>> commands;
	};

	inline auto UniformCache::copy(
		const UniformHandle& uniform_handle,
		const void* data,
		uint16 size
	) -> void
	{
		auto tmp_data = _frame_allocator.allocate(size);
		std::memcpy(tmp_data, data, size);
		commands.push_back({ uniform_handle, tmp_data, size });
	}

	inline auto UniformCache::ref(
		const UniformHandle& uniform_handle, 
		const void* data, 
		uint16 size
	) -> void
	{
		commands.push_back({ uniform_handle, data, size });
	}
}
