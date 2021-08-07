#include<gfx/entry/TextureLoader.hpp>
#include<gfx/middle/GfxTraits.hpp>
#include<../thirdparty/dds-loader/DDSTextureLoader.h>
#include<fstream>

namespace tutorial::gfx
{
	extern GfxDevice* _device;

	auto TextureLoader::load(const fs::Path& path)
		-> GfxObject<Texture2D, SRV>
	{
		DirectX::DDS_HEADER* header = nullptr;
		uint8_t* bitData = nullptr;
		size_t bitSize = 0;
		std::unique_ptr<uint8_t[]> ddsData;
		HRESULT hr = DirectX::LoadTextureDataFromFile(
			path.wstring().c_str(),
			ddsData,
			&header,
			&bitData,
			&bitSize
		);
		if (FAILED(hr))
		{
			throw std::runtime_error{
				"[gfx] TextureLoader::load ---  load texture fail " +
				path.string()
			};
		}

		std::fstream file;
		file.open(path, std::ios::binary | std::ios::in);
		file.seekg(0, std::ios::end);
		auto size = (size_t)file.tellg();
		file.seekg(0, std::ios::beg);

		Texture2D resource;
		SRV srv;
		if (FAILED(DirectX::CreateDDSTextureFromMemory(
			_device->get_device(),
			ddsData.get(), size,
			(ID3D11Resource**)&resource.vptr,
			(ID3D11ShaderResourceView**)&srv.vptr)))
		{
			throw std::runtime_error{
				"[gfx] TextureLoader::load ---  create texture fail " +
				path.string()
			};
		}

		auto format = DirectX::GetDXGIFormat(header->ddspf);
		resource.desc = ResourceDesc<Texture2D>{
			header->width,
			header->height,
			1, 1,
			(eFormat)format,
			{ 1, 0, },
			eUsage::DEFAULT,
			eBindFlag::SHADER_RESOURCE, 0
		};

		return GfxObject<Texture2D, SRV> { std::move(resource), std::move(srv) };
	}
}
