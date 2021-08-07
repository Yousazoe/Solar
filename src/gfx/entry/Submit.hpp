#pragma once
#include<gfx/entry/GfxObject.hpp>
#include<gfx/entry/ShaderBinding.hpp>

namespace tutorial::gfx
{
	void set_primitive_type(PrimType type);
	void set_blend_mode(BlendMode mode);
	void set_depth_test(bool depth_test, bool depth_write, DepthFunc func);
	void set_rasterize_mode(PolyCullMode cull_mode, PolyFillMode fill_mode, bool depth_clip);
	void set_viewport(const Viewport viewports[], uint8 count);
	void set_viewport(const Viewport& viewport);
	void bind_framebuffer(const DSV& dsv, const std::tuple<const RTV&>& rtv);
	void bind_framebuffer(const DSV& dsv, const std::tuple<const RTV&, const RTV&>& rtv);
	void bind_framebuffer(const DSV& dsv, const std::tuple<const RTV&, const RTV&, const RTV&>& rtv);
	void bind_framebuffer(const DSV& dsv, const std::tuple<const RTV&, const RTV&, const RTV&, const RTV&>& rtv);
	void unbind_framebuffer(const DSV& dsv, const std::tuple<const RTV&>& rtv);
	void unbind_framebuffer(const DSV& dsv, const std::tuple<const RTV&, const RTV&>& rtv);
	void unbind_framebuffer(const DSV& dsv, const std::tuple<const RTV&, const RTV&, const RTV&>& rtv);
	void unbind_framebuffer(const DSV& dsv, const std::tuple<const RTV&, const RTV&, const RTV&, const RTV&>& rtv);
	template<class D, class ...R>
	void bind_framebuffer(const FrameBuffer<D, R...>& buffer);
	template<class D, class ...R>
	void unbind_framebuffer(const FrameBuffer<D, R...>& buffer);
	void bind_computebuffer(const ShaderStage stage, const std::tuple<const UAV&>& uavs);
	void bind_computebuffer(const ShaderStage stage, const std::tuple<const UAV&, const UAV&>& uavs);
	void unbind_computebuffer(const ShaderStage stage, const std::tuple<const UAV&>& uavs);
	void unbind_computebuffer(const ShaderStage stage, const std::tuple<const UAV&, const UAV&>& uavs);
	template<class ...U>
	void bind_computebuffer(const ShaderStage stage, const ComputeBuffer<U...>& buffer);
	template<class ...U>
	void unbind_computebuffer(const ShaderStage stage, const ComputeBuffer<U...>& buffer);
	void bind_shader(const Shader& vs, const Shader& gs, const Shader& ps, const ShaderBinding& binding);
	void bind_shader(const Shader& vs, const Shader& ps, const ShaderBinding& binding);
	void bind_compute_shader(const Shader& cs);
	void bind_vertex_buffer(const VertexBuffer& vb, const VertexLayout& layout);
	void bind_index_buffer(const IndexBuffer& ib);
	void set_uniform(const UniformHandle& uniform_handle, const void* value, uint16 size);
	void set_texture(const UniformHandle& uniform_handle, const SRV& srv);
	void set_global_uniform(const UniformHandle& buffer_handle, const void* value, uint16 size);
	void set_global_texture(const UniformHandle& uniform_handle, const SRV& srv);
	template<class T>
	void set_compute_uniform(const UniformHandle& uniform_handle, const T& value);
	void set_compute_uniform(const UniformHandle& uniform_handle, const void* value, uint16 size);
	void set_compute_texture(const UniformHandle& uniform_handle, const SRV& srv);
	template<class T>
	void set_uniform(const UniformHandle& uniform_handle, const T& value);
	void bind_sampler(uint8 stream, ShaderStage stage, const Sampler& sampler);
	void draw(uint32 vert_count, uint32 start_vert);
	void draw_indexed(uint32 index_count, uint32 start_index, uint32 start_vert);
	void draw_indexed_instanced(uint32 index_count, uint32 instance_count, uint32 start_index, uint32 start_vert, uint32 start_instance);
	void compute(const Shader& shader, const ShaderBinding& binding, uint32 group_x, uint32 group_y, uint32 group_z);



	template<class D, class ...R>
	inline void bind_framebuffer(const gfx::FrameBuffer<D, R...>& buffer)
	{
		bind_framebuffer(buffer.dsv, buffer.rtvs);
	}

	template<class D, class ...R>
	void unbind_framebuffer(const gfx::FrameBuffer<D, R...>& buffer)
	{
		unbind_framebuffer(buffer.dsv, buffer.rtvs);
	}

	template<class ...U>
	void bind_computebuffer(const gfx::ShaderStage stage, const gfx::ComputeBuffer<U...>& buffer)
	{
		bind_computebuffer(stage, buffer.uavs);
	}

	template<class ...U>
	void unbind_computebuffer(const gfx::ShaderStage stage, const gfx::ComputeBuffer<U...>& buffer)
	{
		unbind_computebuffer(stage, buffer.uavs);
	}

	template<class T>
	void set_compute_uniform(const UniformHandle& uniform_handle, const T& value)
	{
		set_compute_uniform(uniform_handle, &value, sizeof(value));
	}

	template<class T>
	void set_uniform(const UniformHandle& uniform_handle, const T& value)
	{
		set_uniform(uniform_handle, &value, sizeof(value));
	}
}
