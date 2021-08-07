#include<render/ShaderBlock.hpp>
#include<map>

namespace tutorial::graphics
{
	static int feature_index(const std::string& name)
	{
		static std::map<std::string, int> _handles;
		auto it = _handles.find(name);
		if (it != _handles.end()) {
			return it->second;
		}

		int handle = int(_handles.size());
		_handles.insert({ name, handle });
		return handle;
	}

	ShaderBlock::ShaderBlock(std::string name, std::vector<std::string> option_strings) noexcept :
		name{ std::move(name) },
		option_strings{ std::move(option_strings) }
	{
		index = feature_index(this->name);
	}
}