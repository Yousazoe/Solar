#pragma once
#include"PathtracingScene.hpp"
#include<vector>
#include<fs/DiskFile.hpp>

namespace tutorial::graphics::ptgi
{
	static  auto ___struct_buffer(uint32 struct_size, uint32 count)
	{
		gfx::GfxObject<gfx::StructureBuffer, gfx::SRV> mat_buf =
			gfx::create_gfx_object(
				gfx::ResourceDesc<gfx::StructureBuffer> {
				count* struct_size,
				gfx::eUsage::DEFAULT,
				gfx::eBindFlag::SHADER_RESOURCE, 0,
				gfx::RESOURCE_MISC_FLAG::RESOURCE_MISC_BUFFER_STRUCTURED,
				struct_size
			},
			gfx::ISRVDesc{ gfx::eFormat::UNKNOWN }
		);
		return std::move(mat_buf);
	}

	static  auto ___format_struct_buffer(uint32 struct_size, uint32 count, gfx::eFormat format)
	{
		gfx::GfxObject<gfx::StructureBuffer, gfx::SRV> mat_buf =
			gfx::create_gfx_object(
				gfx::ResourceDesc<gfx::StructureBuffer> {
			count* struct_size,
				gfx::eUsage::DEFAULT,
				gfx::eBindFlag::SHADER_RESOURCE, 0, 0,
				struct_size
			},
			gfx::ISRVDesc{ format }
		);
		return std::move(mat_buf);
	}

	auto PTGIScene::build() -> void
	{
		_material_buffer = ___struct_buffer(sizeof(Material), _materials.size());
		gfx::upload_data(_material_buffer, 0, _materials.size() * sizeof(Material), _materials.data(), _material_buffer.desc.ByteWidth);

		if (_lights.size() > 0u)
		{
			_light_buffer = ___struct_buffer(sizeof(Light), _lights.size());
			gfx::upload_data(_light_buffer, 0, _lights.size() * sizeof(Light), _lights.data(), _light_buffer.desc.ByteWidth);
		}

		if (_balls.size() > 0u)
		{
			_sphere_buffer = ___struct_buffer(sizeof(SphereMesh), _balls.size());
			gfx::upload_data(_sphere_buffer, 0, _balls.size() * sizeof(SphereMesh), _balls.data(), _sphere_buffer.desc.ByteWidth);
		}

		if (_quads.size() > 0u)
		{
			_quad_buffer = ___struct_buffer(sizeof(QuadMesh), _quads.size());
			gfx::upload_data(_quad_buffer, 0, _quads.size() * sizeof(QuadMesh), _quads.data(), _quad_buffer.desc.ByteWidth);
		}

	}

	auto PTGIScene::update_material(int handle, const Material& material) -> void
	{
		if (handle >= (int)_materials.size())
			return;
		_materials[handle] = material;
		gfx::upload_data(_material_buffer, 0, _materials.size() * sizeof(Material), _materials.data(), _material_buffer.desc.ByteWidth);
	}

	auto PTGIScene::update_light(int handle, const Light& light) -> void
	{
		if (handle >= (int)_lights.size())
			return;
		_lights[handle] = light;
		gfx::upload_data(_light_buffer, 0, _lights.size() * sizeof(Light), _lights.data(), _light_buffer.desc.ByteWidth);
	}
}

