#include<gfx/Gfx.hpp>
#include<gfx/entry/ConstBuffers.hpp>

#include<vector>
#include<map>

namespace tutorial::gfx::detail
{
	static std::map<std::string, UniformHandle> _handles;
	static std::vector<ConstBuffer> _buffers;
	static std::vector<std::vector<char>> _caches;

	auto ConstBuffers::get(const std::string& name) -> UniformHandle
	{
		auto it = _handles.find(name);
		if (it != _handles.end()) {
			return it->second;
		}

		UniformHandle handle{ int(_handles.size()), UniformType::ConstBuffer };
		_handles.insert({ { name }, handle });
		return handle;
	}

	auto ConstBuffers::get_or_create(const std::string& name, uint16 size) -> UniformHandle
	{
		assert(size % 16 == 0);
		auto&& handle = get(name);
		while (handle.index >= _buffers.size())
		{
			_buffers.emplace_back(ConstBuffer{});
			_caches.emplace_back();
		}

		if (!_buffers[handle.index].valid())
		{
			ResourceDesc<ConstBuffer> desc;
			desc.ByteWidth = size;
			desc.Usage = gfx::eUsage::DYNAMIC;
			desc.CPUAccessFlags = gfx::eCpuAccessFlag::CPU_ACCESS_WRITE;
			_buffers[handle.index] = create_gfx_object(desc);
		}

		if (_caches[handle.index].empty())
		{
			_caches[handle.index].resize(size);
		}

		return handle;
	}

	auto ConstBuffers::get(const UniformHandle& handle) -> const ConstBuffer*
	{
		return &_buffers[handle.index];
	}

	auto ConstBuffers::copy(
		const UniformHandle& handle, 
		const void* data, uint16 offset, uint16 size
	) -> void
	{
		char* dst = &_caches[handle.index][0];
		std::memcpy(dst + offset, data, size);
	}

	auto ConstBuffers::upload(GfxDevice* device, const UniformHandle& handle) -> void
	{
		const auto& buffer = _buffers[handle.index];
		const char* data = &_caches[handle.index][0];
		device->upload_dynamic_data(buffer, data, buffer.desc.ByteWidth);
	}

}