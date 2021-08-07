#pragma once
#include<gfx/entry/GfxObject.hpp>
#include<gfx/entry/ShaderBinding.hpp>
#include<gfx/entry/Swapchain.hpp>
#include<string_view>

namespace tutorial::gfx
{
	auto create_swapchain(void* hwnd, uint32 width, uint32 height) -> Swapchain;
	Texture2D create_texture2D(const ResourceDesc<Texture2D>& desc);
	Texture3D create_texture3D(const ResourceDesc<Texture3D>& desc);
	Texture2DArray create_texture2DArray(const ResourceDesc<Texture2DArray>& desc);
	StructureBuffer create_buffer(const ResourceDesc<StructureBuffer>& desc);
	ConstBuffer create_buffer(const ResourceDesc<ConstBuffer>& desc);
	VertexBuffer create_buffer(const ResourceDesc<VertexBuffer>& desc);
	IndexBuffer create_buffer(const ResourceDesc<IndexBuffer>& desc);
	SRV create_srv(const ResourceObject& resource, const SHADER_RESOURCE_VIEW_DESC& desc);
	RTV create_rtv(const ResourceObject& resource, const RENDER_TARGET_VIEW_DESC& desc);
	DSV create_dsv(const ResourceObject& resource, const DEPTH_STENCIL_VIEW_DESC& desc);
	UAV create_uav(const ResourceObject& resource, const UNORDERED_ACCESS_VIEW_DESC& desc);
	auto create_shader(ShaderStage type, std::string compiled_code)->Shader;
	auto compile_shader(
		ShaderStage type, const std::string& source_code, 
		const std::string& entry, const std::string& profile,
		bool save_bytecode = true
	) -> Shader;
	auto shader_reflection(const std::string_view& compiled_code) -> ShaderReflection;
	auto create_vertex_layout(std::vector<VertexElements> vb_descs, std::string_view shader_code) -> VertexLayout;
	auto create_sampler(const SAMPLER_DESC& desc) -> Sampler;

	template<class R>
	R create_resource(const ResourceDesc<R>& desc);

	template<class R, class View>
	View create_view(const R& resource, const ViewDesc<View>& desc);

	template<template<class R, class ...Views> class T, class R, class ...Views>
	void create_gfx_object(
		T<R, Views...>& object,
		const ResourceDesc<R>& resource_desc,
		const ViewDesc<Views>& ... view_descs
	);

	template<class R, class ...Views>
	GfxObject<R, Views...> create_gfx_object(
		const ResourceDesc<R>& resource_desc,
		const ViewDesc<Views>& ... view_descs
	);

	namespace detail
	{
		template<class R, class View> struct ViewFilter { using type = bool; static type filter(const R&, const ViewDesc<View>&) { assert(false);  return type(); } };

		template<class R>
		struct ViewFilter<R, SRV>
		{
			using type = SHADER_RESOURCE_VIEW_DESC;

			static type filter(const Texture2D& resource, const ViewDesc<SRV>& desc)
			{
				type res = { desc.format, SRV_DIMENSION::SRV_DIMENSION_TEXTURE2D, 0 };
				res.Texture2D.MipLevels = resource.desc.MipLevels;
				return res;
			}

			static type filter(const Texture2DArray& resource, const ViewDesc<SRV>& desc)
			{
				type res = { desc.format, SRV_DIMENSION::SRV_DIMENSION_TEXTURE2DARRAY, 0 };
				res.Texture2DArray.FirstArraySlice = 0;
				res.Texture2DArray.ArraySize = resource.desc.ArraySize;
				res.Texture2DArray.MipLevels = resource.desc.MipLevels;
				return res;
			}

			static type filter(const Texture3D& resource, const ViewDesc<SRV>& desc)
			{
				type res = { desc.format, SRV_DIMENSION::SRV_DIMENSION_TEXTURE3D, 0 };
				res.Texture3D.MipLevels = resource.desc.MipLevels;
				return res;
			}

			static type filter(const StructureBuffer& resource, const ViewDesc<SRV>& desc)
			{
				type res = { desc.format, SRV_DIMENSION::SRV_DIMENSION_BUFFER, 0 };
				res.Buffer.ElementOffset = 0;
				res.Buffer.NumElements = resource.element_count();
				return res;
			}

		};

		template<class R>
		struct ViewFilter<R, RTV>
		{
			using type = RENDER_TARGET_VIEW_DESC;

			static type filter(const Texture2D& resource, const ViewDesc<RTV>& desc)
			{
				return type{ desc.format, RTV_DIMENSION::RTV_DIMENSION_TEXTURE2D, 0 };
			}

			static type filter(const Texture2DArray& resource, const ViewDesc<RTV>& desc)
			{
				type res = { desc.format, RTV_DIMENSION::RTV_DIMENSION_TEXTURE2DARRAY, 0 };
				res.Texture2DArray.FirstArraySlice = 0;
				res.Texture2DArray.ArraySize = resource.desc.ArraySize;
				return res;
			}
		};

		template<class R>
		struct ViewFilter<R, DSV>
		{
			using type = DEPTH_STENCIL_VIEW_DESC;

