#pragma once
#include <cassert>
#include <memory>
#include<memory/IAllocator.hpp>

namespace tutorial
{

	class LinearAllocator : public IAllocator
	{
	public:
		LinearAllocator(IAllocator& allocator, size_t size) :
			_allocator(allocator),
			_data(static_cast<char*>(allocator.allocate(size))),
			_total(size),
			_free(size)
		{
		}
		~LinearAllocator()
		{
			_allocator.deallocate(_data);
		}
		LinearAllocator(const LinearAllocator&) = delete;
		void operator=(const LinearAllocator&) = delete;

		virtual void* allocate(size_t size) override;
		virtual void deallocate(void* p) override;
		virtual void* reallocate(void* p, size_t size) override;
		virtual void* allocate_aligned(size_t size, size_t alignment) override;
		virtual void deallocate_aligned(void* p) override;
		virtual void* reallocate_aligned(void* p, size_t size, size_t alignment) override;

		void reset()
		{
			_free = _total;
		}

	private:
		IAllocator & _allocator;
		char* _data;
		size_t _total;
		size_t _free;
	};


	inline void* LinearAllocator::allocate(size_t size)
	{
		if (_free < size)
		{
			return nullptr;
		}

		auto result = static_cast<void*>(_data + (_total - _free));
		_free -= size;

		return result;
	}

	inline void LinearAllocator::deallocate(void* p)
	{
	}

	inline void* LinearAllocator::reallocate(void* p, size_t size)
	{
		assert(false);
		return nullptr;
	}

	inline void* LinearAllocator::allocate_aligned(size_t size, size_t alignment)
	{
		assert((alignment & (alignment - 1)) == 0); //power of 2
		auto result = static_cast<void*>(_data + (_total - _free));
		result = std::align(alignment, size, result, _free);
		if (!result)
		{
			assert(false && "LinearAllocator full !");
			return nullptr;
		}
		_free -= size;

		return result;
	}

	inline void LinearAllocator::deallocate_aligned(void* p)
	{
	}

	inline void* LinearAllocator::reallocate_aligned(void* p, size_t size, size_t alignment)
	{
		assert(false);
		return nullptr;
	}
}
