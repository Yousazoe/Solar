#pragma once
#include<gfx/entry/Define.hpp>
#include<gfx/entry/GfxObject.hpp>
#include<gfx/middle/StateInterface.hpp>
#include<gfx/entry/Swapchain.hpp>
#include<string>

namespace tutorial::gfx
{
	template<class GraphicsApi>
	class IGfxDevice
	{
	public:
		IGfxDevice(bool debug_msg, bool enable_msaa4x, uint32 msaa4x_quality);
		~IGfxDevice();

	public:
		bool setup();
		bool shutdown();
		GPUHandle get_device() const;
		GPUHandle get_context() const; 
		GPUHandle compile_shader(ShaderStage type, const std::string& bytecode);
		GPUHandle create_sampler(const SAMPLER_DESC& desc);
		auto create_swapchain(void* hwnd, uint32 width, uint32 height)->Swapchain;

		void clear_rtv(const RTV& rtv, const float color[4]) const;
		void clear_dsv(const DSV& dsv, float depth = 1.0f) const;
		void copy_texture2D(
			Texture2D& dst_texture,
			const uint8 dst_mip,
			const uint16 dst_x,
			const uint16 dst_y,
			const Texture2D& src_texture,
			const uint8 src_mip,
			const uint16 src_left,
			const uint16 src_top,
			const uint16 src_right,
			const uint16 src_bottom
		);
		void copy_texture2D(
			const SubResource& dst,
			const uint16 dst_x,
			const uint16 dst_y,
			const SubResource& src,
			const uint16 src_left,
			const uint16 src_top,
			const uint16 src_right,
			const uint16 src_bottom
		);
		void upload_data(ResourceObject& resouce, uint32 start_byte, uint32 end_byte, const void* data, uint32 row_pitch);
		void upload_data(ResourceObject& resouce, uint32 left, uint32 right, uint32 top, uint32 bottom, const void* data, uint32 row_pitch);
		void upload_texture(
			SubResource& texture,
			uint32 left,
			uint32 right,
			uint32 top,
			uint32 bottom,
			const void* data,
			uint32 src_row_pitch
		);
		void upload_texture(
			Texture2D& texture,
			uint8 mip,
			uint32 left,
			uint32 right,
			uint32 top,
			uint32 bottom,
			const void* data,
			uint32 src_row_pitch
		);
		void upload_texture(
			Texture2DArray& texture,
			uint8 array_index,
			uint8 mip,
			uint32 left,
			uint32 right,
			uint32 top,
			uint32 bottom,
			const void* data,
			uint32 src_row_pitch
		);
		void download_texture(Texture2D& texture, uint8 mip, void* buffer);
		void upload_dynamic_data(const ResourceObject& resouce, const void* data, uint32 size);
		void set_primitive_type(PrimType type);
		void set_blend_mode(BlendMode mode);
		void set_depth_test(bool depth_test, bool depth_write, DepthFunc func);
		void set_rasterize_mode(PolyCullMode cull_mode, PolyFillMode fill_mode, bool depth_clip);
		void set_viewport(const Viewport viewports[], uint8 count);
		//void setSampler(uint8 slot, SamplerState samp);
		template<class D, class ...R>
		void bind_framebuffer(const FrameBuffer<D, R...>& buffer);
		template<class D, class ...R>
		void unbind_framebuffer(const FrameBuffer<D, R...>& buffer);
		template<class ...U>
		void bind_computebuffer(const ShaderStage stage, const ComputeBuffer<U...>& buffer);
		template<class ...U>
		void unbind_computebuffer(const ShaderStage stage, const ComputeBuffer<U...>& buffer);
		void bind_shader(const ShaderStage stage, const Shader& shader);
		void bind_vertex_buffer(uint8 stream, const VertexBuffer& vb, const VertexLayout& layout);
		void bind_index_buffer(const IndexBuffer& ib);
		void bind_const_buffer(uint8 stream, ShaderStage stage, const ConstBuffer& cb);
		void bind_srv(uint8 stream, ShaderStage stage, const SRV& srv);
		void bind_sampler(uint8 stream, ShaderStage stage, const Sampler& sampler);
		void draw(uint32 vert_count, uint32 start_vert);
		void draw_indexed(uint32 index_count, uint32 start_index, uint32 start_vert);
		void draw_indexed_instanced(
			uint32 index_count,
			uint32 instance_count,
			uint32 start_index,
			uint32 start_vert,
			uint32 start_instance
		);
		void dispatch(uint32 group_x, uint32 group_y, uint32 group_z);
		auto present(const Swapchain& swapchain) -> void;
	};
}