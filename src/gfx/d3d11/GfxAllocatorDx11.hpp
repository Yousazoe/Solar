#pragma once
#include <d3d11_1.h>
//#include<core/Log.hpp>
#include<gfx/d3d11/DxInterface.hpp>
#include<gfx/d3d11/GfxDeviceDx11.hpp>
#include<gfx//middle/IGfxAllocator.hpp>

namespace tutorial::gfx
{
	template<>
	class IGfxAllocator<D3D> final
	{
	public:
		IGfxAllocator(IGfxDevice<D3D>& device) : _device(static_cast<ID3D11Device*>(device.get_device())) {}
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

	private:
		ID3D11Device* _device;
	};

	inline Texture2D IGfxAllocator<D3D>::create_texture2D(const ResourceDesc<Texture2D>& desc)
	{
		Texture2D resource;
		D3D11_TEXTURE2D_DESC texture_desc;
		static_assert(sizeof std::decay<decltype(desc)>::type == sizeof D3D11_TEXTURE2D_DESC, 
			"[IGfxAllocator][create_texture2D] --- D3D11_TEXTURE2D_DESC not fit");
		std::memcpy(&texture_desc, &desc, sizeof(desc));

		ID3D11Texture2D* resource_object = nullptr;
		auto hr = _device->CreateTexture2D(&texture_desc, 0, &resource_object);
		if (FAILED(hr))
		{
			//g_log_error.log("gfx") << "[IGfxAllocator][create_texture2D] fail !!!";
			GfxRelease(resource_object);
		}

		resource.vptr = resource_object;
		resource.desc = desc;

		return resource;
	}

	inline Texture3D IGfxAllocator<D3D>::create_texture3D(const ResourceDesc<Texture3D>& desc)
	{
		Texture3D resource;
		D3D11_TEXTURE3D_DESC texture_desc;
		static_assert(sizeof std::decay<decltype(desc)>::type == sizeof D3D11_TEXTURE3D_DESC,
			"[IGfxAllocator][create_texture3D] --- D3D11_TEXTURE3D_DESC not fit");
		std::memcpy(&texture_desc, &desc, sizeof(desc));

		ID3D11Texture3D* resource_object = nullptr;
		HRESULT hr = _device->CreateTexture3D(&texture_desc, 0, &resource_object);
		if (FAILED(hr))
		{
			//g_log_error.log("gfx") << "[IGfxAllocator][create_texture3D] fail !!!";
			GfxRelease(resource_object);
		}

		resource.vptr = resource_object;
		resource.desc = desc;

		return resource;
	}

	inline Texture2DArray IGfxAllocator<D3D>::create_texture2DArray(const ResourceDesc<Texture2DArray>& desc)
	{
		Texture2DArray resource;
		D3D11_TEXTURE2D_DESC texture_desc;
		static_assert(sizeof std::decay<decltype(desc)>::type == sizeof D3D11_TEXTURE2D_DESC,
			"[IGfxAllocator][create_texture2DArray] --- D3D11_TEXTURE2D_DESC not fit");
		std::memcpy(&texture_desc, &desc, sizeof(desc));

		ID3D11Texture2D* resource_object = nullptr;
		auto hr = _device->CreateTexture2D(&texture_desc, 0, &resource_object);
		if (FAILED(hr))
		{
			//g_log_error.log("gfx") << "[IGfxAllocator][create_texture2DArray] fail !!!";
			GfxRelease(resource_object);
		}

		resource.vptr = resource_object;
		resource.desc = desc;

		return resource;
	}

	inline StructureBuffer IGfxAllocator<D3D>::create_buffer(const ResourceDesc<StructureBuffer>& desc)
	{
		StructureBuffer resource;
		D3D11_BUFFER_DESC buffer_desc;
		static_assert(sizeof std::decay<decltype(desc)>::type == sizeof D3D11_BUFFER_DESC,
			"[IGfxAllocator][create_buffer] --- D3D11_BUFFER_DESC not fit");
		std::memcpy(&buffer_desc, &desc, sizeof(desc));

		ID3D11Buffer* resource_object = nullptr;
		auto hr = _device->CreateBuffer(&buffer_desc, 0, &resource_object);
		if (FAILED(hr))
		{
			//g_log_error.log("gfx") << "[IGfxAllocator][create_buffer] fail !!!";
			GfxRelease(resource_object);
		}

		resource.vptr = resource_object;
		resource.desc = desc;

		return resource;
	}

