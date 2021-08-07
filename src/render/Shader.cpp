#include<render/Shader.hpp>
#include<render/ShaderInstanceLoader.hpp>
#include<fs/Path.hpp>
#include<fs/FileSystem.hpp>
#include<sstream>

namespace tutorial::graphics
{
	bool Shader::_has_shader_cachae = true;

	using ShaderCompiler = std::string;
	struct CompileStrategy
	{
		std::string version;
		std::string shader_type;
		std::string suffix;
	};

	std::string _shader_res_path = "../../data/shaders/";
	CompileStrategy vs_5_0 = { "vs_5_0", "VERTEX_SHADER",		"_vs" };
	CompileStrategy gs_5_0 = { "gs_5_0", "GEOMETRY_SHADER", "_gs" };
	CompileStrategy ps_5_0 = { "ps_5_0", "PIXEL_SHADER",		"_ps" };
	CompileStrategy cs_5_0 = { "cs_5_0", "COMPUTE_SHADER",	"_cs" };

	inline void compile_shader(
		const ShaderCompiler& compiler,
		const fs::Path& path,
		const fs::Path& output_path,
		const CompileStrategy& strategy,
		const std::string& entry,
		const std::vector<std::string>& defines
	)
	{
		static std::stringstream cmd;
		cmd.clear();
		cmd << compiler;
		cmd << " /Od ";
		cmd << " /T " << strategy.version;
		cmd << " /E " << entry;   // use main as default entry
		cmd << " /I ./ ";			// pipeline closure
		cmd << " /I " << path.parent_path().string(); // include directory/
		cmd << " /D " << strategy.shader_type;
		for (const auto& define : defines) {
			cmd << " /D " << define;
		}
		cmd << " /Fo " << output_path.string();
		cmd << path.string();
		system(cmd.str().c_str());
		cmd.str("");
	}

	inline auto get_output_path(
		const fs::Path& path,
		const CompileStrategy& strategy
	)
	{
		auto output_directory =	path.parent_path();
		output_directory.concat("/fxbin/");
		auto file_name = path.stem().string();
		return output_directory.append(
			file_name +
			strategy.suffix +
			".fxo "
		);
	}

	Shader::Shader(std::string name, bool compute) noexcept :
		_name{ std::move(name) }, 
		_compute{ compute }
	{
	}

	Shader::Shader(std::string name, std::string entry, bool compute) noexcept :
		_name{ std::move(name) },
		_entry{ std::move(entry) },
		_compute{ compute }
	{
	}

	void Shader::operator=(Shader&& rhs) noexcept
	{
		_name = std::move(rhs._name);
		_entry = std::move(rhs._entry);
		_option_count = rhs._option_count;
		_options = rhs._options;
		_blocks = rhs._blocks;
		_shader_instances = std::move(rhs._shader_instances);
	}

	auto Shader::add_block(const ShaderBlock& block) noexcept -> void
	{
		assert(block.index < 32u);
		_blocks[block.index].enable = true;
		_blocks[block.index].option_offset = _option_count;
		const auto& option_strings = block.option_strings;
		std::copy_n(option_strings.begin(), option_strings.size(), _options.begin() + _option_count);
		_option_count += uint8(block.option_strings.size());
	}

	auto Shader::default() noexcept -> ShaderVersion
	{
		return ShaderVersion{ *this };
	}

	auto Shader::fetch(uint64 version) noexcept -> const ShaderInstance&
	{
		if (_shader_instances.find(version) == _shader_instances.end())
		{
			_shader_instances[version] = compile(version);
		}
		return _shader_instances[version];
	}

	auto Shader::compile(uint64 version) const noexcept
		-> ShaderInstance
	{
		std::vector<std::string> defines = {};
		for (uint32 opt = 0u; opt < 32u; ++opt)
		{
			if (version & (1 << opt))
			{
				defines.push_back(_options[opt]);
			}
		}

		static std::string compiler = std::string("fxc ");
		//compiler += " /I " + std::string(compilerDirectory) + "FX";
		auto path = _shader_res_path + _name + (_compute ? ".compute" : ".shader");
		auto output_path_base = _shader_res_path + _name + "_" + std::to_string(version);
		if (_compute)
		{
			auto output_path_cs = get_output_path(output_path_base, cs_5_0);
			compile_shader(compiler, path, output_path_cs, cs_5_0, _entry, defines);
			return ShaderInstanceLoader::load_compute(output_path_cs);
		}
		else
		{
			auto output_path_vs = get_output_path(output_path_base, vs_5_0);
			auto output_path_gs = get_output_path(output_path_base, gs_5_0);
			auto output_path_ps = get_output_path(output_path_base, ps_5_0);
			if (!_has_shader_cachae || !fs::exists(fs::status(output_path_vs)))
			{
				compile_shader(compiler, path, output_path_vs, vs_5_0, _entry, defines);
			}
			if (!_has_shader_cachae || !fs::exists(fs::status(output_path_gs)))
			{
				compile_shader(compiler, path, output_path_gs, gs_5_0, _entry, defines);
			}
			if (!_has_shader_cachae || !fs::exists(fs::status(output_path_ps)))
			{
				compile_shader(compiler, path, output_path_ps, ps_5_0, _entry, defines);
			}
			return ShaderInstanceLoader::load(output_path_vs, output_path_gs, output_path_ps);
		}
	}

	auto ShaderVersion::set_option(uint8 block, uint8 option, bool enable) noexcept 
		-> void
	{
		assert(block < 32u);
		uint8 local_option = _shader._blocks[block].option_offset + option;
		_options.set(local_option, enable);
	}

}