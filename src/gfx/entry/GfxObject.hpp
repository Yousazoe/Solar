#pragma once
#include<gfx/entry/Define.hpp>
#include<type_traits>
#include<utility>
#include<cassert>
#include<functional>
#include<algorithm>
#include<array>

namespace tutorial::gfx
{
	template<class T> struct empty;
	struct VertexBuffer;
	struct IndexBuffer;
	struct ConstBuffer;
	struct StructureBuffer;
	struct Texture2D;
	struct Texture3D;
	struct Texture2DArray;
	struct InstanceBuffer;
	struct CommandBuffer;
	struct CommandList;
	struct CommandQueue;
	struct SRV;
	struct RTV;
	struct DSV;
	struct UAV;
	struct Viewport;
	template<class D, class ...R> class FrameBuffer;
	template<class ...U> class ComputeBuffer;
	template<class R> struct ResourceDesc;
	struct VertexElement;
	struct VertexElements;
	struct VertexLayout;
	struct Shader;

	template<>
	struct ResourceDesc<Texture2D>
	{
		uint32 Width;
		uint32 Height;
		uint32 MipLevels;
		uint32 ArraySize;
		eFormat Format;
		SAMPLE_DESC SampleDesc;
		eUsage Usage;
		uint32 BindFlags;
		uint32 CPUAccessFlags;
		uint32 MiscFlags;
	};

	template<>
	struct ResourceDesc<Texture2DArray>
	{
		uint32 Width;
		uint32 Height;
		uint32 MipLevels;
		uint32 ArraySize;
		eFormat Format;
		SAMPLE_DESC SampleDesc;
		eUsage Usage;
		uint32 BindFlags;
		uint32 CPUAccessFlags;
		uint32 MiscFlags;
	};
	// TODO : 
	//struct ResourceDesc<Texture2DArray> : ResourceDesc<Texture2D> {};

	template<>
	struct ResourceDesc<Texture3D>
	{
		uint32 Width;
		uint32 Height;
		uint32 Depth;
		uint32 MipLevels;
		eFormat Format;
		eUsage Usage;
		uint32 BindFlags;
		uint32 CPUAccessFlags;
		uint32 MiscFlags;
	};

	template<>
	struct ResourceDesc<StructureBuffer>
	{
		uint32 ByteWidth;
		eUsage Usage;
		uint32 BindFlags;
		uint32 CPUAccessFlags;
		uint32 MiscFlags;
		uint32 StructureByteStride;
	};

	template<>
	struct ResourceDesc<ConstBuffer>
	{
		uint32 ByteWidth = 0u;
		eUsage Usage = eUsage::DEFAULT;
		uint32 CPUAccessFlags = 0u;
	};

	template<>
	struct ResourceDesc<VertexBuffer>
	{
		uint32 ByteWidth = 0u;  // vert_count * index_size
		eUsage Usage = eUsage::DEFAULT;
		uint32 CPUAccessFlags = 0u;
	};

	template<>
	struct ResourceDesc<IndexBuffer>
	{
		uint32 ByteWidth = 0u;  // index_count * index_size
		eUsage Usage = eUsage::DEFAULT;
		uint32 CPUAccessFlags = 0u;
	};
	
	namespace detail
	{
		template<bool ...> struct bool_pack;
		template<bool ...v> using all_true = std::is_same<bool_pack<true, v...>, bool_pack<v..., true>>;

		template<class ...T> struct overload : T... { using T::operator()...; };
		template<class ...T> overload(T...)->overload<T...>;
	}

	template<class View>
	struct ViewDesc;

	template<> struct ViewDesc<SRV> { eFormat format; };
	using ISRVDesc = ViewDesc<SRV>;

	template<> struct ViewDesc<RTV> { eFormat format; };
	using IRTVDesc = ViewDesc<RTV>;

	template<> struct ViewDesc<DSV> { eFormat format; };
	using IDSVDesc = ViewDesc<DSV>;

	template<> 
	struct ViewDesc<UAV> 
	{
		eFormat format; 
		union 
		{
			BUFFER_UAV_FLAG flag;
		};
	};
	using IUAVDesc = ViewDesc<UAV>;

	template<class T> struct empty { using type = T; static type value; };
	template<class T> inline typename empty<T>::type empty<T>::value;
	template<class T> inline auto &empty_v = empty<T>::value;

	struct ResourceObject
	{
	public:
		ResourceObject(GPUHandle vptr = nullptr) : vptr{ vptr } {}
		~ResourceObject() = default;
		ResourceObject(const ResourceObject&) = delete;
		ResourceObject& operator=(const ResourceObject&) = delete;
		ResourceObject(ResourceObject&& rhs) noexcept : 
			vptr{ rhs.vptr }
		{
			rhs.vptr = nullptr;
		}
		ResourceObject& operator=(ResourceObject&& rhs) noexcept
		{
			vptr = rhs.vptr;
			rhs.vptr = nullptr;
			return *this;
		}

