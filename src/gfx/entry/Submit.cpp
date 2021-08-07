#include<gfx/entry/Submit.hpp>
#include<gfx/entry/UniformCache.hpp>
#include<gfx/middle/GfxTraits.hpp>
#include<gfx/entry/ConstBuffers.hpp>
#include<gfx/entry/Swapchain.hpp>
#include<memory/Allocator.hpp>
#include<memory/LinearAllocator.hpp>

#include<unordered_map>


namespace tutorial::gfx
{
	extern GfxDevice* _device;
	extern Sampler _point_sampler;
	extern Sampler _linear_wrap_sampler;
	extern Sampler _linear_clamp_sampler;

	Allocator _default_alloc;
	LinearAllocator _frame_allocator{ _default_alloc, 8 * 1024 * 1024 };


	template<class T>
	struct BindEvent
	{
		auto set(const T& v)
		{
			if (&v != _ptr) {
				_ptr = &v;
				_changed = true;
			}
		}

		auto get() -> const T& { return *_ptr; }

		auto trigger() -> bool {
			auto ret = _changed;
			_changed = false;
			return ret;
		}

	private:
		const T* _ptr = nullptr;
		bool _changed = false;
	};

	UniformCache* _default_uniform_cache = nullptr;
	UniformCache* _uniform_cache = nullptr;
	UniformCache* _compute_cache = nullptr;
	BindEvent<VertexBuffer> _vb;
	BindEvent<VertexLayout> _vert_layout;
	BindEvent<IndexBuffer> _ib;
	BindEvent<Shader> _vs;
	BindEvent<Shader> _gs;
	BindEvent<Shader> _ps;
	BindEvent<ShaderBinding> _shader_binding;
	using RegisterToHandle = std::unordered_map<uint8 /*register*/, UniformHandle>; // use array128
	std::array<RegisterToHandle, size_t(ShaderStage::Count)> _cbuffer_handles;
	std::vector<const SRV*> _srvs(128);  // use interface



	void set_primitive_type(PrimType type)
	{
		_device->set_primitive_type(type);
	}

	void set_blend_mode(BlendMode mode)
	{
		_device->set_blend_mode(mode);
	}

	void set_depth_test(bool depth_test, bool depth_write, DepthFunc func)
	{
		_device->set_depth_test(depth_test, depth_write, func);
	}

	void set_rasterize_mode(PolyCullMode cull_mode, PolyFillMode fill_mode, bool depth_clip)
	{
		_device->set_rasterize_mode(cull_mode, fill_mode, depth_clip);
	}

	void set_viewport(const Viewport viewports[], uint8 count)
	{
		_device->set_viewport(viewports, count);
	}

	void set_viewport(const Viewport& viewport)
	{
		Viewport viewports[] = { viewport };
		_device->set_viewport(viewports, 1);
	}

	void bind_framebuffer(const DSV& dsv, const std::tuple<const RTV&>& rtv)
	{
		FrameBuffer fb{ 0, 0, dsv, std::get<0>(rtv) };
		_device->bind_framebuffer(fb);
	}

	void bind_framebuffer(const DSV& dsv, const std::tuple<const RTV&, const RTV&>& rtv)
	{
		FrameBuffer fb{ 0, 0, dsv, std::get<0>(rtv), std::get<1>(rtv) };
		_device->bind_framebuffer(fb);
	}

	void bind_framebuffer(const DSV& dsv, const std::tuple<const RTV&, const RTV&, const RTV&>& rtv)
	{
		FrameBuffer fb{ 0, 0, dsv, std::get<0>(rtv), std::get<1>(rtv), std::get<2>(rtv) };
		_device->bind_framebuffer(fb);
	}

	void bind_framebuffer(const DSV& dsv, const std::tuple<const RTV&, const RTV&, const RTV&, const RTV&>& rtv)
	{
		FrameBuffer fb{ 0, 0, dsv, std::get<0>(rtv), std::get<1>(rtv), std::get<2>(rtv), std::get<3>(rtv) };
		_device->bind_framebuffer(fb);
	}

	void unbind_framebuffer(const DSV& dsv, const std::tuple<const RTV&>& rtv)
	{
		FrameBuffer fb{ 0, 0, dsv, std::get<0>(rtv) };
		_device->unbind_framebuffer(fb);
	}

	void unbind_framebuffer(const DSV& dsv, const std::tuple<const RTV&, const RTV&>& rtv)
	{
		FrameBuffer fb{ 0, 0, dsv, std::get<0>(rtv), std::get<1>(rtv) };
		_device->unbind_framebuffer(fb);
	}

	void unbind_framebuffer(const DSV& dsv, const std::tuple<const RTV&, const RTV&, const RTV&>& rtv)
	{
		FrameBuffer fb{ 0, 0, dsv, std::get<0>(rtv), std::get<1>(rtv), std::get<2>(rtv) };
		_device->unbind_framebuffer(fb);
	}