	inline ConstBuffer IGfxAllocator<D3D>::create_buffer(const ResourceDesc<ConstBuffer>& desc)
	{
		ConstBuffer resource;
		D3D11_BUFFER_DESC buffer_desc;
		buffer_desc.ByteWidth = (UINT)desc.ByteWidth;
		buffer_desc.Usage = (D3D11_USAGE)desc.Usage;
		buffer_desc.CPUAccessFlags = (UINT)desc.CPUAccessFlags;
		buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;

		ID3D11Buffer* buffer_vptr = nullptr;
		auto hr = _device->CreateBuffer(&buffer_desc, 0, &buffer_vptr);
		if (FAILED(hr))
		{
			//g_log_error.log("gfx") << "[IGfxAllocator][create const buffer] fail !!!";
			GfxRelease(buffer_vptr);
		}

		resource.vptr = buffer_vptr;
		resource.desc = desc;

		return resource;
	}

	inline VertexBuffer IGfxAllocator<D3D>::create_buffer(const ResourceDesc<VertexBuffer>& desc)
	{
		VertexBuffer resource;
		D3D11_BUFFER_DESC buffer_desc;
		buffer_desc.ByteWidth = (UINT)desc.ByteWidth;
		buffer_desc.Usage = (D3D11_USAGE)desc.Usage;
		buffer_desc.CPUAccessFlags = (UINT)desc.CPUAccessFlags;
		buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;

		ID3D11Buffer* buffer_vptr = nullptr;
		auto hr = _device->CreateBuffer(&buffer_desc, 0, &buffer_vptr);
		if (FAILED(hr))
		{
			//g_log_error.log("gfx") << "[IGfxAllocator][create vertex buffer] fail !!!";
			GfxRelease(buffer_vptr);
		}

		resource.vptr = buffer_vptr;
		resource.desc = desc;

		return resource;
	}

	inline IndexBuffer IGfxAllocator<D3D>::create_buffer(const ResourceDesc<IndexBuffer>& desc)
	{
		IndexBuffer resource;
		D3D11_BUFFER_DESC buffer_desc;
		buffer_desc.ByteWidth = (UINT)desc.ByteWidth;
		buffer_desc.Usage = (D3D11_USAGE)desc.Usage;
		buffer_desc.CPUAccessFlags = (UINT)desc.CPUAccessFlags;
		buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;

		ID3D11Buffer* buffer_vptr = nullptr;
		auto hr = _device->CreateBuffer(&buffer_desc, 0, &buffer_vptr);
		if (FAILED(hr))
		{
			//g_log_error.log("gfx") << "[IGfxAllocator][create index buffer] fail !!!";
			GfxRelease(buffer_vptr);
		}

		resource.vptr = buffer_vptr;
		resource.desc = desc;

		return resource;
	}
	
	inline SRV IGfxAllocator<D3D>::create_srv(const ResourceObject& resource, const SHADER_RESOURCE_VIEW_DESC& desc)
	{
		SRV view;

		if (resource.valid())
		{
			static_assert(sizeof SHADER_RESOURCE_VIEW_DESC == sizeof D3D11_SHADER_RESOURCE_VIEW_DESC,
				"[IGfxAllocator][create_srv] --- D3D11_SHADER_RESOURCE_VIEW_DESC not fit");
			D3D11_SHADER_RESOURCE_VIEW_DESC view_desc;
			memcpy(&view_desc, &desc, sizeof desc);

			ID3D11Resource* resource_object = static_cast<ID3D11Resource*>(resource.vptr);
			ID3D11ShaderResourceView* view_object = nullptr;
			auto hr = _device->CreateShaderResourceView(resource_object, &view_desc, &view_object);
			if (FAILED(hr))
			{
				//g_log_error.log("gfx") << "[IGfxAllocator][create_srv] fail !!! ";
				GfxRelease(view_object);
			}

			view.vptr = view_object;
		}

		return view;
	}
	
	inline RTV IGfxAllocator<D3D>::create_rtv(const ResourceObject& resource, const RENDER_TARGET_VIEW_DESC& desc)
	{
		RTV view;

		if (resource.valid())
		{
			static_assert(sizeof D3D11_RENDER_TARGET_VIEW_DESC == sizeof RENDER_TARGET_VIEW_DESC,
				"[IGfxAllocator][create_rtv] --- D3D11_RENDER_TARGET_VIEW_DESC not fit");
			D3D11_RENDER_TARGET_VIEW_DESC view_desc;
			memcpy(&view_desc, &desc, sizeof desc);

			ID3D11Resource* resource_object = static_cast<ID3D11Resource*>(resource.vptr);
			ID3D11RenderTargetView* view_object = nullptr;
			auto hr = _device->CreateRenderTargetView(resource_object, &view_desc, &view_object);
			if (FAILED(hr))
			{
				//g_log_error.log("gfx") << "[IGfxAllocator][create_rtv] fail !!! ";
				GfxRelease(view_object);
			}

			view.vptr = view_object;
		}

		return view;
	}
	
