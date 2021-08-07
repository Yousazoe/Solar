#pragma once
#include<gfx/d3d11/DxInterface.hpp>
#include<gfx/entry/Define.hpp>
#include<gfx/middle/IGfxDevice.hpp>
#include<gfx/d3d11/GfxStatesDx11.hpp>

#include<cassert>
#include<string>
#include<d3d11_1.h>
//#include<core/Log.hpp>
#include<stdexcept>

namespace tutorial::gfx
{
	template<>
	class IGfxDevice<D3D> final
	{
	public:
		IGfxDevice(bool debug_msg, bool enable_msaa4x, uint32 msaa4x_quality);
		~IGfxDevice();
		bool setup();
		bool shutdown();
		unsigned getMultiSampleQuality(DXGI_FORMAT format, unsigned sampleCount) const;
		unsigned getMultiSampleQuality(eFormat format, unsigned sampleCount) const;
		ID3D11Device* get_device() const { return _device; }
		ID3D11DeviceContext* get_context() const { return _context; }
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

	private:

	protected:
		bool initDirect3D();

	protected:
		ID3D11Device* _device = nullptr;
		ID3D11DeviceContext* _context = nullptr;
		D3D_DRIVER_TYPE	_driverType = D3D_DRIVER_TYPE_HARDWARE;
		DXGI_FORMAT	_ibFormat = DXGI_FORMAT_R32_UINT;

	public:
		bool _debug_msg;
		bool _enable_msaa4x;
		UINT _msaa4x_quality;

	private:
		States _states;
		BlendState _curBlendState;
		DepthState _curDepthState;
		RasterizeState _curRasterState;
	};
	
	inline IGfxDevice<D3D>::IGfxDevice(bool debug_msg, bool enable_msaa4x, uint32 msaa4x_quality) :
		_debug_msg(debug_msg),
		_enable_msaa4x(enable_msaa4x),
		_msaa4x_quality(msaa4x_quality)
	{

	}

	inline IGfxDevice<D3D>::~IGfxDevice()
	{
	}

	inline bool IGfxDevice<D3D>::setup()
	{
		if (!initDirect3D())
		{
			//g_log_error.log("gfx") << "setup gfx fail";
			return false;
		}
		return true;
	}

	inline bool IGfxDevice<D3D>::shutdown()
	{
		if (_context)
			_context->ClearState();
		GfxRelease(_context);
		GfxRelease(_device);

		return true;
	}

	inline unsigned IGfxDevice<D3D>::getMultiSampleQuality(DXGI_FORMAT format, unsigned sampleCount) const
	{
		if (sampleCount < 2)
			return 0; // Not multisampled, should use quality 0

		if (_device->GetFeatureLevel() >= D3D_FEATURE_LEVEL_10_1)
			return 0xffffffff; // D3D10.1+ standard level

		UINT numLevels = 0;
		if (FAILED(_device->CheckMultisampleQualityLevels(format, sampleCount, &numLevels)) || !numLevels)
			return 0; // Errored or sample count not supported
		else
			return numLevels - 1; // D3D10.0 and below: use the best quality
	}

	inline unsigned IGfxDevice<D3D>::getMultiSampleQuality(gfx::eFormat format, unsigned sampleCount) const
	{
		return 0;
	}

	inline GPUHandle IGfxDevice<D3D>::compile_shader(ShaderStage type, const std::string & bytecode)
	{
		HRESULT hr;
		GPUHandle shader;

		switch (type)
		{
		case ShaderStage::VS:
		{
			ID3D11VertexShader* object = nullptr;
			hr = _device->CreateVertexShader(bytecode.c_str(), bytecode.size(), 0, &object);
			shader = object;
			break;
		}
		case ShaderStage::GS:
		{
			ID3D11GeometryShader* object = nullptr;
			hr = _device->CreateGeometryShader(bytecode.c_str(), bytecode.size(), 0, &object);
			shader = object;
			break;
		}
		case ShaderStage::PS:
		{
			ID3D11PixelShader* object = nullptr;
			hr = _device->CreatePixelShader(bytecode.c_str(), bytecode.size(), 0, &object);
			shader = object;
			break;
		}
		case ShaderStage::CS:
		{
			ID3D11ComputeShader* object = nullptr;
			hr = _device->CreateComputeShader(bytecode.c_str(), bytecode.size(), 0, &object);
			shader = object;
			break;
		}
		}

		if (FAILED(hr))
		{
			throw std::invalid_argument{ "[gfx] ---  compile shader fail" };
		}

		return shader;
	}

