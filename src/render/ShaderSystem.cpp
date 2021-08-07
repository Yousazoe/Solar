#include<render/ShaderSystem.hpp>
#include<array>
#include<string>
#include<cassert>

namespace tutorial::graphics
{
	struct ShaderSystemImpl
	{
		struct LocalBlock
		{
			bool enable = false;
			uint8 option_offset = 0u;
		};

		auto add_block(const ShaderBlock& block) noexcept -> void
		{
			assert(block.index < _blocks.size());
			_blocks[block.index].enable = true;
			_blocks[block.index].option_offset = _option_count;
			const auto& option_strings = block.option_strings;
			std::copy_n(option_strings.begin(), option_strings.size(), _options.begin() + _option_count);
			_option_count += uint8(block.option_strings.size());
		}

		auto set_option(uint8 block, uint8 option, bool enable) noexcept -> void
		{
			assert(block < _blocks.size());
			uint8 global_option = _blocks[block].option_offset + option;
			//_options.set(local_option, enable);
		}

		uint8 _option_count = 0u;
		std::array<std::string, 32> _options = {};
		std::array<LocalBlock, 32> _blocks = {};
	};

	auto _ShaderSystemImpl() -> decltype(auto)
	{
		static ShaderSystemImpl impl;
		return (impl);
	}

	auto ShaderSystem::add_block(const ShaderBlock& block) noexcept -> void
	{
		_ShaderSystemImpl().add_block(block);
	}

	auto ShaderSystem::set_option(uint8 block, uint8 option, bool enable) noexcept -> void
	{
		_ShaderSystemImpl().set_option(block, option, enable);
	}
}
