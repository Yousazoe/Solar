#pragma once
#include<core/Types.hpp>
#include<array>
#include<optional>
#include<render/Texture.hpp>
#include<core/Blob.hpp>

namespace tutorial::graphics
{
	class MaterialInstance final
	{
	public:
		using MaterialPropertyHandle = uint16;
		static const uint32 Max_Texture_Count = 4u;

	private:
		friend class MaterialBlueprint;
		friend class MaterialView;
		MaterialInstance(const MaterialBlueprint& bp) noexcept : bp{ bp } {}

	public:
		template<class T>
		auto set_uniform(MaterialPropertyHandle handle, const T& data);
		auto set_option(MaterialPropertyHandle handle, bool enable) -> void;
		auto submit() -> void;
		auto serialize(OutputBlob& blob) -> void;
	

	public:
		const MaterialBlueprint& bp;

	public:
		uint16 _option_count = 0u;
		std::array<bool, 8> _options;
		std::array<std::optional<Texture>, Max_Texture_Count> _textures;

	private:
		uint16 _uniform_stream_size = 0u;
		std::array<char, 256> _uniform_stream = {};
	};

	template<class T>
	inline auto MaterialInstance::set_uniform(MaterialPropertyHandle handle, const T& data)
	{
		memcpy(&_uniform_stream[handle], &data, sizeof(data));
	}
}