	inline GPUHandle IGfxDevice<D3D>::create_sampler(const SAMPLER_DESC& desc)
	{
		GPUHandle sampler;
		D3D11_SAMPLER_DESC samp_desc;
		ZeroMemory(&samp_desc, sizeof(samp_desc));
		samp_desc.Filter = (D3D11_FILTER)desc.Filter;
		samp_desc.AddressU = (D3D11_TEXTURE_ADDRESS_MODE)desc.AddressU;
		samp_desc.AddressV = (D3D11_TEXTURE_ADDRESS_MODE)desc.AddressV;
		samp_desc.AddressW = (D3D11_TEXTURE_ADDRESS_MODE)desc.AddressW;
		samp_desc.ComparisonFunc = (D3D11_COMPARISON_FUNC)desc.ComparisonFunc;
		samp_desc.MinLOD = 0;
		samp_desc.MaxLOD = D3D11_FLOAT32_MAX;
		samp_desc.MaxAnisotropy = /*(filter == D3D11_FILTER_ANISOTROPIC) ? D3D11_REQ_MAXANISOTROPY :*/ 1;
		_device->CreateSamplerState(&samp_desc, (ID3D11SamplerState**)&sampler);
		return sampler;
	}

	inline auto IGfxDevice<D3D>::create_swapchain(void* hwnd, uint32 width, uint32 height) -> Swapchain
	{
		IDXGISwapChain* swapchain = nullptr;
		ID3D11Texture2D* depthbuffer = nullptr;

		DXGI_SWAP_CHAIN_DESC sd;
		sd.BufferDesc.Width = width;
		sd.BufferDesc.Height = height;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		// Use 4X MSAA? 
		if (_enable_msaa4x)
		{
			sd.SampleDesc.Count = 4;
			sd.SampleDesc.Quality = _msaa4x_quality - 1;
		}
		// No MSAA
		else
		{
			sd.SampleDesc.Count = 1;
			sd.SampleDesc.Quality = 0;
		}

		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
		sd.BufferCount = 2;
		sd.OutputWindow = (HWND)hwnd;
		sd.Windowed = true;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = 0;

		IDXGIDevice* dxgiDevice = 0;
		if (FAILED(_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice)))
		{
			throw std::invalid_argument{
				"[gfx][error] --- create swapchain fail, _device->QueryInterface"
			};
		}

