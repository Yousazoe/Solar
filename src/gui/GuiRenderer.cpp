// https://github.com/ocornut/imgui
#include<../thirdparty/imgui/imgui.h>
#include<gui/GuiRenderer.hpp>
//#include<core/Log.hpp>
#include<iostream>

namespace tutorial::gui
{
	gfx::UniformHandle GuiRenderer::_main_tex_handle;
	gfx::UniformHandle GuiRenderer::_proj_mat_handle;
	
	void GuiRenderer::invalidate_device_objects()
	{
	}

	void GuiRenderer::setup_render_states(ImDrawData* draw_data)
	{
		gfx::Viewport vp[] = { gfx::Viewport{ 0, 0, uint32(draw_data->DisplaySize.x), uint32(draw_data->DisplaySize.y) } };
		gfx::set_viewport(vp, 1);
		gfx::bind_vertex_buffer(_vb, _vert_layout);
		gfx::bind_index_buffer(_ib);
		gfx::set_primitive_type(gfx::PrimType::PRIM_TRILIST);
		gfx::bind_shader(_vs, gfx::empty_v<gfx::Shader>, _ps, _binding);
		gfx::set_blend_mode(gfx::BlendMode::AlphaBlend);
		gfx::set_depth_test(false, false, gfx::DepthFunc::DSS_DEPTHFUNC_ALWAYS);
		gfx::set_rasterize_mode(gfx::PolyCullMode::PCM_NONE, gfx::PolyFillMode::PFM_SOLID, true);
	}
	
	void GuiRenderer::render_draw_data(ImDrawData* draw_data)
	{
		if (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f)
			return;

		if (!_vb.valid() || _VertexBufferSize < draw_data->TotalVtxCount)
		{
			if (_vb.valid()) { /*_pVB->Release();*/  }
			_VertexBufferSize = draw_data->TotalVtxCount + 5000;
			gfx::ResourceDesc<gfx::VertexBuffer> desc;
			desc.Usage = gfx::eUsage::DYNAMIC;
			desc.ByteWidth = _VertexBufferSize * sizeof(ImDrawVert);
			desc.CPUAccessFlags = gfx::eCpuAccessFlag::CPU_ACCESS_WRITE;
			_vb = gfx::create_gfx_object(desc);
			_vert_data.resize(_VertexBufferSize);
		}

		if (!_ib.valid() || _IndexBufferSize < draw_data->TotalIdxCount)
		{
			if (_ib.valid()) { /*_ib->Release(); */ }
			_IndexBufferSize = draw_data->TotalIdxCount + 10000;
			gfx::ResourceDesc<gfx::IndexBuffer>  desc;
			desc.Usage = gfx::eUsage::DYNAMIC;
			desc.ByteWidth = _IndexBufferSize * sizeof(ImDrawIdx);
			desc.CPUAccessFlags = gfx::eCpuAccessFlag::CPU_ACCESS_WRITE;
			_ib = gfx::create_gfx_object(desc);
			_indices_data.resize(_IndexBufferSize);
		}


		ImDrawVert* vtx_dst = &_vert_data[0];
		ImDrawIdx* idx_dst = &_indices_data[0];
		for (int n = 0; n < draw_data->CmdListsCount; n++)
		{
			const ImDrawList* cmd_list = draw_data->CmdLists[n];
			memcpy(vtx_dst, cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof(ImDrawVert));
			memcpy(idx_dst, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));
			vtx_dst += cmd_list->VtxBuffer.Size;
			idx_dst += cmd_list->IdxBuffer.Size;
		}
		gfx::upload_dynamic_data(_vb, &_vert_data[0], draw_data->TotalVtxCount * sizeof(ImDrawVert));
		gfx::upload_dynamic_data(_ib, &_indices_data[0], draw_data->TotalIdxCount * sizeof(ImDrawIdx));

		if (gfx::begin())
		{
			float L = draw_data->DisplayPos.x;
			float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
			float T = draw_data->DisplayPos.y;
			float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;
			float mvp[4][4] =
			{
					{ 2.0f / (R - L),   0.0f,           0.0f,       0.0f },
					{ 0.0f,         2.0f / (T - B),     0.0f,       0.0f },
					{ 0.0f,         0.0f,           0.5f,       0.0f },
					{ (R + L) / (L - R),  (T + B) / (B - T),    0.5f,       1.0f },
			};
			gfx::set_uniform(_proj_mat_handle, mvp, sizeof(mvp));
			gfx::end();
		}

		setup_render_states(draw_data);

