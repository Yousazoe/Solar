#include<render/MaterialInstance.hpp>
#include<gfx/Gfx.hpp>

namespace tutorial::graphics
{
	auto MaterialInstance::set_option(MaterialPropertyHandle handle, bool enable) -> void
	{
		_options[handle] = enable;
	}

	auto MaterialInstance::submit() -> void
	{
		InputBlob blob{ _uniform_stream.data(), _uniform_stream_size };
		while (blob.getPosition() < _uniform_stream_size)
		{
			gfx::UniformType type = gfx::UniformType::Unknown;
			blob.read(type);
			if (type == gfx::UniformType::Unknown) {
				uint8 size; blob.read(size);
				blob.skip(size);
			} else if (type == gfx::UniformType::Srv) {
				gfx::UniformHandle handle;
				blob.read(handle);
				uint8 size; blob.read(size);
				uint8 texture_index; blob.read(texture_index);
				if (_textures[texture_index]) {
					gfx::set_texture(handle, _textures[texture_index].value().get());
				}
			} else {
				gfx::UniformHandle handle;
				blob.read(handle);
				uint8 size; blob.read(size);
				const void* val = blob.skip(size);
				gfx::set_uniform(handle, val, size);
			}
		}
	}

	auto MaterialInstance::serialize(OutputBlob& output) -> void
	{
		InputBlob blob{ _uniform_stream.data(), _uniform_stream_size };
		while (blob.getPosition() < _uniform_stream_size)
		{
			gfx::UniformType type = gfx::UniformType::Unknown;
			blob.read(type);
			if (type == gfx::UniformType::Unknown) {
				uint8 size; blob.read(size);
				blob.skip(size);
			}
			else if (type == gfx::UniformType::Srv) {
				gfx::UniformHandle handle;
				blob.read(handle);
				uint8 size; blob.read(size);
				uint8 texture_index; blob.read(texture_index);
			}
			else {
				gfx::UniformHandle handle;
				blob.read(handle);
				uint8 size; blob.read(size);
				const void* val = blob.skip(size);
				output.write(val, size);
			}
		}
	}
}