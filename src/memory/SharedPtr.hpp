#pragma once
#include<memory>
#include<functional>
#include<memory/IAllocator.hpp>

namespace tutorial
{
	template<class T>
	class unique_ptr
	{
	private:
		using pointer = T*;

	public:
		unique_ptr(IAllocator& alloctor, void* p = nullptr);
		~unique_ptr() = default;
		unique_ptr(const unique_ptr&) = delete;
		void operator=(const unique_ptr&) = delete;
		unique_ptr(unique_ptr&& rhs);
		void operator=(unique_ptr&& rhs);
		void reset(pointer p);
		auto release();

	private:
		std::unique_ptr<T, std::function<void(T*)>> _p;
	};

	template<class T>
	inline unique_ptr<T>::unique_ptr(IAllocator& alloctpr, void* p) :
		_p{ static_cast<T*>(p), 
			[&](T* p) {
				alloctpr.deallocate_aligned(p);
			}
		}
	{
	}

	template<class T>
	inline unique_ptr<T>::unique_ptr(unique_ptr&& rhs) : 
		_p { std::move(rhs._p) } 
	{
	}
	
	template<class T>
	inline void unique_ptr<T>::operator=(unique_ptr&& rhs)
	{
		_p = std::move(rhs._p);
	}

	template<class T>
	inline void unique_ptr<T>::reset(pointer p)
	{
		_p.reset(p);
	}

	template<class T>
	inline auto unique_ptr<T>::release()
	{
		return _p.release();
	}

}