		// Render command lists
		// (Because we merged all buffers into a single one, we maintain our own offset into them)
		int global_idx_offset = 0;
		int global_vtx_offset = 0;
		ImVec2 clip_off = draw_data->DisplayPos;
		for (int n = 0; n < draw_data->CmdListsCount; n++)
		{
			const ImDrawList* cmd_list = draw_data->CmdLists[n];
			for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
			{
				const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
				if (pcmd->UserCallback != NULL)
				{
					// User callback, registered via ImDrawList::AddCallback()
					// (ImDrawCallback_ResetRenderState is a special callback value used by the user to request the renderer to reset render state.)
					if (pcmd->UserCallback == ImDrawCallback_ResetRenderState)
						setup_render_states(draw_data);
					else
						pcmd->UserCallback(cmd_list, pcmd);
				}
				else
				{
					//// Apply scissor/clipping rectangle
					//const D3D11_RECT r = { 
					//	(LONG)(pcmd->ClipRect.x - clip_off.x), 
					//	(LONG)(pcmd->ClipRect.y - clip_off.y),
					//	(LONG)(pcmd->ClipRect.z - clip_off.x),
					//	(LONG)(pcmd->ClipRect.w - clip_off.y) };
					//ctx->RSSetScissorRects(1, &r);

					if (gfx::begin())
					{
						_main_tex.vptr = static_cast<gfx::GPUHandle>(pcmd->TextureId);
						gfx::set_texture(_main_tex_handle, _main_tex);
						gfx::draw_indexed(
							pcmd->ElemCount,
							pcmd->IdxOffset + global_idx_offset,
							pcmd->VtxOffset + global_vtx_offset
						);
						gfx::end();
					}
				}
			}
			global_idx_offset += cmd_list->IdxBuffer.Size;
			global_vtx_offset += cmd_list->VtxBuffer.Size;
		}
	}

	void GuiRenderer::create_font_texture()
	{
		// Build texture atlas
		ImGuiIO& io = ImGui::GetIO();
		unsigned char* pixels;
		int width, height;
		io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

		gfx::ResourceDesc<gfx::Texture2D> desc;
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = gfx::eFormat::R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = gfx::eUsage::DEFAULT;
		desc.BindFlags = gfx::eBindFlag::SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		gfx::ISRVDesc srv_desc{ desc.Format };
		_font_texture = gfx::create_gfx_object(desc, srv_desc);

		gfx::SubResource sub{ _font_texture, 0, 0, 1 };
		gfx::upload_texture(sub, 0, width, 0, height, pixels, width * 4);

		// Store our identifier
		io.Fonts->TexID = (ImTextureID)_font_texture.get_view_object();
	}

	bool GuiRenderer::create_device_objects()
	{
		//if (_pFontSampler)
		//	invalidate_device_objects();

		{
			static std::string vertex_shader =
				"cbuffer vertexBuffer : register(b0) \
          {\
          float4x4 _proj_mat; \
          };\
          struct VS_INPUT\
          {\
          float2 pos : POSITION;\
          float4 col : COLOR0;\
          float2 uv  : TEXCOORD0;\
          };\
          \
          struct PS_INPUT\
          {\
          float4 pos : SV_POSITION;\
          float4 col : COLOR0;\
          float2 uv  : TEXCOORD0;\
          };\
          \
          PS_INPUT main(VS_INPUT input)\
          {\
          PS_INPUT output;\
          output.pos = mul(_proj_mat, float4(input.pos.xy, 0.f, 1.f));\
          output.col = input.col;\
          output.uv  = input.uv;\
          return output;\
          }";

			try {
				_vs = gfx::compile_shader(gfx::ShaderStage::VS, std::move(vertex_shader), "main", "vs_5_0", true);
				gfx::ShaderBinding binding{ gfx::ShaderStage::VS, gfx::shader_reflection(_vs.bytecode) };
				_binding.merge(binding);
			} catch (std::exception & e) {
				std::cout << e.what();
				return false;
			}

			gfx::VertexElements vert_elements{
				0, // stream 0
				{
					gfx::VertexElement{ gfx::VertexElementType::FLOAT2, "POSITION", 0, false, },
					gfx::VertexElement{ gfx::VertexElementType::FLOAT2, "TEXCOORD", 0, false, },
					gfx::VertexElement{ gfx::VertexElementType::RGBA_UNORM, "COLOR", 0, false, },
				}
			};

			_vert_layout = gfx::create_vertex_layout(
				{ std::move(vert_elements) },
				std::string_view{ _vs.bytecode }
			);
		}

		// Create the pixel shader
		{
			static std::string pixel_shader =
				"struct PS_INPUT\
          {\
          float4 pos : SV_POSITION;\
          float4 col : COLOR0;\
          float2 uv  : TEXCOORD0;\
          };\
          sampler sampler0 : register(s1);\
          Texture2D _main_tex;\
          \
          float4 main(PS_INPUT input) : SV_Target\
          {\
          float4 out_col = input.col * _main_tex.Sample(sampler0, input.uv); \
          return out_col; \
          }";
			try {
				_ps = gfx::compile_shader(gfx::ShaderStage::PS, std::move(pixel_shader), "main", "ps_5_0", true);
				gfx::ShaderBinding binding{ gfx::ShaderStage::PS, gfx::shader_reflection(_ps.bytecode) };
				_binding.merge(binding);
			} catch (std::exception & e) {
				std::cout << e.what();
				return false;
			}
		}

		create_font_texture();

		return true;
	}

	bool GuiRenderer::setup()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.BackendRendererName = "imgui_impl";
		io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;  // We can honor the ImDrawCmd::VtxOffset field, allowing for large meshes.

		if (!create_device_objects()) {
			return false;
		}

		_proj_mat_handle = gfx::UniformHash::get("_proj_mat", gfx::UniformType::Var);
		_main_tex_handle = gfx::UniformHash::get("_main_tex", gfx::UniformType::Srv);
		return true;
	}

	void GuiRenderer::shutdown()
	{
		invalidate_device_objects();
	}

	void GuiRenderer::new_frame()
	{
	}
}
