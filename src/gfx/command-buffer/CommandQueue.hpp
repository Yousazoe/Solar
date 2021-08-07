#pragma once
#include<../app/graphics-pro/mt/CommandBuffer.hpp>
#include<../app/graphics-pro/mt/StateCommand.hpp>
#include<memory/IAllocator.hpp>
#include<atomic>
#include<functional>

namespace disk::gfx
{
	class CommandQueue;

	class CommandQueueProxy final
	{
	public:
		CommandQueueProxy(CommandQueue& pass);
		~CommandQueueProxy();
		operator bool() const { return true; }

	public:
		auto new_state() -> SetStateCommand&;
		auto set_uniform(const UniformHandle& uniform_handle, const void* data, uint32 size) -> void;
		auto set_uniform(const UniformHandle& buffer_handle, const UniformHandle& uniform_handle, const void* data, uint32 size) -> void;
		auto bind_vertex_buffer(uint8 stream, const VertexBuffer& vb, const VertexLayout& layout) -> void;
		auto bind_index_buffer(const IndexBuffer& ib) -> void;
		auto bind_srv(const UniformHandle& srv_handle, ShaderStage stage, const SRV& srv) -> void;
		auto bind_shader(const Shader& vs, const Shader& gs, const Shader& ps, const ShaderBinding& binding) -> void;
		auto draw(
			const uint32 vert_count, 
			const uint32 start_vert
		) -> void;
		auto draw_indexed(
			uint32 index_count, 
			uint32 start_vert,
			uint32 start_index
		) -> void;
		auto draw_indexed_instanced(
			uint32 index_count,
			uint32 instance_count,
			uint32 start_index,
			uint32 start_vert,
			uint32 start_instance
		) -> void;

	private:
		void set_state(SetStateCommand* state);
		void submit(CommandBuffer* cmd);

	private:
		CommandQueue& _queue;
		SetStateCommand* _state;
		CommandBuffer* _head;
		CommandBuffer* _tail;
	};

	class CommandQueue final
	{
		friend class CommandDispatcher;
	public:
		using CommandQueueEntry = std::function<void()>;
		using CommandQueueExit = std::function<void()>;

	public:
		CommandQueue(
			IAllocator& allocator, 
			size_t count, 
			std::function<void()> prepare = [] {},
			std::function<void()> finish = [] {}
		);
		~CommandQueue() {
			// TODO : deallocate
		}
		void set_entry(CommandQueueEntry entry_func);
		void set_exit(CommandQueueEntry exit_func);
		auto new_group() { return CommandQueueProxy(*this); }
		void reserve(size_t count);
		void submit(SetStateCommand* state, CommandBuffer* cmd);
		void clear();
		void sort();

	private:
		const size_t _size;
		CommandQueueEntry _prepare;
		CommandQueueExit _finish;

		struct KeyValue {
			bool operator<(const KeyValue& rhs) const {
				return state->get_key() < rhs.state->get_key();
			}
			SetStateCommand* state; // 4bytes
			CommandBuffer* command; // 4bytes
		} *_commands;
		std::atomic<size_t> _count = 0;
	};


	inline CommandQueueProxy::CommandQueueProxy(CommandQueue& pass) :
		_queue{ pass },
		_state{ nullptr },
		_head{ nullptr },
		_tail{ nullptr }
	{
	}

	inline CommandQueueProxy::~CommandQueueProxy()
	{
		if (_head) {
			_queue.submit(_state, _head);
		}
	}

	inline void CommandQueueProxy::set_state(SetStateCommand* state)
	{
		_state = state;
	}

	inline void CommandQueueProxy::submit(CommandBuffer* cmd)
	{
		if (_head) {
			_tail->next = cmd;
			_tail = cmd;
		}
		else {
			_head = _tail = cmd;
		}
	}

	inline CommandQueue::CommandQueue(
		IAllocator& allocator,
		size_t count,
		std::function<void()> prepare,
		std::function<void()> finish 
	) :
		_size{ count },
		_commands{ 
				static_cast<KeyValue*>(
					allocator.allocate(count * sizeof(KeyValue))
				)
		},
		_prepare{ prepare },
		_finish{ finish }
	{
	}

	static thread_local size_t _where_to_submit = 0;

	inline void CommandQueue::set_entry(
		CommandQueueEntry entry_func
	)
	{
		_prepare = entry_func;
	}

	inline void CommandQueue::set_exit(CommandQueueEntry exit_func)
	{
		_finish = exit_func;
	}

	inline void CommandQueue::reserve(size_t count)
	{
		_where_to_submit = _count.fetch_add(
			count, 
			std::memory_order::memory_order_relaxed
		);
	}

	inline void CommandQueue::submit(
		SetStateCommand* state, 
		CommandBuffer* cmd
	)
	{
		auto i = _where_to_submit++;
		_commands[i].state = state;
		_commands[i].command = cmd;
	}

	inline void CommandQueue::clear()
	{
		_count.store(0, std::memory_order::memory_order_relaxed);
	}

	inline void CommandQueue::sort()
	{
		std::atomic_thread_fence(std::memory_order::memory_order_acquire);
		auto count = _count.load(std::memory_order_relaxed);
		std::sort(_commands, _commands + count);
	}
}
