#pragma once

#include<core/Types.hpp>
#include<hw/Window.hpp>
#include<hw/Event.hpp>
#include<gfx/Gfx.hpp>
#include<math/Math3d.hpp>
#include<../thirdparty/imgui/imgui.h>
#include<ecs/Transform.hpp>
#include<render/TextureLoader.hpp>
#include<render/Camera.hpp>
#include<optional>


struct App
{
	hw::Window& window;
	std::optional<hw::Event> event;
	tutorial::gfx::Swapchain swapchain;
	tutorial::graphics::Camera camera;
	tutorial::ecs::Transform camera_trans;
};

#define Demo(name) void name(App& app,  bool init)

Demo(___PathTracerSimple);
Demo(___PbrPathTracer);



