// https://github.com/ocornut/imgui
#pragma once
#include<../thirdparty/imgui/imgui.h>
#include<gfx/Gfx.hpp>

namespace tutorial::gui
{
	class GuiRenderer
	{
	public:
		bool setup();
		void shutdown();
		void new_frame();
		void render_draw_data(ImDrawData* draw_data);

	private:
		void create_font_texture();
		bool create_device_objects();
		// Use if you want to reset your rendering device without losing ImGui state.
		void invalidate_device_objects();
		void setup_render_states(ImDrawData* draw_data);
	private:
		int _VertexBufferSize = 5000, _IndexBufferSize = 10000;
		std::vector<ImDrawVert> _vert_data;
		std::vector<ImDrawIdx> _indices_data;
		gfx::VertexBuffer _vb;
		gfx::IndexBuffer _ib;
		gfx::Shader _vs;
		gfx::Shader _ps;
		gfx::ShaderBinding _binding;
		gfx::VertexLayout _vert_layout;
		gfx::GfxObject<gfx::Texture2D, gfx::SRV> _font_texture;
		static gfx::UniformHandle _main_tex_handle;
		static gfx::UniformHandle _proj_mat_handle;
		gfx::SRV _main_tex;
	};
}
