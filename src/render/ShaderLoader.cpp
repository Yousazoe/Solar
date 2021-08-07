#include<render/ShaderInstanceLoader.hpp>
#include<gfx/Gfx.hpp>

namespace tutorial::graphics
{
	auto create_shader = [&](
		gfx::ShaderStage type, 
		const fs::Path& path,
		gfx::ShaderBinding& all_binding
	) -> gfx::Shader
	{
		fs::IFile<fs::DeviceType::Disk> file;
		if (file.open(path, fs::OpenMode::READ))
		{
			std::string code;
			code.resize(file.size());
			file.read(&code[0], file.size());
			auto shader = gfx::create_shader(type, code);
			auto reflection = gfx::shader_reflection(code);
			gfx::ShaderBinding binding{ type, reflection };
			all_binding.merge(binding);
			assert(shader.vptr);
			return shader;
		}
		return gfx::Shader{};
	};

	auto ShaderInstanceLoader::load(
		fs::Path vs_path,
		fs::Path gs_path,
		fs::Path ps_path
	) -> ShaderInstance
	{
		gfx::ShaderBinding all_binding;
		gfx::Shader vs = create_shader(gfx::ShaderStage::VS, vs_path, all_binding);
		gfx::Shader gs = create_shader(gfx::ShaderStage::GS, gs_path, all_binding);
		gfx::Shader ps = create_shader(gfx::ShaderStage::PS, ps_path, all_binding);

		return ShaderInstance{
			std::move(vs),
			std::move(gs),
			std::move(ps),
			std::move(all_binding)
		};
	}

	auto ShaderInstanceLoader::load(fs::Path vs_path, fs::Path ps_path)
		-> ShaderInstance
	{
		gfx::ShaderBinding all_binding;
		gfx::Shader vs = create_shader(gfx::ShaderStage::VS, vs_path, all_binding);
		gfx::Shader ps = create_shader(gfx::ShaderStage::PS, ps_path, all_binding);

		return ShaderInstance{
			std::move(vs),
			gfx::Shader{},
			std::move(ps),
			std::move(all_binding)
		};
	}

	auto ShaderInstanceLoader::load_compute(fs::Path path) -> ShaderInstance
	{
		gfx::ShaderBinding all_binding;
		gfx::Shader cs = create_shader(gfx::ShaderStage::CS, path, all_binding);

		return ShaderInstance{
			std::move(cs),
			std::move(all_binding)
		};
	}
}