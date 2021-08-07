//#pragma once
//#include<memory/IAllocator.hpp>
//#include<../app/graphics-pro/mt/StateCommand.hpp>
//#include<../app/graphics-pro/mt/CommandBuffer.hpp>
//#include<../app/graphics-pro/mt/RenderCommand.hpp>
//
//#include<memory/LinearAllocator.hpp>
//
//namespace disk::gfx
//{
//	class MtRenderer
//	{
//	public:
//		MtRenderer(IAllocator& allocator);
//		~MtRenderer() = default;
//		MtRenderer(const MtRenderer&) = delete;
//		void operator=(const MtRenderer&) = delete;
//		MtRenderer(MtRenderer&&) = delete;
//		void operator=(MtRenderer&&) = delete;
//		
//	public:
//		void frame_begin();
//		auto new_state()->SetStateCommand*;
//		auto upload_buffer(
//			gfx::ResourceObject& buffer, 
//			void* data, 
//			uint32 size
//		);
//		auto upload_dynamic_data(
//			gfx::ResourceObject& buffer, 
//			const void* data, 
//			uint32 size
//		);
//		auto bind_vertex_buffer(
//			uint8 stream, 
//			const gfx::VertexBuffer& vb, 
//			const gfx::VertexLayout& layout
//		);
//		auto bind_index_buffer(const gfx::IndexBuffer& ib);
//		auto bind_const_buffer(
//			uint8 stream,
//			gfx::ShaderStage stage,
//			const gfx::ConstBuffer& cb
//		);
//		auto bind_srv(uint8 stream, gfx::ShaderStage stage, const gfx::SRV& srv);
//		auto bind_shader(
//			const gfx::Shader& vs, 
//			const gfx::Shader& gs,
//			const gfx::Shader& ps
//		);
//		auto draw(const uint32 vert_count, const uint32 start_vert);
//		auto draw_indexed(
//			uint32 index_count, 
//			uint32 start_vert,
//			uint32 start_index
//		);
//		auto draw_indexed_instanced(
//			uint32 index_count,
//			uint32 instance_count,
//			uint32 start_index,
//			uint32 start_vert,
//			uint32 start_instance
//		);
//	private:
//		IAllocator& get_allocator();
//
//	private:
//		IAllocator& _allocator;
//		size_t _frame;
//	};
//
//	template<typename CommandData, typename CommandFunc, typename... Args>
//	inline CommandBuffer* allocate_command(IAllocator& allocator, CommandFunc func, Args&&...args)
//	{
//		auto cmd = static_cast<CommandBuffer*>(allocator.allocate(sizeof(CommandBuffer)));
//		cmd->func = func;
//		auto data = allocator.allocate(sizeof(CommandData));
//		cmd->data = new(data)CommandData{ args... };
//		cmd->next = nullptr;
//		return cmd;
//	}
//
//	inline auto MtRenderer::new_state()->SetStateCommand*
//	{
//		auto state = get_allocator().allocate(sizeof(SetStateCommand));
//		return new(state)SetStateCommand{};
//	}
//
//	inline auto MtRenderer::upload_buffer(gfx::ResourceObject& buffer, void* data, uint32 size)
//	{
//		return allocate_command<detail::UploadBufferCommand>(get_allocator(), detail::upload_buffer, &buffer, data, size);
//	}
//
//	inline auto MtRenderer::upload_dynamic_data(gfx::ResourceObject& buffer, const void* data, uint32 size)
//	{
//		auto tmp_data = get_allocator().allocate(size);
//		std::memcpy(tmp_data, data, size);
//		return allocate_command<detail::UploadDynamicBufferCommand>(get_allocator(), detail::upload_dynamic_buffer, &buffer, tmp_data, size);
//	}
//
//	inline auto MtRenderer::bind_vertex_buffer(uint8 stream, const gfx::VertexBuffer& vb, const gfx::VertexLayout& layout)
//	{
//		return allocate_command<detail::BindVertexbufferCommand>(get_allocator(), detail::bind_vertex_buffer, stream, &vb, &layout);
//	}
//
//	inline auto MtRenderer::bind_index_buffer(const gfx::IndexBuffer& ib)
//	{
//		return allocate_command<detail::BindIndexbufferCommand>(get_allocator(), detail::bind_index_buffer, &ib);
//	}
//
//	inline auto MtRenderer::bind_const_buffer(uint8 stream, gfx::ShaderStage stage, const gfx::ConstBuffer& cb)
//	{
//		return allocate_command<detail::BindConstBufferCommand>(get_allocator(), detail::bind_const_buffer, stream, stage, &cb);
//	}
//
//	inline auto MtRenderer::bind_srv(uint8 stream, gfx::ShaderStage stage, const gfx::SRV& srv)
//	{
//		return allocate_command<detail::BindSRVCommand>(get_allocator(), detail::bind_srv, stream, stage, &srv);
//	}
//
//	inline auto MtRenderer::bind_shader(const gfx::Shader& vs, const gfx::Shader& gs, const gfx::Shader& ps)
//	{
//		return allocate_command<detail::BindShaderCommand>(get_allocator(), detail::bind_shader, &vs, &gs, &ps);
//	}
//
//	inline auto MtRenderer::draw(const uint32 vert_count, const uint32 start_vert)
//	{
//		return allocate_command<detail::DrawCommand>(get_allocator(), detail::draw, vert_count, start_vert);
//	}
//
//	inline auto MtRenderer::draw_indexed(
//		uint32 index_count,
//		uint32 start_vert, 
//		uint32 start_index
//	)
//	{
//		return allocate_command<detail::DrawIndexedCommand>(
//			get_allocator(), 
//			detail::draw_indexed, 
//			index_count, 
//			start_vert, 
//			start_index
//		);
//	}
//
//	inline auto MtRenderer::draw_indexed_instanced(
//		uint32 index_count, 
//		uint32 instance_count,
//		uint32 start_index,
//		uint32 start_vert, 
//		uint32 start_instance
//	)
//	{
//		return allocate_command<detail::DrawIndexedInstancedCommand>(
//			get_allocator(), 
//			detail::draw_indexed_instanced,
//			index_count,
//			instance_count,
//			start_index,
//			start_vert,
//			start_instance
//		);
//	}
//}
