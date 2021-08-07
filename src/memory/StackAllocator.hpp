#pragma once
#include <cassert>
#include <memory>
#include<memory/IAllocator.hpp>

namespace tutorial
{
	class StackAllocator : public IAllocator
	{
	public:
		struct Scope
		{
		public:
			Scope(StackAllocator &allocator, size_t space) : _allocator(allocator), _space(space) {}
			~Scope() { _allocator.rollback(_space); }

		private:
			StackAllocator & _allocator;
			size_t _space;
		};

		friend Scope;

	public:
		StackAllocator(IAllocator& allocator, size_t size) :
			_allocator(allocator),
			_data(static_cast<char*>(allocator.allocate(size))),
			_total(size),
			_free(size)
		{
		}
		~StackAllocator()
		{
			_allocator.deallocate(_data);
		}
		StackAllocator(const StackAllocator&) = delete;
		void operator=(const StackAllocator&) = delete;

		virtual void* allocate(size_t size) override;
		virtual void deallocate(void* p) override;
		virtual void* reallocate(void* p, size_t size) override;
		virtual void* allocate_aligned(size_t size, size_t alignment) override;
		virtual void deallocate_aligned(void* p) override;
		virtual void* reallocate_aligned(void* p, size_t size, size_t alignment) override;

		auto scope() { return Scope(*this, _free); }

	private:
		void rollback(size_t space)
		{
			_free = space;
		}

	private:
		IAllocator & _allocator;
		char* _data;
		size_t _total;
		size_t _free;
	};


	inline void* StackAllocator::allocate(size_t size)
	{
		if (_free < size)
		{
			return nullptr;
		}

		auto result = static_cast<void*>(_data + (_total - _free));
		_free -= size;

		return result;
	}

	inline void StackAllocator::deallocate(void* p)
	{
	}

	inline void* StackAllocator::reallocate(void* p, size_t size)
	{
		assert(false);
		return nullptr;
	}

	inline void* StackAllocator::allocate_aligned(size_t size, size_t alignment)
	{
		assert((alignment & (alignment - 1)) == 0); //power of 2
		auto result = static_cast<void*>(_data + (_total - _free));
		result = std::align(alignment, size, result, _free);
		if (!result)
		{
			assert(false && "StackAllocator full !");
			return nullptr;
		}
		_free -= size;

		return result;
	}

	inline void StackAllocator::deallocate_aligned(void* p)
	{
	}

	inline void* StackAllocator::reallocate_aligned(void* p, size_t size, size_t alignment)
	{
		assert(false);
		return nullptr;
	}
}
