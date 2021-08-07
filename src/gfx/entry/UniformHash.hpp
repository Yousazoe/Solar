#pragma once
#include<core/Types.hpp>
#include<string>

namespace tutorial::gfx
{
	enum class UniformType : uint8
	{
		Unknown,
		Var,
		ConstBuffer,
		Srv,
		Count,
	};

	struct UniformHandle
	{
		static UniformHandle Invalid;

		auto valid() const -> bool { return index != -1; }
		auto operator == (const UniformHandle& rhs) const -> bool { return index == rhs.index && type == rhs.type; }
		auto operator != (const UniformHandle& rhs) const -> bool { return !(*this == rhs); }

		int index = -1;
		UniformType type = UniformType::Unknown;
	};

	class UniformHash final
	{
	public:
		static auto setup() -> void {}
		static auto get(std::string name, UniformType type) -> UniformHandle;
		static auto get_or_create(std::string name, uint16 size, UniformType type) -> UniformHandle;
	};

}