			static type filter(const Texture2D& resource, const ViewDesc<DSV>& desc)
			{
				return type{ desc.format, DSV_DIMENSION::DSV_DIMENSION_TEXTURE2D, 0 };
			}

			static type filter(const Texture2DArray& resource, const ViewDesc<DSV>& desc)
			{
				type res = { desc.format, DSV_DIMENSION::DSV_DIMENSION_TEXTURE2DARRAY, 0 };
				res.Texture2DArray.FirstArraySlice = 0;
				res.Texture2DArray.ArraySize = resource.desc.ArraySize;
				return res;
			}
		};

		template<class R>
		struct ViewFilter<R, UAV>
		{
			using type = UNORDERED_ACCESS_VIEW_DESC;

			static type filter(const Texture2D& resource, const ViewDesc<UAV>& desc)
			{
				return type{ desc.format, UAV_DIMENSION::UAV_DIMENSION_TEXTURE2D, 0 };
			}

			static type filter(const Texture3D& resource, const ViewDesc<UAV>& desc)
			{
				type res = { desc.format, UAV_DIMENSION::UAV_DIMENSION_TEXTURE3D, 0 };
				res.Texture3D.MipSlice = 0;
				res.Texture3D.FirstWSlice = 0;
				res.Texture3D.WSize = -1;
				return res;
			}

			static type filter(const StructureBuffer& resource, const ViewDesc<UAV>& desc)
			{
				type res = { desc.format, UAV_DIMENSION::UAV_DIMENSION_BUFFER, 0 };
				res.Buffer.FirstElement = 0;
				res.Buffer.NumElements = resource.element_count();
				res.Buffer.Flags = desc.flag;
				return res;
			}
		};
	}

	template<class R>
	R create_resource(const ResourceDesc<R> &desc)
	{
		return gfx::detail::overload{
			[](const ResourceDesc<Texture2D>& desc) { return create_texture2D(desc); },
			[](const ResourceDesc<Texture3D>& desc) { return create_texture3D(desc); },
			[](const ResourceDesc<Texture2DArray>& desc) { return create_texture2DArray(desc); },
			[](const ResourceDesc<StructureBuffer>& desc) { return create_buffer(desc); },
			[](const ResourceDesc<ConstBuffer>& desc) { return create_buffer(desc); },
			[](const ResourceDesc<VertexBuffer>& desc) { return create_buffer(desc); },
			[](const ResourceDesc<IndexBuffer>& desc) { return create_buffer(desc); },
		}(desc);
	}

	template<class R, class View>
	View create_view(const R &resource, const ViewDesc<View>& desc)
	{
		using detail::ViewFilter;
		auto &&desc_full = ViewFilter<R, View>::filter(resource, desc);

		return gfx::detail::overload{
			[](const ResourceObject &resource, const typename ViewFilter<R, SRV>::type& desc) { return create_srv(resource, desc); },
			[](const ResourceObject &resource, const typename ViewFilter<R, RTV>::type& desc) { return create_rtv(resource, desc); },
			[](const ResourceObject &resource, const typename ViewFilter<R, DSV>::type& desc) { return create_dsv(resource, desc); },
			[](const ResourceObject &resource, const typename ViewFilter<R, UAV>::type& desc) { return create_uav(resource, desc); },
		}(static_cast<const ResourceObject&>(resource), desc_full);
	}

	namespace detail
	{
		template<class T, class ...Views> struct views_checker { static constexpr int value = 0; };
		template<class T, class V, class ...Views>
		struct views_checker<T, V, Views...>
		{
			static constexpr int value = std::is_base_of<V, T>::value +
				views_checker<T, Views...>::value;
		};
	}

	template<template<class R, class ...Views> class T,
		class R, class ...Views, class View>
	auto& write_view(T<R, Views...>& object, View&& view)
	{
		View &v = object;
		v = std::forward<View&&>(view);
		return v;
	}

	template<template<class R, class ...Views> class T, class R, class ...Views>
	void create_views(T<R, Views...>& object, const ViewDesc<Views>& ... desc)
	{
		static_assert(detail::views_checker<T<R, Views...>, Views...>::value == sizeof ...(Views), "[create_views] fail !!! --- gfx object don't have views");

		auto views_holder = [](Views& ...) {};
		views_holder(write_view(object, create_view(object, desc))...);
	}

	template<template<class R, class ...Views> class T, class R, class ...Views>
	void create_gfx_object(
		T<R, Views...>& object,
		const ResourceDesc<R>& resource_desc,
		const ViewDesc<Views>& ... view_descs
	)
	{
		static_assert(std::is_base_of<R, T<R, Views...>>::value, "[create_gfx_object] --- gfx object must have resource object");
		R &r = object;
		r = std::forward<R&&>(create_resource(resource_desc));

		// if(object.valid())
		create_views(object, view_descs...);
	}

	template<class R, class ...Views>
	GfxObject<R, Views...> create_gfx_object(
		const ResourceDesc<R>& resource_desc,
		const ViewDesc<Views>& ... view_descs
	)
	{
		GfxObject<R, Views...> object;
		create_gfx_object(object, resource_desc, view_descs...);

		return object;
	}

	
}
