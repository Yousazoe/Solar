#pragma once
#include<vector>
#include<queue>
#include<map>
#include<cassert>

namespace std
{
	template<class T>
	class ObjectPoolView;

	template<class T>
	class ObjectPool
	{
	public:
		using Handle = int;
		static const Handle InvalidHandle = 0;

	public:
		ObjectPool();
		ObjectPool(const ObjectPool&) = delete;
		void operator = (const ObjectPool&) = delete;
		~ObjectPool() = default;

	public:
		auto allocate() noexcept -> Handle;
		auto remove(const Handle& handle) noexcept -> void;
		auto get(const Handle& handle) noexcept -> T&;
		auto get(const Handle& handle) const noexcept -> const T&;

	private:
		auto resize() noexcept -> void;

	private:
		struct Element {
			Handle index;
			T value;
		};
		struct Node {
			Handle prev;
			Handle next;
			Element ele;
		};
		Handle _head = 0;
		Handle _tail = 0;
		std::vector<Node> _nodes;
		std::queue<Handle> _free_handles;

	public:
		using Type = typename Element;
		struct Iterator :
			public std::iterator<
			std::forward_iterator_tag,
			Type,
			int,
			const Type*,
			Type&
			>
		{
			using ElementList = decltype(ObjectPool<T>::_nodes);

			explicit Iterator(ElementList& elements, Handle index) : _nodes{ elements }, handle{ index } {}
			Iterator& operator++()
			{
				handle = _nodes[handle].next;
				return *this;
			}
			Iterator operator++(int) { auto retval = *this; ++(*this); return retval; }
			bool operator==(Iterator other) const { return handle == other.handle; }
			bool operator!=(Iterator other) const { return !(*this == other); }
			const Type& operator*() const {
				if (handle >= _nodes.size()) {
					throw std::runtime_error{
						"[core][container] --- ObjectPool out of range"
					};
				}
				return _nodes[handle].ele;
			}
			Type& operator*() {
				if (handle >= _nodes.size()) {
					throw std::runtime_error{
						"[core][container] --- ObjectPool out of range"
					};
				}
				return _nodes[handle].ele;
			}

		public:
			Handle handle;
		private:
			ElementList& _nodes;
		};

		//auto begin() const { return Iterator{ _nodes, _nodes[_head].next }; }
		//auto end() const { return Iterator{ _nodes, _tail }; }
		auto begin() { return Iterator{ _nodes, _nodes[_head].next }; }
		auto end() { return Iterator{ _nodes, _tail }; }
	};

	template<class T>
	inline ObjectPool<T>::ObjectPool()
	{
		resize();
		_nodes[_head].ele.index = _nodes[_head].prev = _nodes[_head].next = 0;
		_tail = _head;
	}

	template<class T>
	inline auto ObjectPool<T>::remove(const Handle& handle) noexcept -> void
	{
		_nodes[_nodes[handle].prev].next = _nodes[handle].next;
		_nodes[_nodes[handle].next].prev = _nodes[handle].prev;
		_free_handles.push(handle);
	}
	
	template<class T>
	inline auto ObjectPool<T>::get(const Handle& handle) noexcept -> T&
	{
		return _nodes[handle].ele.value;
	}

	template<class T>
	inline auto ObjectPool<T>::get(const Handle& handle) const noexcept -> const T&
	{
		return _nodes[handle].ele.value;
	}

	template<class T>
	inline auto ObjectPool<T>::allocate() noexcept -> Handle
	{
		Handle free_handle;
		if (!_free_handles.empty())
		{
			free_handle = _free_handles.front();
			_free_handles.pop();
		}
		else
		{
			free_handle = int(_nodes.size());
			resize();
		}
		_nodes[free_handle].ele.index = free_handle;
		_nodes[_nodes[_tail].prev].next = free_handle;
		_nodes[free_handle].prev = _nodes[_tail].prev;
		_nodes[_tail].prev = free_handle;
		_nodes[free_handle].next = _nodes[_tail].ele.index;
		return free_handle;
	}

	template<class T>
	inline auto ObjectPool<T>::resize() noexcept -> void
	{
		_nodes.push_back(Node{});
	}

}
