#include<gui/GuiSystem.hpp>
#include<windows.h>
//#include<core/Log.hpp>

namespace tutorial::gui
{
	static HWND                 g_hWnd = 0;
	static INT64                g_Time = 0;
	static INT64                g_TicksPerSecond = 0;
	static ImGuiMouseCursor     g_LastMouseCursor = ImGuiMouseCursor_COUNT;
	static bool                 g_HasGamepad = false;
	static bool                 g_WantUpdateHasGamepad = true;

	static bool update_cursor()
	{
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange)
			return false;

		ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
		if (imgui_cursor == ImGuiMouseCursor_None || io.MouseDrawCursor)
		{
			// Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
			::SetCursor(NULL);
		}
		else
		{
			// Show OS mouse cursor
			LPTSTR win32_cursor = IDC_ARROW;
			switch (imgui_cursor)
			{
			case ImGuiMouseCursor_Arrow:        win32_cursor = IDC_ARROW; break;
			case ImGuiMouseCursor_TextInput:    win32_cursor = IDC_IBEAM; break;
			case ImGuiMouseCursor_ResizeAll:    win32_cursor = IDC_SIZEALL; break;
			case ImGuiMouseCursor_ResizeEW:     win32_cursor = IDC_SIZEWE; break;
			case ImGuiMouseCursor_ResizeNS:     win32_cursor = IDC_SIZENS; break;
			case ImGuiMouseCursor_ResizeNESW:   win32_cursor = IDC_SIZENESW; break;
			case ImGuiMouseCursor_ResizeNWSE:   win32_cursor = IDC_SIZENWSE; break;
			case ImGuiMouseCursor_Hand:         win32_cursor = IDC_HAND; break;
			}
			::SetCursor(::LoadCursor(NULL, win32_cursor));
		}
		return true;
	}

	static void update_mouse_pos()
	{
		ImGuiIO& io = ImGui::GetIO();

		// Set OS mouse position if requested (rarely used, only when ImGuiConfigFlags_NavEnableSetMousePos is enabled by user)
		if (io.WantSetMousePos)
		{
			POINT pos = { (int)io.MousePos.x, (int)io.MousePos.y };
			::ClientToScreen(g_hWnd, &pos);
			::SetCursorPos(pos.x, pos.y);
		}

		// Set mouse position
		io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
		POINT pos;
		if (HWND active_window = ::GetForegroundWindow())
			if (active_window == g_hWnd || ::IsChild(active_window, g_hWnd))
				if (::GetCursorPos(&pos) && ::ScreenToClient(g_hWnd, &pos))
					io.MousePos = ImVec2((float)pos.x, (float)pos.y);
	}

	bool GuiSystem::setup(hw::Window& window)
	{
		//g_log_info.log("gui") << "gui system setup begin.";
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();

		if (!::QueryPerformanceFrequency((LARGE_INTEGER *)&g_TicksPerSecond))
			return false;
		if (!::QueryPerformanceCounter((LARGE_INTEGER *)&g_Time))
			return false;

		// Setup back-end capabilities flags
		g_hWnd = (HWND)window.getSystemHandle();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;         // We can honor GetMouseCursor() values (optional)
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;          // We can honor io.WantSetMousePos requests (optional, rarely used)
		io.BackendPlatformName = "imgui";
		io.ImeWindowHandle = g_hWnd;

		// Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array that we will update during the application lifetime.
		io.KeyMap[ImGuiKey_Tab] = hw::Keyboard::Tab;
		io.KeyMap[ImGuiKey_LeftArrow] = hw::Keyboard::Left;
		io.KeyMap[ImGuiKey_RightArrow] = hw::Keyboard::Right;
		io.KeyMap[ImGuiKey_UpArrow] = hw::Keyboard::Up;
		io.KeyMap[ImGuiKey_DownArrow] = hw::Keyboard::Down;
		io.KeyMap[ImGuiKey_PageUp] = hw::Keyboard::PageUp;
		io.KeyMap[ImGuiKey_PageDown] = hw::Keyboard::PageDown;
		io.KeyMap[ImGuiKey_Home] = hw::Keyboard::Home;
		io.KeyMap[ImGuiKey_End] = hw::Keyboard::End;
		io.KeyMap[ImGuiKey_Insert] = hw::Keyboard::Insert;
#ifdef ANDROID
		io.KeyMap[ImGuiKey_Backspace] = hw::Keyboard::Delete;
#else
		io.KeyMap[ImGuiKey_Delete] = hw::Keyboard::Delete;
		io.KeyMap[ImGuiKey_Backspace] = hw::Keyboard::BackSpace;
#endif
		io.KeyMap[ImGuiKey_Space] = hw::Keyboard::Space;
		io.KeyMap[ImGuiKey_Enter] = hw::Keyboard::Return;
		io.KeyMap[ImGuiKey_Escape] = hw::Keyboard::Escape;
		io.KeyMap[ImGuiKey_A] = hw::Keyboard::A;
		io.KeyMap[ImGuiKey_C] = hw::Keyboard::C;
		io.KeyMap[ImGuiKey_V] = hw::Keyboard::V;
		io.KeyMap[ImGuiKey_X] = hw::Keyboard::X;
		io.KeyMap[ImGuiKey_Y] = hw::Keyboard::Y;
		io.KeyMap[ImGuiKey_Z] = hw::Keyboard::Z;

		if (!_renderer.setup()) {
			return false;
		}

		//g_log_info.log("gui") << "gui system setup end.";
		return true;
	}

	void GuiSystem::shutdown()
	{
		g_hWnd = (HWND)0;
	}

	void GuiSystem::update()
	{
		_renderer.new_frame();

		ImGuiIO& io = ImGui::GetIO();
		IM_ASSERT(io.Fonts->IsBuilt() &&
			"Font atlas not built! It is generally built by the renderer back-end. Missing call to renderer _NewFrame() function? e.g. ImGui_ImplOpenGL3_NewFrame()."
		);

		// Setup display size (every frame to accommodate for window resizing)
		RECT rect;
		::GetClientRect(g_hWnd, &rect);
		io.DisplaySize = ImVec2((float)(rect.right - rect.left), (float)(rect.bottom - rect.top));

		// Setup time step
		INT64 current_time;
		::QueryPerformanceCounter((LARGE_INTEGER *)&current_time);
		io.DeltaTime = (float)(current_time - g_Time) / g_TicksPerSecond;
		g_Time = current_time;

		// Read keyboard modifiers inputs
		io.KeyCtrl = (::GetKeyState(VK_CONTROL) & 0x8000) != 0;
		io.KeyShift = (::GetKeyState(VK_SHIFT) & 0x8000) != 0;
		io.KeyAlt = (::GetKeyState(VK_MENU) & 0x8000) != 0;
		io.KeySuper = false;
		// io.KeysDown[], io.MousePos, io.MouseDown[], io.MouseWheel: filled by the WndProc handler below.

		// Update OS mouse position
		update_mouse_pos();

		// Update OS mouse cursor with the cursor requested by imgui
		ImGuiMouseCursor mouse_cursor = io.MouseDrawCursor ? ImGuiMouseCursor_None : ImGui::GetMouseCursor();
		if (g_LastMouseCursor != mouse_cursor)
		{
			g_LastMouseCursor = mouse_cursor;
			update_cursor();
		}

		// frame begin
		ImGui::NewFrame();
	}

	void GuiSystem::handle_input(const hw::Event & event)
	{
		if (ImGui::GetCurrentContext() == NULL)
		{
			return;
		}

		ImGuiIO& io = ImGui::GetIO();

		switch (event.type)
		{
		case hw::Event::MouseMoved:
			//s_mouseMoved = true;
			break;
		case hw::Event::MouseButtonPressed:  // fall-through
		case hw::Event::MouseButtonReleased: {
			int button = event.mouseButton.button;
			if (event.type == hw::Event::MouseButtonPressed &&
				button >= 0 && button < 3) {
			}
		} break;
		case hw::Event::MouseWheelScrolled:
			if (event.mouseWheelScroll.wheel == hw::Mouse::VerticalWheel ||
				(event.mouseWheelScroll.wheel == hw::Mouse::HorizontalWheel &&
					io.KeyShift)) {
				io.MouseWheel += event.mouseWheelScroll.delta;
			}
			else if (event.mouseWheelScroll.wheel == hw::Mouse::HorizontalWheel) {
				io.MouseWheelH += event.mouseWheelScroll.delta;
			}
			break;
		case hw::Event::KeyPressed:  // fall-through
		case hw::Event::KeyReleased:
			io.KeysDown[event.key.code] =
				(event.type == hw::Event::KeyPressed);
			break;
		case hw::Event::TextEntered:
			// Don't handle the event for unprintable characters
			if (event.text.unicode < ' ' || event.text.unicode == 127) {
				break;
			}
			io.AddInputCharacter(event.text.unicode);
			break;
		default:
			break;
		}

		io.MouseDown[0] = hw::Mouse::isButtonPressed(hw::Mouse::Left);
		io.MouseDown[1] = hw::Mouse::isButtonPressed(hw::Mouse::Right);
		io.MouseDown[2] = hw::Mouse::isButtonPressed(hw::Mouse::Middle);
	}

	void GuiSystem::render()
	{
		ImGui::Render();
		if (show_gui)
		{
			_renderer.render_draw_data(ImGui::GetDrawData());
		}
	}

}