	void unbind_framebuffer(const DSV& dsv, const std::tuple<const RTV&, const RTV&, const RTV&, const RTV&>& rtv)
	{
		FrameBuffer fb{ 0, 0, dsv, std::get<0>(rtv), std::get<1>(rtv), std::get<2>(rtv), std::get<3>(rtv) };
		_device->unbind_framebuffer(fb);
	}

	void bind_computebuffer(const ShaderStage stage, const std::tuple<const UAV&>& uavs)
	{
		ComputeBuffer cb{ std::get<0>(uavs) };
		_device->bind_computebuffer(stage, cb);
	}

	void bind_computebuffer(const ShaderStage stage, const std::tuple<const UAV&, const UAV&>& uavs)
	{
		ComputeBuffer cb{ std::get<0>(uavs), std::get<1>(uavs) };
		_device->bind_computebuffer(stage, cb);
	}

	void unbind_computebuffer(const ShaderStage stage, const std::tuple<const UAV&>& uavs)
	{
		ComputeBuffer cb{ std::get<0>(uavs) };
		_device->unbind_computebuffer(stage, cb);
	}

	void unbind_computebuffer(const ShaderStage stage, const std::tuple<const UAV&, const UAV&>& uavs)
	{
		ComputeBuffer cb{ std::get<0>(uavs), std::get<1>(uavs) };
		_device->unbind_computebuffer(stage, cb);
	}

	void bind_shader(const Shader& vs, const Shader& gs, const Shader& ps, const ShaderBinding& binding)
	{
		_vs.set(vs);
		_gs.set(gs);
		_ps.set(ps);
		_shader_binding.set(binding);
	}

	void bind_shader(const Shader& vs, const Shader& ps, const ShaderBinding& binding)
	{
		_vs.set(vs);
		_ps.set(ps);
		_shader_binding.set(binding);
	}

	void bind_compute_shader(const Shader& cs)
	{
		_device->bind_shader(ShaderStage::CS, cs);
	}

	void bind_vertex_buffer(const VertexBuffer& vb, const VertexLayout& layout)
	{
		_vb.set(vb);
		_vert_layout.set(layout);
	}

	void bind_index_buffer(const IndexBuffer& ib)
	{
		_ib.set(ib);
	}

	void bind_sampler(uint8 stream, ShaderStage stage, const Sampler& sampler)
	{
		_device->bind_sampler(stream, stage, sampler);
	}

	void set_uniform(const UniformHandle& uniform_handle, const void* value, uint16 size)
	{
		assert(_uniform_cache && "[gfx][error] --- call gfx::begin() first");
		_uniform_cache->copy(uniform_handle, value, size);
	}

	void set_texture(const UniformHandle& uniform_handle, const SRV& srv)
	{
		assert(_uniform_cache && "[gfx][error] --- call gfx::begin() first");
		_uniform_cache->ref(uniform_handle, &srv, 0u);
	}

	void set_global_uniform(const UniformHandle& buffer_handle, const void* value, uint16 size)
	{
		assert(buffer_handle.valid());
		detail::ConstBuffers::copy(buffer_handle, value, 0u, size);
		detail::ConstBuffers::upload(_device, buffer_handle);
	}

	void set_global_texture(const UniformHandle& uniform_handle, const SRV& srv)
	{
		assert(uniform_handle.valid());
		_srvs[uniform_handle.index] = &srv;
	}

	void set_compute_uniform(const UniformHandle& uniform_handle, const void* value, uint16 size)
	{
		assert(_compute_cache);
		_compute_cache->copy(uniform_handle, value, size);
	}

	void set_compute_texture(const UniformHandle& uniform_handle, const SRV& srv)
	{
		assert(_compute_cache);
		_compute_cache->ref(uniform_handle, &srv, 0u);
	}

	inline auto switch_constbuffer(uint8 stream, const ShaderStage stage, const UniformHandle& handle)
	{
		if (_cbuffer_handles[uint8(stage)][stream] != handle)
		{
			auto cbuffer = detail::ConstBuffers::get(handle);
			_cbuffer_handles[uint8(stage)][stream] = handle;
			_device->bind_const_buffer(stream, stage, *cbuffer);
		}
	}

	inline auto switch_texture(uint8 stream, const ShaderStage stage, const UniformHandle& handle)
	{
		// don't need to record texture binding.
		// texture binding cache on CPU is not reliable,
		// because a texture can be write as a RT, at that moment, 
		// the SRV will be unbound by driver.
		auto srv = _srvs[handle.index];
		if (srv) {
			_device->bind_srv(stream, stage, *srv);
		}
	}

	inline auto switch_shader(const ShaderBinding& binding)
	{
		for (auto& cbuffer : binding.cbuffer_bindings)
		{
			switch_constbuffer(cbuffer.index, cbuffer.stage, cbuffer.handle);
		}

		for (auto& srv : binding.srv_bindings)
		{
			switch_texture(srv.index, srv.stage, srv.handle);
		}
	}

