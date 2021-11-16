#include"PathtracingMaterial.hpp"
#include<../thirdparty/imgui/imgui.h>

namespace tutorial::graphics::ptgi
{
	auto disney_material_gui(Material& material) -> bool
	{
		bool change = false;
		if (ImGui::CollapsingHeader("material"))
		{
			change |= ImGui::ColorEdit3("baseColor", material.baseColor.v);
		}
		return change;
	}
}
