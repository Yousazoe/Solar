#pragma once
#include<math/Math3d.hpp>
#include<core/Types.hpp>
#include<variant>
#include<array>
#include<string>

namespace tutorial::graphics::mtl
{
	struct ColorPalette { float r, g, b; };
	struct SliderFloat { float min; float max; float default; };
	struct SliderInt { int min; int max; int default; };
	struct CheckBox { bool value; };
	struct Texture2D {};
	struct Int { int default; };
	struct Int2 { int default[2]; };
	struct Int3 { int default[3]; };
	struct Int4 { int default[4]; };
	struct Float { float default; };
	struct Float2 { float default[2]; };
	struct Float3 { float default[3]; };
	struct Float4 { float default[4]; };

	struct Property
	{
		using float2 = math::Vec2f;
		using float3 = math::Vec3f;
		using float4 = math::Vec4f;
		using int2 = std::array<int, 2>;
		using int3 = std::array<int, 3>;
		using int4 = std::array<int, 4>;

		enum class Control : uint8
		{
			Color,
			SliderFloat,
			SliderInt,
			CheckBox,
			Texture2D,
			Int, 
			Int2, 
			Int3, 
			Int4,
			Float, 
			Float2, 
			Float3, 
			Float4,
			Count,
		};

		Property(std::string name, ColorPalette c) noexcept;
		Property(std::string name, SliderFloat c) noexcept;
		Property(std::string name, SliderInt c) noexcept;
		Property(std::string name, CheckBox c) noexcept;
		Property(std::string name, Texture2D c) noexcept;
		Property(std::string name, Int c) noexcept;
		Property(std::string name, Int2 c) noexcept;
		Property(std::string name, Int3 c) noexcept;
		Property(std::string name, Int4 c) noexcept;
		Property(std::string name, Float c) noexcept;
		Property(std::string name, Float2 c) noexcept;
		Property(std::string name, Float3 c) noexcept;
		Property(std::string name, Float4 c) noexcept;

		auto val_ptr() const -> const void* { return reinterpret_cast<const void*>(&default_val); }

		std::string name = "";
		Control type;
		uint8 size;

		std::variant<
			ColorPalette, SliderFloat, SliderInt, CheckBox, Texture2D,
			Int, Int2, Int3, Int4, Float, Float2, Float3, Float4
		> control;
		std::variant<float, float2, float3, float4, int, int2, int3, int4, bool> default_val;
	};


}