	inline DSV IGfxAllocator<D3D>::create_dsv(const ResourceObject& resource, const DEPTH_STENCIL_VIEW_DESC& desc)
	{
		DSV view;

		if (resource.valid())
		{
			static_assert(sizeof D3D11_DEPTH_STENCIL_VIEW_DESC == sizeof DEPTH_STENCIL_VIEW_DESC,
				"[IGfxAllocator][create_dsv] --- D3D11_DEPTH_STENCIL_VIEW_DESC not fit");
			D3D11_DEPTH_STENCIL_VIEW_DESC view_desc;
			memcpy(&view_desc, &desc, sizeof desc);

			ID3D11Resource* resource_object = static_cast<ID3D11Resource*>(resource.vptr);
			ID3D11DepthStencilView* view_object = nullptr;
			auto hr = _device->CreateDepthStencilView(resource_object, &view_desc, &view_object);
			if (FAILED(hr))
			{
				//g_log_error.log("gfx") << "[IGfxAllocator][create_dsv] fail !!! ";
				GfxRelease(view_object);
			}

			view.vptr = view_object;
		}

		return view;
	}

	inline UAV IGfxAllocator<D3D>::create_uav(const ResourceObject& resource, const UNORDERED_ACCESS_VIEW_DESC& desc)
	{
		UAV view;

		if (resource.valid())
		{
			static_assert(sizeof D3D11_UNORDERED_ACCESS_VIEW_DESC == sizeof UNORDERED_ACCESS_VIEW_DESC,
				"[IGfxAllocator][create_uav] --- D3D11_UNORDERED_ACCESS_VIEW_DESC not fit");
			D3D11_UNORDERED_ACCESS_VIEW_DESC view_desc;
			memcpy(&view_desc, &desc, sizeof desc);

			ID3D11Resource* resource_object = static_cast<ID3D11Resource*>(resource.vptr);
			ID3D11UnorderedAccessView* view_object = nullptr;
			auto hr = _device->CreateUnorderedAccessView(resource_object, &view_desc, &view_object);
			if (FAILED(hr))
			{
				//g_log_error.log("gfx") << "[IGfxAllocator][create_uav] fail !!! ";
				GfxRelease(view_object);
			}

			view.vptr = view_object;
		}

		return view;
	}

	inline VertexLayout IGfxAllocator<D3D>::create_vertex_layout(std::vector<VertexElements> vb_descs, std::string_view shader_code)
	{
		VertexLayout layout;

		if (vb_descs.empty())
		{
			return layout;
		}

		static const DXGI_FORMAT type_to_format[] =
		{
			DXGI_FORMAT_R32_SINT,
			DXGI_FORMAT_R32_FLOAT,
			DXGI_FORMAT_R32G32_FLOAT,
			DXGI_FORMAT_R32G32B32_FLOAT,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_R8G8B8A8_UINT,
			DXGI_FORMAT_R8G8B8A8_UNORM
		};

		std::vector<D3D11_INPUT_ELEMENT_DESC> descs;
		std::vector<uint32> stream_size;
		for (const auto& vert_elements : vb_descs)
		{
			for (const auto& vert_element : vert_elements.elements)
			{
				D3D11_INPUT_ELEMENT_DESC desc;
				desc.SemanticName = vert_element.semantic;
				desc.SemanticIndex = vert_element.index;
				desc.Format = type_to_format[(int)vert_element.type];
				desc.InputSlot = vert_elements.stream;
				desc.AlignedByteOffset = vert_element.offset;
				desc.InputSlotClass = vert_element.per_instance ? D3D11_INPUT_PER_INSTANCE_DATA : D3D11_INPUT_PER_VERTEX_DATA;
				desc.InstanceDataStepRate = vert_element.per_instance ? 1 : 0;
				descs.push_back(desc);
			}
			assert(vert_elements.vertex_size < (1 << 8));
			stream_size.push_back(vert_elements.vertex_size);
		}

		ID3D11InputLayout* layout_object = nullptr;
		HRESULT hr = _device->CreateInputLayout(&descs[0], (UINT)descs.size(), shader_code.data(), shader_code.size(), &layout_object);
		if (FAILED(hr))
		{
			//g_log_error.log("gfx") << "[IGfxAllocator][create_vertex_layout] fail !!! ";
			GfxRelease(layout_object);
		}

		layout.vptr = layout_object;
		layout.set(std::move(stream_size));

		return layout;
	}

}