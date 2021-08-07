#pragma once
#include<render/ShaderBlock.hpp>

namespace tutorial::graphics
{
	class ShaderSystem
	{
		ShaderSystem() = delete;
	public:
		static auto add_block(const ShaderBlock& block) noexcept -> void;
		static auto set_option(uint8 block, uint8 option, bool enable) noexcept -> void;
	};

}

