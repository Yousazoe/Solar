#include<gfx/entry/UniformHash.hpp>
#include<gfx/entry/ConstBuffers.hpp>
#include<map>


namespace tutorial::gfx
{
	UniformHandle UniformHandle::Invalid{ -1, UniformType::Unknown };

	static auto _get_handles() -> decltype(auto)
	{
		static std::map<std::pair<std::string, UniformType>, UniformHandle> _handles;
		return (_handles);
	}

	auto UniformHash::get(std::string name, UniformType type) -> UniformHandle
	{
		if (type == UniformType::ConstBuffer) {
			return detail::ConstBuffers::get(name);
		}

		auto& handles = _get_handles();
		auto it = handles.find({ name, type });
		if (it != handles.end()) {
			return it->second;
		}

		UniformHandle handle{ handles.size(), type };
		handles.insert({ { name,type }, handle });
		return handle;
	}

	auto UniformHash::get_or_create(std::string name, uint16 size, UniformType type) -> UniformHandle
	{
		assert(type == UniformType::ConstBuffer);
		return detail::ConstBuffers::get_or_create(name, size);
	}
}