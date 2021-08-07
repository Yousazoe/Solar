#pragma once
#include<../app/graphics-pro/mt/CommandQueue.hpp>

namespace disk::gfx
{
	class CommandDispatcher final
	{
	public:
		CommandDispatcher(gfx::GfxFrameContext& context);
		~CommandDispatcher() = default;

	public:
		void execute(CommandQueue& command_que);

	private:
		gfx::GfxFrameContext& _context;
	};

	inline CommandDispatcher::CommandDispatcher(
		gfx::GfxFrameContext& context
	) :
		_context{ context }
	{
	}

	inline void CommandDispatcher::execute(CommandQueue& command_que)
	{
		command_que._prepare();

		std::atomic_thread_fence(std::memory_order::memory_order_acquire);
		auto count = command_que._count.load(
			std::memory_order::memory_order_relaxed
		);
		for (auto i = 0u; i < count; ++i)
		{
			// decode
			const auto& state = command_que._commands[i].state->state;
			_context.set_blend_mode((gfx::BlendMode)state.blend);
			_context.set_depth_test(
				state.depth_test, 
				state.depth_write, 
				(gfx::DepthFunc)state.depth_compare_func
			);
			_context.set_rasterize_mode(
				(gfx::PolyCullMode)state.cull_mode, 
				(gfx::PolyFillMode)state.fill_mode, 
				state.depth_clip
			);

			// render
			auto cmd = command_que._commands[i].command;
			while (cmd) {
				cmd->func(_context, cmd->data);
				cmd = cmd->next;
			}
		}

		command_que._finish();
	}
}