		bool valid() const { return vptr != nullptr; }
		GPUHandle get_resource_object() const { return vptr; }

	public:
		GPUHandle vptr = nullptr;
	};

	struct SubResource
	{
		const ResourceObject& resource;
		uint32 mip_level = 0u;
		uint32 array_index = 0u;
		uint32 mip_level_count = 1u;
	};

	struct ResourceView
	{
	public:
		ResourceView() = default;
		~ResourceView() = default;
		ResourceView(const ResourceView &) = delete;
		ResourceView& operator=(const ResourceView &) = delete;
		ResourceView(ResourceView&& rhs) noexcept
		{
			vptr = rhs.vptr;
			rhs.vptr = nullptr;
		}
		ResourceView& operator=(ResourceView&& rhs) noexcept
		{
			vptr = rhs.vptr;
			rhs.vptr = nullptr;
			return *this;
		}

		bool valid() const { return vptr != nullptr; }
		GPUHandle get_view_object() const { return vptr; }

	public:
		GPUHandle vptr = nullptr;
	};

	struct ConstBuffer : public ResourceObject
	{
		ResourceDesc<ConstBuffer> desc;
	};

	struct VertexBuffer : public ResourceObject
	{
		VertexBuffer(GPUHandle handle = nullptr) noexcept : ResourceObject(handle) {}
		ResourceDesc<VertexBuffer> desc;
	};

	struct IndexBuffer : public ResourceObject
	{
		using IndexType = uint32;
		IndexBuffer(GPUHandle handle = nullptr) noexcept : ResourceObject(handle) {}
		ResourceDesc<IndexBuffer> desc;
	};

	struct StructureBuffer : public ResourceObject
	{
		uint32 element_count() const { return desc.ByteWidth / desc.StructureByteStride;  }

		ResourceDesc<StructureBuffer> desc;
	};

	struct Texture2D : public ResourceObject
	{
		uint32 width() const { return desc.Width; }
		uint32 height() const { return desc.Height; }
		ResourceDesc<Texture2D> desc;
	};

	struct Texture3D : public ResourceObject
	{
		ResourceDesc<Texture3D> desc;
	};

	struct Texture2DArray : public ResourceObject
	{
		ResourceDesc<Texture2DArray> desc;
	};

	struct InstanceBuffer : public ResourceObject
	{
		uint32 start_offset = 0;
	};

	struct Sampler : public ResourceObject
	{
		SAMPLER_DESC desc;
	};

	struct SRV : public ResourceView { };
	struct RTV : public ResourceView { };
	struct DSV : public ResourceView { };
	struct UAV : public ResourceView { };

	struct VertexElement
	{
		VertexElementType type;
		const char* semantic;
		uint8 index;
		bool per_instance;
		uint16 offset;
	};

	struct VertexElements
	{
		VertexElements() = default;
		VertexElements(uint8 stream, std::vector<VertexElement> elements) :
			stream(stream)
		{
			static const uint16 type_to_size[] =
			{
				sizeof(int),
				sizeof(float),
				2 * sizeof(float),
				3 * sizeof(float),
				4 * sizeof(float),
				sizeof(unsigned),
				sizeof(unsigned),
			}; // TODO use interface

			uint16 offset = 0u;
			for (auto &ele : elements)
			{
				ele.offset = offset;
				offset += type_to_size[(uint8)ele.type];
			}

			this->elements = elements;
			vertex_size = offset;
		}
		VertexElements(const VertexElements& rhs) = default;
		VertexElements(VertexElements&& rhs) noexcept :
			stream{ rhs.stream },
			elements { std::move(rhs.elements) },
			vertex_size { rhs.vertex_size }
		{
		}
		void operator=(VertexElements&& rhs) noexcept {
			stream = rhs.stream;
			elements = std::move(rhs.elements);
			vertex_size = rhs.vertex_size;
		}

		uint8 stream = 0u;
		std::vector<VertexElement> elements;
		uint16 vertex_size = 0u;
	};

	struct VertexLayout : public ResourceObject
	{
		VertexLayout(GPUHandle handle = nullptr) noexcept : ResourceObject(handle) {}
		VertexLayout(VertexLayout&& rhs) noexcept : ResourceObject(std::move(rhs)), stream_size(std::move(rhs.stream_size)) { }
		VertexLayout& operator=(VertexLayout&& rhs) noexcept {
			ResourceObject::operator=(std::move(rhs));
			stream_size = std::move(rhs.stream_size);
			return *this;
		}
		void set(std::vector<uint32> stream_size) {
			this->stream_size = std::move(stream_size);
		}

