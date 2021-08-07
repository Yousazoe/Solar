#pragma once
#include<gfx/middle/StateInterface.hpp>

#include<d3d11_1.h>
#include<wrl.h>
#include<map>

namespace tutorial::gfx
{
	using Microsoft::WRL::ComPtr;

	class States final
	{
	public:
		auto get_blend_mode(ID3D11Device* device, BlendMode mode, BlendState blend_hash);
		auto get_depth_test(ID3D11Device* device, DepthState depthtest_hash);
		auto get_rasterize_mode(ID3D11Device* device, PolyCullMode cull_mode, PolyFillMode fill_mode, RasterizeState raster_hash);

	private:
		std::map<uint32 /*hash*/, ComPtr<ID3D11BlendState>> _blendStates;
		std::map<uint32 /*hash*/, ComPtr<ID3D11DepthStencilState>> _depthStates;
		std::map<uint32 /*hash*/, ComPtr<ID3D11RasterizerState>> _rasterizeStates;
	};
	
	inline auto States::get_blend_mode(ID3D11Device* device, BlendMode mode, BlendState blend_hash)
	{
		ComPtr<ID3D11BlendState> blendstate = 0;
		if (_blendStates.find(blend_hash.hash) == _blendStates.end())
		{

			D3D11_BLEND_DESC desc = { 0 };
			desc.AlphaToCoverageEnable = false;
			desc.IndependentBlendEnable = false;

			desc.RenderTarget[0].BlendEnable = true;
			desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

			if (mode == BlendMode::BlendAdd)
				desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
			else
				desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
			desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
			desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			D3D11_BLEND d3dblendfunc[] = {
				D3D11_BLEND_ZERO,
				D3D11_BLEND_ONE,
				D3D11_BLEND_SRC_ALPHA,
				D3D11_BLEND_INV_SRC_ALPHA,
				D3D11_BLEND_DEST_COLOR
			};

			if (blend_hash.blendEnable)
				desc.RenderTarget[0].BlendEnable = true;
			else
				desc.RenderTarget[0].BlendEnable = false;

			desc.RenderTarget[0].SrcBlend = d3dblendfunc[(uint32)blend_hash.srcBlendFunc];
			desc.RenderTarget[0].DestBlend = d3dblendfunc[(uint32)blend_hash.dstBlendFunc];
			device->CreateBlendState(&desc, blendstate.GetAddressOf());
			_blendStates[blend_hash.hash] = blendstate;
		}
		else
			blendstate = _blendStates[blend_hash.hash];
		return blendstate.Get();
	}

	inline auto States::get_depth_test(ID3D11Device* device, DepthState depthtest_hash)
	{
		ComPtr<ID3D11DepthStencilState> state;
		if (_depthStates.find(depthtest_hash.hash) == _depthStates.end())
		{
			D3D11_DEPTH_STENCIL_DESC desc;
			desc.DepthEnable = true;
			desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
			desc.StencilEnable = false;
			desc.StencilReadMask = 0xff;
			desc.StencilWriteMask = 0xff;
			desc.DepthEnable = depthtest_hash.depthTest;
			if (depthtest_hash.depthWrite)
				desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			else
				desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

			//class DepthFunc
			//{
			//	DSS_DEPTHFUNC_LESS_EQUAL = 0,
			//	DSS_DEPTHFUNC_LESS,
			//	DSS_DEPTHFUNC_EQUAL,
			//	DSS_DEPTHFUNC_GREATER,
			//	DSS_DEPTHFUNC_GREATER_EQUAL,
			//	DSS_DEPTHFUNC_ALWAYS
			//};
			/*
			typedef enum D3D11_COMPARISON_FUNC {
			D3D11_COMPARISON_NEVER = 1,
			D3D11_COMPARISON_LESS = 2,
			D3D11_COMPARISON_EQUAL = 3,
			D3D11_COMPARISON_LESS_EQUAL = 4,
			D3D11_COMPARISON_GREATER = 5,
			D3D11_COMPARISON_NOT_EQUAL = 6,
			D3D11_COMPARISON_GREATER_EQUAL = 7,
			D3D11_COMPARISON_ALWAYS = 8
			} D3D11_COMPARISON_FUNC;
			*/
			D3D11_COMPARISON_FUNC d3dcomp[] =
			{
				D3D11_COMPARISON_LESS_EQUAL,
				D3D11_COMPARISON_LESS,
				D3D11_COMPARISON_EQUAL,
				D3D11_COMPARISON_GREATER,
				D3D11_COMPARISON_GREATER_EQUAL,
				D3D11_COMPARISON_ALWAYS
			};

			desc.DepthFunc = d3dcomp[(uint32)depthtest_hash.compareFunc];
			device->CreateDepthStencilState(&desc, state.GetAddressOf());
			_depthStates[depthtest_hash.hash] = state;
		}
		state = _depthStates[depthtest_hash.hash];
		return state.Get();
	}

	inline auto States::get_rasterize_mode(ID3D11Device* device, PolyCullMode cull_mode, PolyFillMode fill_mode, RasterizeState raster_hash)
	{
		ComPtr<ID3D11RasterizerState > state;

		if (_rasterizeStates.find(raster_hash.hash) == _rasterizeStates.end())
		{
			D3D11_RASTERIZER_DESC desc;

			desc.FillMode = D3D11_FILL_SOLID;
			desc.CullMode = D3D11_CULL_BACK;
			desc.FrontCounterClockwise = false;
			desc.DepthBias = 0;
			desc.DepthBiasClamp = 0.f;
			desc.SlopeScaledDepthBias = 0;
			desc.DepthClipEnable = (bool)raster_hash.depthClip;
			desc.ScissorEnable = false;
			desc.MultisampleEnable = false;
			desc.AntialiasedLineEnable = false;

			switch (fill_mode)
			{
			case PolyFillMode::PFM_SOLID:
				desc.FillMode = D3D11_FILL_SOLID;
				break;
			case PolyFillMode::PFM_WIRE:
				desc.FillMode = D3D11_FILL_WIREFRAME;
				break;
			}

			switch (cull_mode)
			{
			case PolyCullMode::PCM_FRONT:
				desc.CullMode = D3D11_CULL_FRONT;
				break;
			case PolyCullMode::PCM_BACK:
				desc.CullMode = D3D11_CULL_BACK;;
				break;
			case PolyCullMode::PCM_NONE:
				desc.CullMode = D3D11_CULL_NONE;
				break;

			}
			//create
			device->CreateRasterizerState(&desc, state.GetAddressOf());
			_rasterizeStates[raster_hash.hash] = state;
		}
		else
			state = _rasterizeStates[raster_hash.hash];
		return state.Get();
	}

}
