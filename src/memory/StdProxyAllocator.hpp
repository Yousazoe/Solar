#pragma once
#include<memory/IAllocator.hpp>

namespace tutorial
{
	template<class T>
	class StdProxyAllocator
	{
	public:
		typedef T value_type;
		typedef value_type* pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef std::size_t size_type;
		//typedef std::ptrdiff_t difference_type;

	public:
		template<typename U>
		struct rebind { typedef StdProxyAllocator<U> other; };

	public:
		StdProxyAllocator(IAllocator& alloc) : _alloc(alloc) {}
		~StdProxyAllocator() {}
		StdProxyAllocator(const StdProxyAllocator& rhs) : _alloc(rhs._alloc) {}
		template<typename U>
		StdProxyAllocator(const StdProxyAllocator<U>& rhs) : _alloc(rhs._alloc) {}
		bool operator==(StdProxyAllocator const&) const { return true; }
		bool operator!=(StdProxyAllocator const& a) const { return !operator==(a); }

		T* address(reference r) { return &r; }
		T* allocate(size_t n, const void* hint = nullptr);
		void deallocate(T* p, size_t);
		template <class U, class...Args>
		void construct(U* p, Args&&... args);
		void destroy(T* p);

	private:
		IAllocator& _alloc;
	private:
		template<typename U> friend class StdProxyAllocator;
	};

	template<class T>
	inline T* StdProxyAllocator<T>::allocate(size_t n, const void* hint)
	{
		return reinterpret_cast<T*>(_alloc.allocate_aligned(n * sizeof(T), sizeof(size_t) == 4 ? 8 : 16));
	}

	template<class T>
	inline void StdProxyAllocator<T>::deallocate(T* p, size_t)
	{
		_alloc.deallocate_aligned(p);
	}

	template<class T>
	template <class U, class... Args>
	inline void StdProxyAllocator<T>::construct(U* p, Args&&... args)
	{
		new(p) U(std::forward<Args>(args)...);
	}

	template<class T>
	inline void StdProxyAllocator<T>::destroy(T* p) { p->~T(); }
}
