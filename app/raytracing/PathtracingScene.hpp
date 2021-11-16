#pragma once
#include<gfx/Gfx.hpp>
#include<fs/Path.hpp>
#include"PathtracingMaterial.hpp"
#include"PathtracingMesh.hpp"
#include"PathtracingAreaLight.hpp"
#include<vector>

#include<ecs/Transform.hpp>
#include<render/Camera.hpp>
#include<render/Texture.hpp>
#include<optional>

namespace tutorial::graphics::ptgi
{
	class PTGIScene final
	{
	public:
		PTGIScene() = default;

	public:
		auto add_material(const Material& material) -> void { _materials.push_back(material); }
		auto add_light(const Light& light) -> void { _lights.push_back(light); }
		auto add_ball(const SphereMesh& ball) -> void { _balls.push_back(ball); }
		auto add_quad(const QuadMesh& quad) -> void { _quads.push_back(quad); }
		auto build() -> void;
		auto update_material(int handle, const Material& material) -> void;
		auto update_light(int handle, const Light& light) -> void;

	private:
	public:
		std::vector<Material> _materials;
		std::vector<Light> _lights;
		std::vector<SphereMesh> _balls;
		std::vector<QuadMesh> _quads;
		gfx::GfxObject<gfx::StructureBuffer, gfx::SRV> _material_buffer;
		gfx::GfxObject<gfx::StructureBuffer, gfx::SRV> _light_buffer;
		gfx::GfxObject<gfx::StructureBuffer, gfx::SRV> _sphere_buffer;
		gfx::GfxObject<gfx::StructureBuffer, gfx::SRV> _quad_buffer;
	};
}
