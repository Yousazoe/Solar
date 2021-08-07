#pragma once
#include<render/ShaderInstance.hpp>
#include<render/ShaderBlock.hpp>
#include<map>
#include<stl/table.h>
#include<bitset>

namespace tutorial::graphics
{
	class Shader final
	{
		friend class ShaderVersion;

		struct LocalBlock
		{
			bool enable = false;
			uint8 option_offset = 0u;
		};

	public:
		explicit Shader(std::string name = "", bool compute = false) noexcept;
		explicit Shader(std::string name, std::string entry, bool compute = false) noexcept;
		Shader(const Shader&) = delete;
		void operator=(const Shader&) = delete;
		Shader(Shader&&) = default;
		void operator=(Shader&& rhs) noexcept;

	public:
		auto add_block(const ShaderBlock& block) noexcept -> void;
		auto default() noexcept -> ShaderVersion;

	private:
		auto fetch(uint64 version) noexcept -> const ShaderInstance&;
		auto compile(uint64 version) const noexcept-> ShaderInstance;

	public:
		static bool _has_shader_cachae;

	private:
		std::string _name = "";
		std::string _entry = "main";
		bool _compute = false;
		uint8 _option_count = 0u;
		std::array<std::string, 32> _options = {};
		std::array<LocalBlock, 32> _blocks = {};
		std::map<uint64, ShaderInstance> _shader_instances = {};
	};

	class ShaderVersion
	{
	public:
		explicit ShaderVersion(Shader& shader) noexcept : _shader{ shader } {}
		auto set_option(uint8 block, uint8 option, bool enable) noexcept -> void;
		auto hash() const noexcept -> uint64 { return _options.to_ullong(); }
		auto fetch() noexcept -> const ShaderInstance& { return _shader.fetch(hash()); }

	private:
		Shader& _shader;
		std::bitset<32> _options = 0u;
	};

}
