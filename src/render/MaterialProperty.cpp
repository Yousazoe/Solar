#include<render/MaterialProperty.hpp>

namespace tutorial::graphics::mtl
{
	Property::Property(std::string name, ColorPalette c) noexcept :
		name{ name },
		type{ Control::Color },
		control{ c },
		size{ sizeof(c) },
		default_val{ math::Vec3f{ c.r, c.g, c.b } }
	{
	}

	Property::Property(std::string name, SliderFloat c) noexcept :
		name{ name },
		type{ Control::SliderFloat },
		control{ c },
		size{ sizeof(c.default) },
		default_val{ c.default }
	{
	}

	Property::Property(std::string name, SliderInt c) noexcept :
		name{ name },
		type{ Control::SliderInt },
		control{ c },
		size{ sizeof(c.default) },
		default_val{ c.default }
	{
	}

	Property::Property(std::string name, CheckBox c) noexcept :
		name{ name },
		type{ Control::CheckBox },
		control{ c },
		size{ sizeof(c) },
		default_val{ c.value }
	{
	}

	Property::Property(std::string name, Texture2D c) noexcept :
		name{ name },
		type{ Control::Texture2D },
		control{ c },
		size{ sizeof(c) },
		default_val{ 1 }
	{
	}

	Property::Property(std::string name, Int c) noexcept :
		name{ name },
		type{ Control::Int },
		control{ c },
		size{ sizeof(c.default) },
		default_val{ c.default }
	{
	}

	Property::Property(std::string name, Int2 c) noexcept :
		name{ name },
		type{ Control::Int2 },
		control{ c },
		size{ sizeof(c.default) },
		default_val{ c.default }
	{
	}

	Property::Property(std::string name, Int3 c) noexcept :
		name{ name },
		type{ Control::Int3 },
		control{ c },
		size{ sizeof(c.default) },
		default_val{ c.default }
	{
	}

	Property::Property(std::string name, Int4 c) noexcept :
		name{ name },
		type{ Control::Int4 },
		control{ c },
		size{ sizeof(c.default) },
		default_val{ c.default }
	{
	}

	Property::Property(std::string name, Float c) noexcept :
		name{ name },
		type{ Control::Float },
		control{ c },
		size{ sizeof(c.default) },
		default_val{ c.default }
	{
	}

	Property::Property(std::string name, Float2 c) noexcept :
		name{ name },
		type{ Control::Float2 },
		control{ c },
		size{ sizeof(c.default) },
		default_val{ c.default }
	{
	}

	Property::Property(std::string name, Float3 c) noexcept :
		name{ name },
		type{ Control::Float3 },
		control{ c },
		size{ sizeof(c.default) },
		default_val{ c.default }
	{
	}

	Property::Property(std::string name, Float4 c) noexcept :
		name{ name },
		type{ Control::Float4 },
		control{ c },
		size{ sizeof(c.default) },
		default_val{ c.default }
	{
	}
}