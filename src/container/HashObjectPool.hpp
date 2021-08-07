#pragma once
#include<vector>
#include<queue>
#include<map>
#include<cassert>

namespace std
{
	template<class T>
	class HashObjectPoolView;

	template<class T>
	class HashObjectPool
	{
	public:
		using GlobalIndex = int;
		using Handle = int;
		static const Handle InvalidHandle = 0;

	public:
		HashObjectPool();
		HashObjectPool(const HashObjectPool&) = delete;
		void operator = (const HashObjectPool&) = delete;
		~HashObjectPool() = default;

	public:
		auto create(const GlobalIndex& index) noexcept -> Handle;
		auto remove(const GlobalIndex& index) noexcept -> void;
		auto get_global(const GlobalIndex& index) noexcept->T&;
		auto get_global(const GlobalIndex& index) const noexcept -> const T&;
		auto get_local(const Handle& handle) noexcept -> T&;
		auto get_local(const Handle& handle) const noexcept -> const T&;

	private:
		auto allocate() noexcept ->Handle;
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
		std::map<GlobalIndex, Handle> _global_to_local;

		friend class HashObjectPoolView<T>;
	};

	template<class T>
	inline HashObjectPool<T>::HashObjectPool()
	{
		resize();
		_nodes[_head].ele.index = _nodes[_head].prev = _nodes[_head].next = 0;
		_tail = _head;
	}

	template<class T>
	inline auto HashObjectPool<T>::create(const GlobalIndex& index) noexcept -> Handle
	{
		auto it = _global_to_local.find(index);
		if (it == _global_to_local.end())
		{
			auto handle = allocate();
			_global_to_local[index] = handle;
			return handle;
		} else {
			return it->second;
		}
	}

	template<class T>
	inline auto HashObjectPool<T>::remove(const GlobalIndex& index) noexcept -> void
	{
		auto it = _global_to_local.find(index);
		if (it == _global_to_local.end()) {
			return;
		}
		Handle handle = it->second;
		_nodes[_nodes[handle].prev].next = _nodes[handle].next;
		_nodes[_nodes[handle].next].prev = _nodes[handle].prev;
		_free_handles.push(handle);
		_global_to_local.erase(index);
	}

	template<class T>
	inline auto HashObjectPool<T>::get_global(const GlobalIndex& index) noexcept ->T &
	{
		auto it = _global_to_local.find(index);
		assert(it != _global_to_local.end());
		Handle handle = it->second;
		return _nodes[handle].ele.value;
	}

	template<class T>
	inline auto HashObjectPool<T>::get_global(const GlobalIndex& index) const noexcept -> const T&
	{
		auto it = _global_to_local.find(index);
		assert(it != _global_to_local.end());
		Handle handle = it->second;
		return _nodes[handle].ele.value;
	}

	template<class T>
	inline auto HashObjectPool<T>::get_local(const Handle& handle) noexcept -> T&
	{
		return _nodes[handle].ele.value;
	}

	template<class T>
	inline auto HashObjectPool<T>::get_local(const Handle& handle) const noexcept -> const T&
	{
		return _nodes[handle].ele.value;
	}

	template<class T>
	inline auto HashObjectPool<T>::allocate() noexcept -> Handle
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
	inline auto HashObjectPool<T>::resize() noexcept -> void
	{
		_nodes.push_back(Node{});
	}


	template<class T>
	class HashObjectPoolView
	{
		using ElementList = decltype(HashObjectPool<T>::_nodes);
		using Type = typename HashObjectPool<T>::Element;

		struct Iterator :
			public std::iterator<
			std::forward_iterator_tag,
			Type,
			int,
			const Type*,
			Type&
			>
		{
			explicit Iterator(const ElementList& elements, int index) : _nodes{ elements }, index{ index } {}
			Iterator& operator++()
			{
				index = _nodes[index].next;
				return *this;
			}
			Iterator operator++(int) { auto retval = *this; ++(*this); return retval; }
			bool operator==(Iterator other) const { return index == other.index; }
			bool operator!=(Iterator other) const { return !(*this == other); }
			const Type& operator*() const {
				if (index >= _nodes.size()) {
					throw std::runtime_error{
						"[core][container] --- HashObjectPoolView out of range"
					};
				}
				return _nodes[index].ele;
			}

		public:
			int index;
		private:
			const ElementList& _nodes;
		};

	public:
		HashObjectPoolView(const HashObjectPool<T>& pool) : _pool{ pool } {}
		auto begin() const { return Iterator{ _pool._nodes, _pool._nodes[_pool._head].next }; }
		auto end() const { return Iterator{ _pool._nodes, _pool._tail }; }

	private:
		const HashObjectPool<T>& _pool;
	};
	template<class T> HashObjectPoolView(const HashObjectPool<T>&) -> HashObjectPoolView<T>;

}
