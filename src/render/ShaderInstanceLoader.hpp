#pragma once
#include<fs/DiskFile.hpp>
#include<render/ShaderInstance.hpp>

namespace tutorial::graphics
{
	class ShaderInstanceLoader final
	{
		ShaderInstanceLoader() = delete;

	public:
		static auto load(
			fs::Path vs_path,
			fs::Path gs_path,
			fs::Path ps_path
		) -> ShaderInstance;

		static auto load(
			fs::Path vs_path,
			fs::Path ps_path
		) -> ShaderInstance;

		static auto load_compute(
			fs::Path path
		) -> ShaderInstance;
	};
}
