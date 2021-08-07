#pragma once
#include <array>
#include <cstddef>

namespace std
{
	template <class Enum, class T, size_t Size = size_t(Enum::Count)>
	struct table
	{
		T m_values[Size] = {};
		size_t size() { return Size; }
		T* data() { return m_values; }
		const T& operator[](Enum e) const { return m_values[size_t(e)]; }
		T& operator[](Enum e) { return m_values[size_t(e)]; }
		using array = T[Size];
		operator const array&() { return m_values; }
	};
}
