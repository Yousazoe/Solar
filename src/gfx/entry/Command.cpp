#include<gfx/Gfx.hpp>
#include<gfx/middle/GfxTraits.hpp>

namespace tutorial::gfx
{
	extern GfxDevice* _device;

	void clear_rtv(const RTV& rtv, const float color[4])
	{
		_device->clear_rtv(rtv, color);
	}

	void clear_dsv(const DSV& dsv, float depth)
	{
		_device->clear_dsv(dsv, depth);
	}

	void copy_texture2D(
		Texture2D& dst_texture,
		const uint8 dst_mip,
		const uint16 dst_x,
		const uint16 dst_y,
		const Texture2D& src_texture,
		const uint8 src_mip,
		const uint16 src_left,
		const uint16 src_top,
		const uint16 src_right,
		const uint16 src_bottom
	)
	{
		_device->copy_texture2D(
			dst_texture,
			dst_mip, dst_x, dst_y,
			src_texture,
			src_mip, src_left, src_top,
			src_right, src_bottom
		);
	}

	void copy_texture2D(
		const SubResource& dst,
		const uint16 dst_x,
		const uint16 dst_y,
		const SubResource& src,
		const uint16 src_left,
		const uint16 src_top,
		const uint16 src_right,
		const uint16 src_bottom
	)
	{
		_device->copy_texture2D(
			dst,
			dst_x, dst_y,
			src,
			src_left, src_top,
			src_right, src_bottom
		);
	}

	void upload_data(
		ResourceObject& resouce,
		uint32 start_byte,
		uint32 end_byte,
		const void* data,
		uint32 row_pitch
	)
	{
		_device->upload_data(
			resouce,
			start_byte, end_byte,
			data, row_pitch
		);
	}

	void upload_dynamic_data(
		const ResourceObject& resouce,
		const void* data, uint32 size
	)
	{
		_device->upload_dynamic_data(resouce, data, size);
	}

	void upload_texture(
		SubResource& texture,
		uint32 left,
		uint32 right,
		uint32 top,
		uint32 bottom,
		const void* data,
		uint32 src_row_pitch
	)
	{
		_device->upload_texture(
			texture,
			left, right, top, bottom,
			data, src_row_pitch
		);
	}

	void upload_texture(
		Texture2D& texture,
		uint8 mip,
		uint32 left,
		uint32 right,
		uint32 top,
		uint32 bottom,
		const void* data,
		uint32 src_row_pitch
	)
	{
		_device->upload_texture(
			texture,
			mip, left, right, top, bottom,
			data, src_row_pitch
		);
	}

	void upload_texture(
		Texture2DArray& texture,
		uint8 array_index,
		uint8 mip,
		uint32 left,
		uint32 right,
		uint32 top,
		uint32 bottom,
		const void* data,
		uint32 src_row_pitch
	)
	{
		_device->upload_texture(
			texture,
			array_index, mip, left, right, top, bottom,
			data, src_row_pitch
		);
	}

	void download_texture(Texture2D& texture, uint8 mip, void* buffer)
	{
		_device->download_texture(texture, mip, buffer);
	}

	void upload_dynamic_data(ResourceObject& resouce, const void* data, uint32 size)
	{
		_device->upload_dynamic_data(resouce, data, size);
	}
}