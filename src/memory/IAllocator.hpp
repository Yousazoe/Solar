#pragma once

namespace tutorial
{

	class IAllocator
	{
	public:
		virtual ~IAllocator() {}
		virtual void* allocate(size_t size) = 0;
		virtual void deallocate(void* p) = 0;
		virtual void* reallocate(void* p, size_t size) = 0;
		virtual void* allocate_aligned(size_t size, size_t alignment) = 0;
		virtual void deallocate_aligned(void* p) = 0;
		virtual void* reallocate_aligned(void* p, size_t size, size_t alignment) = 0;
	};

	template<class T, class...Args>
	inline auto newpp(IAllocator& alloc, Args&&... args)
	{
		return ::new(alloc.allocate_aligned(sizeof(T), alignof(T)))T{ std::forward<Args...>(args...) };
	}

	template<class T, class...Args>
	inline auto newpp(IAllocator& alloc)
	{
		return ::new(alloc.allocate_aligned(sizeof(T), alignof(T)))T();
	}
}