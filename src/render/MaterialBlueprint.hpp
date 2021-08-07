#pragma once
#include<core/Types.hpp>
#include<vector>
#include<render/MaterialProperty.hpp>
#include<render/MaterialInstance.hpp>

namespace tutorial::graphics
{
	class MaterialBlueprint final
	{
		friend class MaterialView;
	public:
		using MaterialPropertyHandle = uint16;
	public:
		MaterialBlueprint(std::string name) noexcept;
		template<class ...Properties>
		MaterialBlueprint(std::string name, Properties&&... properties) noexcept;

	public:
		auto register_property(const mtl::Property& prop) -> bool;
		auto create_instance() const -> MaterialInstance;
		auto get_property(const std::string& name) const -> MaterialPropertyHandle;

	public:
		std::string _name;
		uint16 _size = 0u;
		uint16 _uniform_count = 0u;
		uint8 _option_count = 0u;
		uint8 _texture_count = 0u;
		uint16 _uniform_stream_size = 0u;
		// uniform = uniform handle | size | data
		// texture = texture handle | local index
		std::array<char, 256> _uniform_stream = {};
		std::array<bool, 8> _options = { false };

		std::vector<uint16> _data_offsets = {};
		std::vector<mtl::Property> _properties = {};
	};

	template<class ...Properties>
	inline MaterialBlueprint::MaterialBlueprint(std::string name, Properties&&... properties) noexcept :
		_name{ std::move(name) }
	{
		using expand = bool[];
		expand{ register_property(properties) ... };
	}
}