	inline auto flush_uniforms(const ShaderBinding& shader_binding, UniformCache& uniform_cache)
	{
		for (const auto& command : uniform_cache.commands)
		{
			const UniformHandle& uniform_handle = command.uniform_handle;
			if (uniform_handle.type == UniformType::Var)
			{
				ShaderBinding::Location&& loc = shader_binding.map_uniform_address(uniform_handle);
				if (const auto& cbuffer_handle = loc.buffer_handle; cbuffer_handle.valid())
				{
					detail::ConstBuffers::copy(cbuffer_handle, command.data, loc.offset, command.size);
					detail::ConstBuffers::upload(_device, cbuffer_handle);
				}
			}
			else if (uniform_handle.type == UniformType::Srv)
			{
				const auto& srv = *static_cast<const SRV*>(command.data);
				_srvs[uniform_handle.index] = &srv;
				for (auto& binding : shader_binding.srv_bindings)
				{
					if (binding.handle == uniform_handle)
					{
						// TODO:move out
						_device->bind_srv(binding.index, binding.stage, srv);
					}
				}
			}
		}

		uniform_cache.clear();
	}

	inline auto commit() -> void
	{
		if(_vb.trigger()) { _device->bind_vertex_buffer(0, _vb.get(), _vert_layout.get()); }
		if (_ib.trigger()) { _device->bind_index_buffer(_ib.get()); }
		if (_vs.trigger()) { _device->bind_shader(ShaderStage::VS, _vs.get()); }
		if (_gs.trigger()) { _device->bind_shader(ShaderStage::GS, _gs.get()); }
		if (_ps.trigger()) { _device->bind_shader(ShaderStage::PS, _ps.get()); }
		if (_shader_binding.trigger()) { switch_shader(_shader_binding.get()); }
		flush_uniforms(_shader_binding.get(), *_uniform_cache);
	}

	void draw(uint32 vert_count, uint32 start_vert)
	{
		commit();
		_device->draw(vert_count, start_vert);
	}

	void draw_indexed(uint32 index_count, uint32 start_index, uint32 start_vert)
	{
		commit();
		_device->draw_indexed(index_count, start_index, start_vert);
	}

	void draw_indexed_instanced(
		uint32 index_count,
		uint32 instance_count,
		uint32 start_index,
		uint32 start_vert,
		uint32 start_instance
	)
	{
		commit();
		_device->draw_indexed_instanced(
			index_count,
			instance_count,
			start_index,
			start_vert,
			start_instance
		);
	}

	void compute(const Shader& shader, const ShaderBinding& binding, uint32 group_x, uint32 group_y, uint32 group_z)
	{
		_device->bind_shader(ShaderStage::CS, shader);
		switch_shader(binding);
		flush_uniforms(binding, *_compute_cache);
		_device->dispatch(group_x, group_y, group_z);
	}

	auto create_frame_uniform_cache() -> void
	{
		auto p = _frame_allocator.allocate(sizeof(UniformCache));
		_default_uniform_cache = new(p) UniformCache{ _frame_allocator };

		p = _frame_allocator.allocate(sizeof(UniformCache));
		_compute_cache = new(p) UniformCache{ _frame_allocator };
	}

	auto bind_samplers() -> void
	{
		bind_sampler(0, gfx::ShaderStage::PS, _point_sampler);
		bind_sampler(1, gfx::ShaderStage::PS, _linear_wrap_sampler);
		bind_sampler(2, gfx::ShaderStage::PS, _linear_clamp_sampler);
		bind_sampler(0, gfx::ShaderStage::CS, _point_sampler);
		bind_sampler(1, gfx::ShaderStage::CS, _linear_wrap_sampler);
		bind_sampler(2, gfx::ShaderStage::CS, _linear_clamp_sampler);
	}

	auto next_frame() -> void
	{
		_frame_allocator.reset();
		set_blend_mode(BlendMode::Replace);
		set_depth_test(true, true, gfx::DepthFunc::DSS_DEPTHFUNC_LESS);
		set_rasterize_mode(PolyCullMode::PCM_BACK, PolyFillMode::PFM_SOLID, true);
		static VertexBuffer vb; _vb.set(vb);
		static VertexLayout vert_layout; _vert_layout.set(vert_layout);
		static IndexBuffer ib; _ib.set(ib);
		static Shader vs; _vs.set(vs);
		static Shader gs; _gs.set(gs);
		static Shader ps; _ps.set(ps);;
		static ShaderBinding default_binding; _shader_binding.set(default_binding);
		create_frame_uniform_cache();
		bind_samplers();
	}

	auto begin() -> bool
	{
		_uniform_cache = _default_uniform_cache;
		return true;
	}

	auto end() -> void
	{
		_uniform_cache = nullptr;
	}

	auto present(const Swapchain& swapchain) -> void
	{
		_device->present(swapchain);
	}
}