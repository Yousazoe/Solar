#pragma once
#include<array>

namespace meta
{
	// types

	template<class T1, class T2> struct is_same { static constexpr bool value = false; };
	template<class T> struct is_same<T, T> { static constexpr bool value = true; };

	template<bool ...> struct bool_pack;
	template<bool ...v> using all_true = std::is_same<bool_pack<true, v...>, bool_pack<v..., true>>;

	// containers
	namespace detail
	{
		template<typename SrcType,
			typename DstType,
			size_t N,
			size_t ...I
		>
		auto array_cast_impl(
				const std::array<SrcType, N>& values,
				std::index_sequence<I...>&&
			) noexcept ->std::array<DstType, N>
		{
			auto helper = [](const SrcType& v) {
				return static_cast<DstType>(v);
			};
			return { (helper(std::get<I>(values)))... };
		}
	}

	template<
		typename DstType,
		typename SrcType,
		size_t N
	>
	auto array_cast(
			const std::array<SrcType, N>& values
		) noexcept
	{
		using Indices = std::make_index_sequence<N>;
		return detail::array_cast_impl<SrcType, DstType, N>(values, Indices{});
	}


	// functions
	template<class ...T> struct overload : T... { using T::operator()...; };
	template<class ...T> overload(T...)->overload<T...>;

	// tuples

	namespace detail
	{
		template <typename F, typename Tuple, int... I>
		constexpr void apply_impl(F&& f, Tuple&& t, std::index_sequence<I...>)
		{
			using expand = bool[];
			(void)expand
			{
				(
					f(std::get<I>(t)),
					true
					)...
			};
		}
	}


	template <typename F, typename Tuple>
	constexpr void apply(F&& f, Tuple&& t)
	{
		detail::apply_impl(
			std::forward<F>(f),
			std::forward<Tuple>(t),
			std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<Tuple>>>{}
		);
	}

}