		std::vector<uint32> stream_size;
	};

	template<class R, class ...Views>
	class GfxObject : public R, public Views...  { };

	template<class D, class ...R>
	class FrameBuffer
	{
	private:
		using Indices = std::make_index_sequence<sizeof ...(R)>;
		static_assert(std::is_base_of_v<DSV, std::decay_t<D>>, "D must be DSV !!!");
		static_assert(detail::all_true<std::is_base_of_v<RTV, std::decay_t<R>>...>::value, "R must be RTV !!!");

	public:
		template<typename = std::enable_if_t<!std::is_reference_v<DSV>, void>>
		FrameBuffer() {}
		FrameBuffer(uint32 width, uint32 height, D&& ds, R&&... rts) : width(width), height(height), dsv(std::forward<D&&>(ds)), rtvs{ std::forward<R&&>(rts)... } {}
		FrameBuffer(FrameBuffer&) = delete;
		FrameBuffer& operator =(FrameBuffer& f) = delete;
		FrameBuffer(FrameBuffer&& f) : width(f.width), height(f.height), dsv(std::move(f.dsv)), rtvs(std::move(f.rtvs)) {}
		FrameBuffer& operator =(FrameBuffer&& f) { width = f.width; height = f.height; dsv = std::move(f.dsv); rtvs = std::move(f.rtvs); return *this; }
		~FrameBuffer() = default;
		template<size_t i> auto& get_output_rtv() const { return std::get<i>(rtvs); }
		auto get_output_dsv() { return std::ref(dsv); }
		template<typename T> auto rtvs_as() const { return get_rtvs_impl<T>(Indices{}); }
		template<typename T> auto dsv_as() const { return static_cast<T>(((const DSV&)dsv).vptr); }

	private:
		template<typename T, size_t ...I>
		std::array<T, sizeof...(R)> get_rtvs_impl(std::index_sequence<I...> &&) const
		{
			auto helper = [](const gfx::RTV &rtv) { return static_cast<T>(rtv.vptr); };
			return { (helper(std::get<I>(rtvs)))... };
		}

	public:
		uint32 width;
		uint32 height;
		D dsv;
		std::tuple<R...> rtvs;
	};
	template<class D, class ...R> FrameBuffer(uint32 width, uint32 height, D&&, R&&...)->FrameBuffer<D, R...>;

	template<class ...U>
	class ComputeBuffer
	{
	private:
		using Indices = std::make_index_sequence<sizeof ...(U)>;
		static_assert(detail::all_true<std::is_base_of_v<UAV, std::decay_t<U>>...>::value, "U must be RTV !!!");

	public:
		ComputeBuffer() {}
		ComputeBuffer(U&&... rts) : uavs{ std::forward<U&&>(rts)... } {}
		ComputeBuffer(ComputeBuffer&) = delete;
		ComputeBuffer& operator =(ComputeBuffer& f) = delete;
		ComputeBuffer(ComputeBuffer&& f) : uavs(std::move(f.uavs)) {}
		ComputeBuffer& operator =(ComputeBuffer&& f) { uavs = std::move(f.uavs); return *this; }
		~ComputeBuffer() = default;
		template<size_t i> auto get_output_uav() { return std::ref(std::get<i>(uavs)); }
		template<typename T> auto uavs_as() const { return get_uavs_impl<T>(Indices{}); }

	private:
		template<typename T, size_t ...I>
		std::array<T, sizeof...(U)> get_uavs_impl(std::index_sequence<I...> &&) const
		{
			auto helper = [](const gfx::UAV &uav) { return static_cast<T>(uav.vptr); };
			return { (helper(std::get<I>(uavs)))... };
		}

	public:
		std::tuple<U...> uavs;
	};

	template<class ...U> ComputeBuffer(U&&...)->ComputeBuffer<U...>;

	struct Viewport { uint32 x; uint32 y; uint32 width; uint32 height; };

	struct Shader : public ResourceObject
	{
		Shader() = default;
		Shader(Shader&& rhs) noexcept : ResourceObject(std::move(rhs)), type(rhs.type), bytecode(std::move(rhs.bytecode)) { }
		Shader& operator=(Shader&& rhs) noexcept {
			ResourceObject::operator=(std::move(rhs));
			type = rhs.type;
			bytecode = std::move(rhs.bytecode);
			return *this;
		}
		void set(ShaderStage type, std::string bytecode) {
			this->type = type;
			this->bytecode = std::move(bytecode);
		}
		std::string_view get_shader_code_view() const { return std::string_view(bytecode); }

		ShaderStage type = ShaderStage::UNKNOWN;
		std::string bytecode;
	};
}