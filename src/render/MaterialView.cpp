#include<render/Material.hpp>
#include<../thirdparty/imgui/imgui.h>
//#include<../thirdparty/imgui/imfilebrowser.h>
#include<render/TextureLoader.hpp>

namespace tutorial::graphics
{
	auto MaterialView::on_gui() -> bool
	{
		if (!ImGui::CollapsingHeader("Material")) {
			return false;
		}

		const auto& bp = _material.bp;
		ImGui::Text(bp._name.c_str());

		bool is_change = false;

		for (auto i = 0u; i < bp._properties.size(); ++i)
		{
			const mtl::Property& prop = bp._properties[i];
			const auto& offset = bp._data_offsets[i];
			void* val = _material._uniform_stream.data() + offset;
			switch (prop.type)
			{
			case mtl::Property::Control::Color: {
				is_change |= ImGui::ColorEdit3(prop.name.c_str(), (float*)val);
			} break;
			case mtl::Property::Control::SliderFloat: {
				const mtl::SliderFloat& slider = std::get<mtl::SliderFloat>(prop.control);
				is_change |= ImGui::SliderFloat(prop.name.c_str(), (float*)val, slider.min, slider.max);
			} break;
			case mtl::Property::Control::SliderInt: {
				const mtl::SliderInt& slider = std::get<mtl::SliderInt>(prop.control);
				is_change |= ImGui::SliderInt(prop.name.c_str(), (int*)val, slider.min, slider.max);
			} break;
			case mtl::Property::Control::CheckBox: {
				//const mtl::CheckBox& box = std::get<mtl::CheckBox>(prop.control);
				uint8 option_index = *static_cast<const uint8*>(val);
				is_change |= ImGui::Checkbox(prop.name.c_str(), &_material._options[option_index]);
			} break;
			case mtl::Property::Control::Texture2D: {
				uint8 texture_index = *static_cast<const uint8*>(val);

				ImVec2 image_size(66, 66);
				if (_material._textures[texture_index]) {
					const gfx::SRV& texture = _material._textures[texture_index].value().get();
					ImGui::Image((ImTextureID)texture.vptr, image_size);
				} else {
					ImGui::Button("texture", image_size);
				}
				
				ImGui::SameLine();

				//static ImGui::FileBrowser file_dialog;
				//static uint8 ___texture_index = 0u;
				//if (ImGui::Button(prop.name.c_str())) {
					//___texture_index = texture_index;
					//file_dialog.Open();
				//}
				//file_dialog.Display();
				//if (file_dialog.HasSelected() && texture_index == ___texture_index)
				//{
				//	auto path = file_dialog.GetSelected();
				//	file_dialog.ClearSelected();
				//	if (path.extension() == ".dds") {
				//		try {
				//			_material._textures[texture_index] = TextureLoader::load(path);
				//			is_change |= true;
				//			file_dialog.SetPwd(path.parent_path());
				//		} catch (std::exception & e) {
				//			//std::cout << e.what() << std::endl;
				//		}
				//	}
				//}
			} break;
			default:
				break;
			}
		}

		return is_change;
	}
}