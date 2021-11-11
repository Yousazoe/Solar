#include"Demo.hpp"
#include<gui/GuiSystem.hpp>
#include<middleware/camera/CameraControllerView.hpp>
#include<render/Shader.hpp>
auto demo_entry = ___PathTracerSimple;

int main()
{
	using namespace tutorial;
	gfx::setup();

	
	int window_width = 1024;
	int window_height = 768;
	auto window = hw::Window{ hw::VideoMode(window_width, window_height), "Solar RenderingEngine" };
	App app{ window };
	app.swapchain = gfx::create_swapchain(app.window.getSystemHandle(), window_width, window_height);
	auto& swapchain = app.swapchain;

	gui::GuiSystem gui_sys{  };
	if (!gui_sys.setup(window)) {
	}


	// camera
	graphics::Camera& camera = app.camera;
	camera.set_aspect_ratio(float(window_width) / float(window_height));
	camera.set_fov(math::PI / 3.f);
	camera.set_zfar(1000.f);
	ecs::Transform& camera_trans = app.camera_trans;
	camera_trans.set_pos({ 0, 0, -5 });
	camera_trans.set_rot({ 0, 0, 0 });
	render::CameraController camera_control{ camera, camera_trans, 1.f };
	camera_control._lookat_dist = -5.0f;
	camera_control._lookat_mode = true;

	static bool init = true;
	static bool running = true;
	while (1)
	{
		hw::Event event;
		if (window.pollEvent(event))
		{
			if (event.type == hw::Event::Closed) {
				window.close();
				return false;
			}
			app.event = event;
			gui_sys.handle_input(event);
			camera_control.handle_input_events(event);

			if (event.type == hw::Event::EventType::KeyPressed)
			{
				if (hw::Keyboard::Key::F1 == event.key.code) {
					gui_sys.show_gui ^= 1;
				}
			}
		}
		else
		{
			app.event = {};
		}

		gui_sys.update();
		camera_control.update(0.16f);

		gfx::next_frame();

		if (running)
		{
			demo_entry(app, init);
		}
		if (init) init = false;

		

		auto backbuffer = swapchain.backbuffer();
		gfx::bind_framebuffer(backbuffer);
		gui_sys.render();
		gfx::present(swapchain);
	}
	
	gfx::shutdown();
}