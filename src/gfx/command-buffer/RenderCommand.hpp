#pragma once
#include<core/Types.hpp>
#include<../app/graphics-pro/mt/entry/GfxObject.hpp>
#include<../app/graphics-pro/mt/middle/GfxTraits.hpp>

#include<../thirdparty/brofiler/Brofiler.h>
namespace disk::gfx::detail
{
	struct UploadBufferCommand {
		gfx::ResourceObject* buffer;
		const void* data;
		uint32 size;
	};

	struct UploadDynamicBufferCommand {
		gfx::ResourceObject* buffer;
		const void* data;
		uint32 size;
	};

	struct BindConstBufferCommand {
		uint8 stream;
		gfx::ShaderStage stage;
		const gfx::ConstBuffer* cb;
	};

	struct BindSRVCommand {
		uint8 stream;
		gfx::ShaderStage stage;
		const gfx::SRV* srv;
	};

	struct BindVertexbufferCommand {
		uint8 stream;
		const gfx::VertexBuffer* vb;
		const gfx::VertexLayout* layout;
	};

	struct BindIndexbufferCommand {
		const gfx::IndexBuffer* ib;
	};

	struct BindShaderCommand {
		const gfx::Shader* vs;
		const gfx::Shader* gs;
		const gfx::Shader* ps;
	};

	struct DrawCommand {
		uint32 vert_count;
		uint32 start_vert;
	};

	struct DrawIndexedCommand {
		uint32 index_count;
		uint32 start_vert;
		uint32 start_index;
	};

	struct DrawIndexedInstancedCommand {
		uint32 index_count;
		uint32 instance_count;
		uint32 start_index;
		uint32 start_vert;
		uint32 start_instance;
	};

	inline void upload_buffer(gfx::GfxFrameContext& context, void* data) 
	{
		auto& param = *static_cast<UploadBufferCommand*>(data);
		assert(false && "upload_buffer no impl");
		//context.upload_data(*param->buffer, param->data, param->size);
	}

	inline void upload_dynamic_buffer(gfx::GfxFrameContext& context, void* data)
	{
		//BROFILER_CATEGORY("upload_dynamic_buffer", Profiler::Color::DarkRed);
		auto& param = *static_cast<UploadDynamicBufferCommand*>(data);
		context.upload_dynamic_data(*param.buffer, param.data, param.size);
	}

	inline void bind_vertex_buffer(gfx::GfxFrameContext& context, void* data)
	{
		//BROFILER_CATEGORY("bind_vertex_buffer", Profiler::Color::LightSalmon);
		auto& param = *static_cast<BindVertexbufferCommand*>(data);
		context.bind_vertex_buffer(param.stream, *param.vb, *param.layout);
	}

	inline void bind_index_buffer(gfx::GfxFrameContext& context, void* data) 
	{
		//BROFILER_CATEGORY("bind_index_buffer", Profiler::Color::AntiqueWhite);
		auto& param = *static_cast<BindIndexbufferCommand*>(data);
		context.bind_index_buffer(*param.ib);
	}

	inline void bind_const_buffer(gfx::GfxFrameContext& context, void* data) 
	{
		//BROFILER_CATEGORY("bind_const_buffer", Profiler::Color::DeepSkyBlue);
		auto& param = *static_cast<BindConstBufferCommand*>(data);
		context.bind_const_buffer(param.stream,  param.stage, *param.cb);
	}

	inline void bind_srv(gfx::GfxFrameContext& context, void* data) 
	{
		//BROFILER_CATEGORY("bind_const_buffer", Profiler::Color::DeepSkyBlue);
		auto& param = *static_cast<BindSRVCommand*>(data);
		context.bind_srv(param.stream, param.stage, *param.srv);
	}

	inline void bind_shader(gfx::GfxFrameContext& context, void* data)
	{
		//BROFILER_CATEGORY("bind_shader", Profiler::Color::DarkTurquoise);
		auto& param = *static_cast<BindShaderCommand*>(data);
		context.bind_shader(*param.vs, *param.gs, *param.ps);
	}

	inline void draw(gfx::GfxFrameContext& context, void* data) 
	{
		//BROFILER_CATEGORY("draw", Profiler::Color::Chartreuse);
		auto& param = *static_cast<DrawCommand*>(data);
		context.draw(param.vert_count, param.start_vert);
	}

	inline void draw_indexed(gfx::GfxFrameContext& context, void* data) 
	{
		//BROFILER_CATEGORY("draw_indexed", Profiler::Color::Azure);
		auto& param = *static_cast<DrawIndexedCommand*>(data);
		context.draw_indexed(param.index_count, param.start_vert, param.start_index);
	}

	inline void draw_indexed_instanced(gfx::GfxFrameContext& context, void* data) 
	{
		auto& param = *static_cast<DrawIndexedInstancedCommand*>(data);
		context.draw_indexed_instanced(
			param.index_count,
			param.instance_count,
			param.start_index,
			param.start_vert,
			param.start_instance
		);
	}
}