		IDXGIAdapter* dxgiAdapter = 0;
		if (FAILED(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter)))
		{
			throw std::invalid_argument{
				"[gfx][error] --- create swapchain fail, dxgiDevice->dxgiAdapter"
			};
		}

		IDXGIFactory* dxgiFactory = 0;
		if (FAILED((dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory))))
		{
			throw std::invalid_argument{
				"[gfx][error] --- create swapchain fail, dxgiDevice->dxgiFactory"
			};
		}
		if (FAILED(dxgiFactory->CreateSwapChain(_device, &sd, &swapchain)))
		{
			throw std::invalid_argument{
				"[gfx][error] --- create swapchain fail, CreateSwapChain"
			};
		}

		GfxRelease(dxgiDevice);
		GfxRelease(dxgiAdapter);
		GfxRelease(dxgiFactory);

		_context->ClearState();
		swapchain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
		ID3D11Texture2D* backbuffer = nullptr;
		ID3D11RenderTargetView* rtv = nullptr;
		swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backbuffer));
		_device->CreateRenderTargetView(backbuffer, 0, &rtv);

		ID3D11DepthStencilView* dsv = nullptr;
		D3D11_TEXTURE2D_DESC depthstencil_desc;
		depthstencil_desc.Width = width;
		depthstencil_desc.Height = height;
		depthstencil_desc.MipLevels = 1;
		depthstencil_desc.ArraySize = 1;
		depthstencil_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		// Use 4X MSAA? --must match swap chain MSAA values.
		if (_enable_msaa4x)
		{
			depthstencil_desc.SampleDesc.Count = 4;
			depthstencil_desc.SampleDesc.Quality = _msaa4x_quality - 1;
		}
		// No MSAA
		else
		{
			depthstencil_desc.SampleDesc.Count = 1;
			depthstencil_desc.SampleDesc.Quality = 0;
		}
		depthstencil_desc.Usage = D3D11_USAGE_DEFAULT;
		depthstencil_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthstencil_desc.CPUAccessFlags = 0;
		depthstencil_desc.MiscFlags = 0;
		if (FAILED(_device->CreateTexture2D(&depthstencil_desc, 0, &depthbuffer)))
		{
			throw std::invalid_argument{
				"[gfx][error] --- create swapchain fail, Create depthbuffer fail"
			};
		}

		if (FAILED(_device->CreateDepthStencilView(depthbuffer, 0, &dsv)))
		{
			throw std::invalid_argument{
				"[gfx][error] --- create swapchain fail, Create dsv fail"
			};
		}

		return Swapchain{
			width, height, hwnd,
			std::move(rtv), std::move(dsv), swapchain, depthbuffer
		};
	}

	inline bool IGfxDevice<D3D>::initDirect3D()
	{
		// Create the device and device context.
		UINT createDeviceFlags = 0;
		if (_debug_msg)
			createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
		D3D_FEATURE_LEVEL featureLevel;
		HRESULT hr = D3D11CreateDevice(
			0,                 // default adapter
			_driverType,
			0,                 // no software device
			createDeviceFlags,
			0, 0,              // default feature level array
			D3D11_SDK_VERSION,
			&_device,
			&featureLevel,
			&_context);

		if (FAILED(hr))
		{
			throw std::invalid_argument{ "D3D11CreateDevice Failed." };
		}

		if (featureLevel != D3D_FEATURE_LEVEL_11_0)
		{
			throw std::invalid_argument{ "Direct3D Feature Level 11 unsupported." };
		}

		_device->CheckMultisampleQualityLevels(
			DXGI_FORMAT_R8G8B8A8_UNORM, 4, &_msaa4x_quality);
		assert(_msaa4x_quality > 0);

		return true;
	}

	inline void IGfxDevice<D3D>::clear_rtv(
		const RTV& rtv,
		const float color[4]
	) const
	{
		_context->ClearRenderTargetView(
			static_cast<ID3D11RenderTargetView*>(rtv.vptr),
			reinterpret_cast<const float*>(color)
		);
	}

	inline void IGfxDevice<D3D>::clear_dsv(
		const DSV& dsv,
		float depth
	) const
	{
		_context->ClearDepthStencilView(
			static_cast<ID3D11DepthStencilView*>(dsv.vptr),
			D3D11_CLEAR_DEPTH,
			depth,
			0
		);
	}

	inline void IGfxDevice<D3D>::copy_texture2D(
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
	)
	{
		const auto dst_subresource = D3D11CalcSubresource(dst_mip, 0, dst_texture.desc.MipLevels);
		const auto src_subresource = D3D11CalcSubresource(src_mip, 0, src_texture.desc.MipLevels);
		D3D11_BOX src_box;
		src_box.left = UINT(src_left);
		src_box.right = UINT(src_right);
		src_box.top = UINT(src_top);
		src_box.bottom = UINT(src_bottom);
		src_box.front = 0;
		src_box.back = 1;
		_context->CopySubresourceRegion(
			static_cast<ID3D11Resource*>(dst_texture.vptr),
			dst_subresource,
			UINT(dst_x),
			UINT(dst_y),
			0u,
			static_cast<ID3D11Resource*>(src_texture.vptr),
			src_subresource,
			&src_box
		);
	}

	inline void IGfxDevice<D3D>::copy_texture2D(
		const SubResource& dst,
		const uint16 dst_x,
		const uint16 dst_y,
		const SubResource& src,
		const uint16 src_left,
		const uint16 src_top,
		const uint16 src_right,
		const uint16 src_bottom
	)
	{
		const auto dst_subresource = D3D11CalcSubresource(dst.mip_level, dst.array_index, dst.mip_level_count);
		const auto src_subresource = D3D11CalcSubresource(src.mip_level, src.array_index, src.mip_level_count);
		D3D11_BOX src_box;
		src_box.left = UINT(src_left);
		src_box.right = UINT(src_right);
		src_box.top = UINT(src_top);
		src_box.bottom = UINT(src_bottom);
		src_box.front = 0;
		src_box.back = 1;
		_context->CopySubresourceRegion(
			static_cast<ID3D11Resource*>(dst.resource.vptr),
			dst_subresource,
			UINT(dst_x),
			UINT(dst_y),
			0u,
			static_cast<ID3D11Resource*>(src.resource.vptr),
			src_subresource,
			&src_box
		);
	}

	inline void IGfxDevice<D3D>::upload_data(
		gfx::ResourceObject& resouce,
		uint32 start_byte,
		uint32 end_byte,
		const void* data,
		uint32 row_pitch
	)
	{
		D3D11_BOX dest;
		dest.left = start_byte;
		dest.right = end_byte;
		dest.top = 0;
		dest.bottom = 1;
		dest.front = 0;
		dest.back = 1;
		_context->UpdateSubresource(
			static_cast<ID3D11Resource*>(resouce.vptr),
			0,
			&dest,
			data,
			row_pitch,
			0
		);
	}

	inline void IGfxDevice<D3D>::upload_texture(
		SubResource& texture,
		uint32 left,
		uint32 right,
		uint32 top,
		uint32 bottom,
		const void* data,
		uint32 src_row_pitch
	)
	{
		D3D11_BOX dest;
		dest.left = left;
		dest.right = right;
		dest.top = top;
		dest.bottom = bottom;
		dest.front = 0;
		dest.back = 1;
		const auto subresource = D3D11CalcSubresource(
			texture.mip_level,
			texture.array_index,
			texture.mip_level_count
		);
		_context->UpdateSubresource(
			static_cast<ID3D11Resource*>(texture.resource.vptr),
			subresource,
			&dest,
			data,
			src_row_pitch,
			0
		);
	}

	inline void IGfxDevice<D3D>::upload_texture(
		Texture2D& texture,
		uint8 mip,
		uint32 left,
		uint32 right,
		uint32 top,
		uint32 bottom,
		const void* data,
		uint32 src_row_pitch
	)
	{
		D3D11_BOX dest;
		dest.left = left;
		dest.right = right;
		dest.top = top;
		dest.bottom = bottom;
		dest.front = 0;
		dest.back = 1;
		const auto subresource = D3D11CalcSubresource(mip, 0, texture.desc.MipLevels);
		_context->UpdateSubresource(
			static_cast<ID3D11Resource*>(texture.vptr),
			subresource,
			&dest,
			data,
			src_row_pitch,
			0
		);
	}

	inline void IGfxDevice<D3D>::upload_texture(
		Texture2DArray& texture,
		uint8 array_index,
		uint8 mip,
		uint32 left,
		uint32 right,
		uint32 top,
		uint32 bottom,
		const void* data,
		uint32 src_row_pitch
	)
	{
		D3D11_BOX dest;
		dest.left = left;
		dest.right = right;
		dest.top = top;
		dest.bottom = bottom;
		dest.front = 0;
		dest.back = 1;
		const auto subresource = D3D11CalcSubresource(mip, array_index, texture.desc.MipLevels);
		_context->UpdateSubresource(
			static_cast<ID3D11Resource*>(texture.vptr),
			subresource,
			&dest,
			data,
			src_row_pitch,
			0
		);
	}

	inline void IGfxDevice<D3D>::download_texture(
		Texture2D& texture,
		uint8 mip,
		void* buffer
	)
	{
		if (!buffer) {
			return;
		}

		assert(
			(texture.desc.CPUAccessFlags & gfx::eCpuAccessFlag::CPU_ACCESS_READ) == gfx::eCpuAccessFlag::CPU_ACCESS_READ
			&& "texture must be created with staging"
		);
		assert(texture.desc.MipLevels == 1u && "must be 2D texture");

		D3D11_MAPPED_SUBRESOURCE mapped_resource;
		const auto subresource = D3D11CalcSubresource(mip, 0, texture.desc.MipLevels);
		_context->Map(
			static_cast<ID3D11Resource*>(texture.vptr),
			subresource,
			D3D11_MAP_READ,
			0,
			&mapped_resource
		);

		auto size = texture.height() * mapped_resource.RowPitch;
		memcpy(buffer, mapped_resource.pData, size);
		_context->Unmap(static_cast<ID3D11Resource*>(texture.vptr), subresource);
	}

	inline void IGfxDevice<D3D>::upload_dynamic_data(
		const ResourceObject& resouce,
		const void* data,
		uint32 size
	)
	{
		D3D11_MAPPED_SUBRESOURCE mapped_data;
		mapped_data.pData = nullptr;
		HRESULT hr = _context->Map(
			(ID3D11Buffer*)resouce.vptr,
			0,
			D3D11_MAP_WRITE_DISCARD,
			0,
			&mapped_data
		);
		if (mapped_data.pData) {
			memcpy(mapped_data.pData, data, size);
			_context->Unmap((ID3D11Buffer*)resouce.vptr, 0);
		}
	}

	inline void IGfxDevice<D3D>::set_primitive_type(PrimType type)
	{
		_context->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)type);
	}

	inline void IGfxDevice<D3D>::set_blend_mode(BlendMode mode)
	{
		BlendState blend_hash = IGfxStates<void>::hash_blend_mode(mode);
		if (blend_hash.hash != _curBlendState.hash)
		{
			auto state = _states.get_blend_mode(_device, mode, blend_hash);
			float blendfactor[] = { 0, 0, 0, 0 };
			_context->OMSetBlendState(state, blendfactor, 0xffffffff);
			_curBlendState = blend_hash;
		}
	}

	inline void IGfxDevice<D3D>::set_depth_test(bool depth_test, bool depth_write, DepthFunc func)
	{
		auto depthtest_hash = IGfxStates<void>::hash_depth_test(depth_test, depth_write, func);
		if (_curDepthState.hash != depthtest_hash.hash)
		{
			auto state = _states.get_depth_test(_device, depthtest_hash);
			_context->OMSetDepthStencilState(state, 0xff);
			_curDepthState = depthtest_hash;
		}
	}

	inline void IGfxDevice<D3D>::set_rasterize_mode(PolyCullMode cull_mode, PolyFillMode fill_mode, bool depth_clip)
	{
		auto newstate = IGfxStates<void>::hash_rasterize_mode((uint32)cull_mode, (uint32)fill_mode, depth_clip);
		if (newstate.hash != _curRasterState.hash)
		{
			auto state = _states.get_rasterize_mode(_device, cull_mode, fill_mode, newstate );
			_context->RSSetState(state);
			_curRasterState = newstate;
		}
	}

	inline void IGfxDevice<D3D>::set_viewport(
		const Viewport viewports[],
		uint8 count
	)
	{
		D3D11_VIEWPORT vp[16];
		for (auto i = 0u; i < count; ++i)
		{
			vp[i].TopLeftX = static_cast<float>(viewports[i].x);
			vp[i].TopLeftY = static_cast<float>(viewports[i].y);
			vp[i].Width = static_cast<float>(viewports[i].width);
			vp[i].Height = static_cast<float>(viewports[i].height);
			vp[i].MinDepth = 0.0f;
			vp[i].MaxDepth = 1.0f;
		}
		_context->RSSetViewports(count, vp);
	}

	template<class D, class ...R>
	inline void IGfxDevice<D3D>::bind_framebuffer(
		const gfx::FrameBuffer<D, R...>& buffer
	)
	{
		auto dx11_rtvs = buffer.rtvs_as<ID3D11RenderTargetView*>();
		auto dx11_dsv = buffer.dsv_as<ID3D11DepthStencilView*>();
		_context->OMSetRenderTargets(sizeof...(R), &dx11_rtvs[0], dx11_dsv);
	}

	template<class D, class ...R>
	inline void IGfxDevice<D3D>::unbind_framebuffer(
		const gfx::FrameBuffer<D, R...>& buffer
	)
	{
		constexpr ID3D11RenderTargetView* dx11_rtvs[sizeof...(R)] = { nullptr };
		constexpr ID3D11DepthStencilView* dx11_dsv = nullptr;
		_context->OMSetRenderTargets(sizeof...(R), &dx11_rtvs[0], dx11_dsv);
	}

	template<class ...U>
	inline void IGfxDevice<D3D>::bind_computebuffer(
		const gfx::ShaderStage stage,
		const gfx::ComputeBuffer<U...>& buffer
	)
	{
		constexpr ID3D11RenderTargetView* dx11_rtvs[] = { nullptr };
		constexpr ID3D11DepthStencilView* dx11_dsv = nullptr;
		auto dx11_uavs = buffer.uavs_as<ID3D11UnorderedAccessView*>();
		constexpr auto uav_count = sizeof...(U);
		UINT initUavValue = 0;
		if (stage == gfx::ShaderStage::CS)
		{
			_context->CSSetUnorderedAccessViews(
				0,
				uav_count,
				&dx11_uavs[0],
				&initUavValue
			);
		}
		else if (stage == gfx::ShaderStage::PS)
		{
			_context->OMSetRenderTargetsAndUnorderedAccessViews(
				0,
				&dx11_rtvs[0],
				dx11_dsv,
				0,
				uav_count,
				&dx11_uavs[0],
				&initUavValue
			);
		}
		else {
			assert(false);
		}
	}

	template<class ...U>
	inline void IGfxDevice<D3D>::unbind_computebuffer(
		const gfx::ShaderStage stage,
		const gfx::ComputeBuffer<U...>& buffer
	)
	{
		ID3D11UnorderedAccessView* dx11_uavs[sizeof...(U)] = { nullptr };
		UINT initUavValue = 0;
		if (stage == gfx::ShaderStage::CS) {
			_context->CSSetUnorderedAccessViews(
				0,
				sizeof...(U),
				&dx11_uavs[0],
				&initUavValue
			);
		}
		else if (stage == gfx::ShaderStage::PS) {
			_context->OMSetRenderTargetsAndUnorderedAccessViews(
				0,
				nullptr,
				nullptr,
				0,
				sizeof...(U),
				&dx11_uavs[0],
				&initUavValue
			);
		}
	}

	namespace detail
	{
		template<typename SrcType, typename DstType, size_t N, size_t ...I>
		std::array<DstType, N>
			array_cast_impl(const std::array<SrcType, N>& values,
				std::index_sequence<I...>&&)
		{
			auto helper = [](const SrcType& v) {
				return static_cast<DstType>(v);
			};
			return { (helper(std::get<I>(values)))... };
		}

		template<typename SrcType, typename DstType, size_t N>
		auto array_cast(const std::array<SrcType, N>& values)
		{
			using Indices = std::make_index_sequence<N>;
			return array_cast_impl<SrcType, DstType, N>(values, Indices{});
		}
	}

	inline void IGfxDevice<D3D>::bind_shader(const ShaderStage stage, const Shader& shader)
	{
		switch (stage)
		{
		case ShaderStage::VS:
			_context->VSSetShader(static_cast<ID3D11VertexShader*>(shader.vptr), 0, 0);
			break;
		case ShaderStage::GS:
			_context->GSSetShader(static_cast<ID3D11GeometryShader*>(shader.vptr), 0, 0);
			break;
		case ShaderStage::PS:
			_context->PSSetShader(static_cast<ID3D11PixelShader*>(shader.vptr), 0, 0);
			break;
		case ShaderStage::CS:
			_context->CSSetShader(static_cast<ID3D11ComputeShader*>(shader.vptr), 0, 0);
			break;
		}
	}

	inline void IGfxDevice<D3D>::bind_vertex_buffer(
		uint8 stream, const VertexBuffer& vb, const VertexLayout& layout
	)
	{
		const auto stream_count = 1u;
		if (layout.valid()) {
			ID3D11Buffer* vbs[] = { static_cast<ID3D11Buffer*>(vb.vptr) };
			UINT offsets[stream_count] = { 0 };
			_context->IASetVertexBuffers(
				stream,
				stream_count,
				&vbs[0],
				&layout.stream_size[0],
				offsets
			);
			_context->IASetInputLayout(static_cast<ID3D11InputLayout*>(layout.vptr));
		}
		else {
			static ID3D11Buffer* vbs[] = { nullptr };
			static UINT offsets[stream_count] = { 0 };
			static UINT strides[stream_count] = { 0 };
			_context->IASetVertexBuffers(
				stream,
				stream_count,
				&vbs[0],
				strides,
				offsets
			);
			_context->IASetInputLayout(
				static_cast<ID3D11InputLayout*>(layout.vptr)
			);
		}
	}

	inline void IGfxDevice<D3D>::bind_index_buffer(const IndexBuffer& ib)
	{
		_context->IASetIndexBuffer(static_cast<ID3D11Buffer*>(ib.vptr), DXGI_FORMAT_R32_UINT, 0);
	}

	inline void IGfxDevice<D3D>::bind_const_buffer(
		uint8 stream,
		ShaderStage stage,
		const ConstBuffer& cb
	)
	{
		ID3D11Buffer* cbs[] = { static_cast<ID3D11Buffer*>(cb.vptr) };
		if (stage == ShaderStage::VS) {
			_context->VSSetConstantBuffers(stream, 1, cbs);
		} else if (stage == ShaderStage::GS) {
			_context->GSSetConstantBuffers(stream, 1, cbs);
		} else if (stage == ShaderStage::PS) {
			_context->PSSetConstantBuffers(stream, 1, cbs);
		} else if (stage == ShaderStage::CS) {
			_context->CSSetConstantBuffers(stream, 1, cbs);
		} else {
			assert(false);
		}
	}

	inline void IGfxDevice<D3D>::bind_sampler(
		uint8 stream,
		ShaderStage stage,
		const Sampler& sampler
	)
	{
		auto sampler_dx11 = static_cast<ID3D11SamplerState*>(sampler.vptr);
		if (stage == ShaderStage::VS) {
			_context->VSSetSamplers(stream, 1, &sampler_dx11);
		}
		else if (stage == ShaderStage::PS) {
			_context->PSSetSamplers(stream, 1, &sampler_dx11);
		}
		else if (stage == ShaderStage::CS) {
			_context->CSSetSamplers(stream, 1, &sampler_dx11);
		}
		else {
			assert(false);
		}
	}

	inline void IGfxDevice<D3D>::bind_srv(
		uint8 stream,
		ShaderStage stage,
		const SRV& srv
	)
	{
		ID3D11ShaderResourceView* srv_dx11[] = {
			static_cast<ID3D11ShaderResourceView*>(srv.vptr)
		};
		if (stage == ShaderStage::VS) {
			_context->VSSetShaderResources(stream, 1, &srv_dx11[0]);
		}
		else if (stage == ShaderStage::GS) {
			_context->GSSetShaderResources(stream, 1, &srv_dx11[0]);
		}
		else if (stage == ShaderStage::PS) {
			_context->PSSetShaderResources(stream, 1, &srv_dx11[0]);
		}
		else if (stage == ShaderStage::CS) {
			_context->CSSetShaderResources(stream, 1, &srv_dx11[0]);
		}
		else {
			assert(false);
		}
	}

	inline void IGfxDevice<D3D>::draw(
		uint32 vert_count,
		uint32 start_vert
	)
	{
		_context->Draw(vert_count, start_vert);
	}

	inline void IGfxDevice<D3D>::draw_indexed(
		uint32 index_count,
		uint32 start_index,
		uint32 start_vert
	)
	{
		_context->DrawIndexed(index_count, start_index, start_vert);
	}

	inline void IGfxDevice<D3D>::draw_indexed_instanced(
		uint32 index_count,
		uint32 instance_count,
		uint32 start_index,
		uint32 start_vert,
		uint32 start_instance
	)
	{
		_context->DrawIndexedInstanced(
			index_count,
			instance_count,
			start_index,
			start_vert,
			start_instance
		);
	}

	inline void IGfxDevice<D3D>::dispatch(
		uint32 group_x,
		uint32 group_y,
		uint32 group_z
	)
	{
		_context->Dispatch(group_x, group_y, group_z);
	}

	inline auto IGfxDevice<D3D>::present(const Swapchain& swapchain) -> void
	{
		IDXGISwapChain* d3d_swapchain = static_cast<IDXGISwapChain*>(swapchain.swapchain);
		d3d_swapchain->Present(0, 0);
	}

}