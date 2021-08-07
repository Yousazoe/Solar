#pragma once
#include<render/MaterialBlueprint.hpp>

namespace tutorial::graphics
{
	class MaterialView final
	{
	public:
		MaterialView(MaterialInstance& material) noexcept : _material{ material } {}

	public:
		auto on_gui() -> bool;

	private:
		MaterialInstance& _material;
	};

}
