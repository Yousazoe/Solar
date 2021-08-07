#pragma once
#include<gfx/entry/GfxObject.hpp>
#include<gfx/middle/IGfxDevice.hpp>
#include<string_view>

namespace tutorial::gfx
{
	struct MemoryDesc
	{
		void *vptr = nullptr;
	};

	template<class GraphicsApi>
	class IGfxAllocator
	{
	public:
		IGfxAllocator(IGfxDevice<GraphicsApi>& device);
		IGfxAllocator(IGfxAllocator&) = delete;
		IGfxAllocator& operator=(IGfxAllocator&) = delete;
		IGfxAllocator(IGfxAllocator&& f) = default;
		IGfxAllocator& operator =(IGfxAllocator&& f) = default;
		~IGfxAllocator() = default;

	public:
		MemoryDesc allocate(uint32 size) { return MemoryDesc(); };
		void deallocate(const MemoryDesc&) {};
		Texture2D create_texture2D(const ResourceDesc<Texture2D>& desc);
		Texture3D create_texture3D(const ResourceDesc<Texture3D>& desc);
		Texture2DArray create_texture2DArray(const ResourceDesc<Texture2DArray>& desc);
		StructureBuffer create_buffer(const ResourceDesc<StructureBuffer>& desc);
		ConstBuffer create_buffer(const ResourceDesc<ConstBuffer>& desc);
		VertexBuffer create_buffer(const ResourceDesc<VertexBuffer>& desc);
		IndexBuffer create_buffer(const ResourceDesc<IndexBuffer>& desc);
		SRV create_srv(const ResourceObject& resource, const SHADER_RESOURCE_VIEW_DESC& desc);
		RTV create_rtv(const ResourceObject& resource, const RENDER_TARGET_VIEW_DESC& desc);
		DSV create_dsv(const ResourceObject& resource, const DEPTH_STENCIL_VIEW_DESC& desc);
		UAV create_uav(const ResourceObject& resource, const UNORDERED_ACCESS_VIEW_DESC& desc);
		VertexLayout create_vertex_layout(std::vector<VertexElements> vb_descs, std::string_view shader_code);
	};
}
