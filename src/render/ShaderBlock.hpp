#pragma once
#include<core/Types.hpp>
#include<vector>
#include<string>

namespace tutorial::graphics
{
	struct ShaderBlock
	{
		ShaderBlock(std::string name, std::vector<std::string> option_strings = {}) noexcept;

		std::string name;
		uint8 index = 0u;
		std::vector<std::string> option_strings = {};
	};
}
