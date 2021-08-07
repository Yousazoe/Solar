#pragma once
#include <cstdlib>
#include <cstring>
#include<memory/IAllocator.hpp>

#ifndef _WIN32
#include <malloc.h>
#endif

namespace tutorial
{
	class Allocator : public IAllocator
	{
	public:
		Allocator() {}
		~Allocator() {}

		void* allocate(size_t n);
		void deallocate(void* p);
		void* reallocate(void* ptr, size_t size);
		void* allocate_aligned(size_t size, size_t align);
		void deallocate_aligned(void* ptr);
		void* reallocate_aligned(void* ptr, size_t size, size_t align);
	};

	inline void* Allocator::allocate(size_t n)
	{
		return malloc(n);
	}

	inline void Allocator::deallocate(void* p)
	{
		free(p);
	}

	inline void* Allocator::reallocate(void* ptr, size_t size)
	{
		return realloc(ptr, size);
	}

#ifdef _WIN32
	inline void* Allocator::allocate_aligned(size_t size, size_t align)
	{
		return _aligned_malloc(size, align);
	}

	inline void Allocator::deallocate_aligned(void* ptr)
	{
		_aligned_free(ptr);
	}

	inline void* Allocator::reallocate_aligned(void* ptr, size_t size, size_t align)
	{
		return _aligned_realloc(ptr, size, align);
	}

#else

	inline void* Allocator::allocate_aligned(size_t size, size_t align)
	{
		return aligned_alloc(align, size);
	}

	inline void Allocator::deallocate_aligned(void* ptr)
	{
		free(ptr);
	}

	inline void* Allocator::reallocate_aligned(void* ptr, size_t size, size_t align)
	{
		// POSIX and glibc do not provide a way to realloc with alignment preservation
		if (size == 0) {
			free(ptr);
			return nullptr;
		}
		void* newptr = aligned_alloc(align, size);
		if (newptr == nullptr) {
			return nullptr;
		}
		memcpy(newptr, ptr, malloc_usable_size(ptr));
		free(ptr);
		return newptr;
	}
#endif
}
