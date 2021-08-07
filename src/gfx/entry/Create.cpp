#include<gfx/entry/Create.hpp>
#include<gfx/middle/GfxTraits.hpp>

namespace tutorial::gfx
{
	extern GfxDevice* _device;
	extern GfxAllocator* _allocator;
	
	auto create_swapchain(void* hwnd, uint32 width, uint32 height) -> Swapchain
	{
		return _device->create_swapchain(hwnd, width, height);
	}

	Texture2D create_texture2D(const ResourceDesc<Texture2D>& desc)
	{
		return _allocator->create_texture2D(desc);
	}

	Texture3D create_texture3D(const ResourceDesc<Texture3D>& desc)
	{
		return _allocator->create_texture3D(desc);
	}

	Texture2DArray create_texture2DArray(const ResourceDesc<Texture2DArray>& desc)
	{
		return _allocator->create_texture2DArray(desc);
	}

	StructureBuffer create_buffer(const ResourceDesc<StructureBuffer>& desc)
	{
		return _allocator->create_buffer(desc);
	}

	ConstBuffer create_buffer(const ResourceDesc<ConstBuffer>& desc)
	{
		return _allocator->create_buffer(desc);
	}

	VertexBuffer create_buffer(const ResourceDesc<VertexBuffer>& desc)
	{
		return _allocator->create_buffer(desc);
	}

	IndexBuffer create_buffer(const ResourceDesc<IndexBuffer>& desc)
	{
		return _allocator->create_buffer(desc);
	}

	SRV create_srv(const ResourceObject& resource, const SHADER_RESOURCE_VIEW_DESC& desc)
	{
		return _allocator->create_srv(resource, desc);
	}

	RTV create_rtv(const ResourceObject& resource, const RENDER_TARGET_VIEW_DESC& desc)
	{
		return _allocator->create_rtv(resource, desc);
	}

	DSV create_dsv(const ResourceObject& resource, const DEPTH_STENCIL_VIEW_DESC& desc)
	{
		return _allocator->create_dsv(resource, desc);
	}

	UAV create_uav(const ResourceObject& resource, const UNORDERED_ACCESS_VIEW_DESC& desc)
	{
		return _allocator->create_uav(resource, desc);
	}

	auto create_shader(ShaderStage type, std::string compiled_code) -> Shader
	{
		Shader shader;
		shader.vptr = _device->compile_shader(type, compiled_code);
		shader.set(type, std::move(compiled_code));
		return shader;
	}

	auto create_sampler(const SAMPLER_DESC& desc) -> Sampler
	{
		Sampler sampler;
		sampler.vptr = _device->create_sampler(desc);
		sampler.desc = desc;
		return sampler;
	}

	auto create_vertex_layout(std::vector<VertexElements> vb_descs, std::string_view shader_code) -> VertexLayout
	{
		return _allocator->create_vertex_layout(std::move(vb_descs), shader_code);
	}
}