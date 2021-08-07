#pragma once
#include<../thirdparty/imgui/imgui.h>
#include<hw/Event.hpp>
#include<hw/Window.hpp>
#include<gui/GuiRenderer.hpp>

namespace tutorial::gui
{
	class GuiSystem
	{
	public:
		GuiSystem() {}

	public:
		bool setup(hw::Window& window);
		void shutdown();
		void update();
		void handle_input(const hw::Event& event);
		void render();

	public:
		bool show_gui = true;
	private:
		GuiRenderer _renderer;
	};
}
