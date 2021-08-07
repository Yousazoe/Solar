#pragma once
#include<middleware/camera/CameraController.hpp>
#include<../thirdparty/imgui/imgui.h>

namespace tutorial::render
{
	class CameraControllerView final
	{
	public:
		CameraControllerView(CameraController& controller) noexcept : _controller{ controller } {}
	public:
		auto on_gui() -> void;

	private:
		CameraController& _controller;
	};

	auto CameraControllerView::on_gui() -> void
	{
		ImGui::Text("camera");
		ImGui::Checkbox("lookat mode", &_controller._lookat_mode);
	}

}
