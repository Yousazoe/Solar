#include<render/MaterialBlueprint.hpp>
#include<gfx/Gfx.hpp>


namespace tutorial::graphics
{
	MaterialBlueprint::MaterialBlueprint(std::string name) noexcept : 
		_name{ std::move(name) }
	{
	}

	auto MaterialBlueprint::register_property(const mtl::Property& prop) -> bool
	{
		++_uniform_count;
		const uint8& size = prop.size;
		_size += size;
		uint16 data_offset = 0u;
		auto offset = _uniform_stream_size;
		if (prop.type == mtl::Property::Control::CheckBox)
		{
			// type | sizeof(option_index) | option_index
			assert(_option_count < _options.size());
			auto type = gfx::UniformType::Unknown;
			uint8 index = _option_count;
			uint8 byte = uint8(sizeof(index));
			memcpy(_uniform_stream.data() + offset, &type, sizeof(type)); offset += sizeof(type);
			memcpy(_uniform_stream.data() + offset, &byte, sizeof(byte)); offset += sizeof(byte);
			data_offset = offset; 
			memcpy(_uniform_stream.data() + offset, &index, sizeof(index)); offset += sizeof(index);
			_options[index] = *static_cast<const bool*>(prop.val_ptr());
			++_option_count;
		}
		else if (prop.type == mtl::Property::Control::Texture2D)
		{
			// type | handle | sizeof(texture_index) | texture_index
			assert(_texture_count < MaterialInstance::Max_Texture_Count);
			uint8 index = _texture_count;
			uint8 byte = uint8(sizeof(index));
			auto type = gfx::UniformType::Srv;
			memcpy(_uniform_stream.data() + offset, &type, sizeof(type)); offset += sizeof(type);
			gfx::UniformHandle handle = gfx::UniformHash::get(prop.name, type);
			memcpy(_uniform_stream.data() + offset, &handle, sizeof(handle)); offset += sizeof(handle);
			memcpy(_uniform_stream.data() + offset, &byte, sizeof(byte)); offset += sizeof(byte);
			data_offset = offset;
			memcpy(_uniform_stream.data() + offset, &index, sizeof(index)); offset += sizeof(index);
			++_texture_count;
		}
		else
		{
			// type | handle | size | data
			auto type = gfx::UniformType::Var;
			memcpy(_uniform_stream.data() + offset, &type, sizeof(type)); offset += sizeof(type);
			gfx::UniformHandle handle = gfx::UniformHash::get(prop.name, type);
			memcpy(_uniform_stream.data() + offset, &handle, sizeof(handle)); offset += sizeof(handle);
			memcpy(_uniform_stream.data() + offset, &size, sizeof(size)); offset += sizeof(size);
			data_offset = offset;
			memcpy(_uniform_stream.data() + offset, prop.val_ptr(), size); offset += size;
		}
		_data_offsets.push_back(data_offset); // offset of value
		_uniform_stream_size = offset;
		_properties.push_back(prop);
		assert(_uniform_stream_size < _uniform_stream.size());
		return true;
	}

	auto MaterialBlueprint::create_instance() const -> MaterialInstance
	{
		MaterialInstance mat{ *this };
		mat._uniform_stream_size = _uniform_stream_size;
		mat._uniform_stream = _uniform_stream;
		mat._option_count = _option_count;
		mat._options = _options;
		return mat;
	}

	auto MaterialBlueprint::get_property(const std::string& name) const -> MaterialPropertyHandle
	{
		for (auto i = 0u; i < _properties.size(); ++i)
		{
			auto& prop = _properties[i];
			if (prop.name.compare(name) != 0)
			{
				continue;
			}

			if (prop.type == mtl::Property::Control::CheckBox) {
				auto offset = _data_offsets[i];
				auto option_index = *reinterpret_cast<const uint8*>(&_uniform_stream[offset]);
				return option_index;
			}
			else if (prop.type == mtl::Property::Control::Texture2D) {
				auto offset = _data_offsets[i];
				auto texture_index = *reinterpret_cast<const uint8*>(&_uniform_stream[offset]);
				return texture_index;
			}
			else { // uniform
				return _data_offsets[i];
			}
		}

		throw std::invalid_argument{
			"[render] MaterialBlueprint::get_property, no material property --- " + name 
		};
	}
}